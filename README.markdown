Casscoon: PHP Extension for Cassandra [[alpha]]
===============================================

PHP Driver for Cassandra that wraps the [Datastax Official C++ Driver](https://github.com/datastax/cpp-driver), using the [PHP CPP development tools by Copernica](https://github.com/CopernicaMarketingSoftware/PHP-CPP). 

Being developed at [Basebone](http://basebone.com).

## Important

- This still an incomplete implementation. As it stands it can do simple querying. **It is under development and testing**.
- Tests are incomplete as of yet.
- Needs more error checking. If you confuse your data types, you will get errors.
- Connection pooling is still in the works!
- No authentication yet. Working on it currently.

Sometimes you'll get an error about `"libcassandra.so": No such file or directory`

This is because your LD_LIBRARY_PATH is empty, and the driver needs that library to work after it has been linked and compiled. Normally this can be solved by doing

	export LD_LIBRARY_PATH=/usr/local/lib/

If that still doesn't work, please let me know.

Lastly, you must have [Monolog](https://github.com/Seldaek/monolog) *somewhere* in your include path. This dependency will be removed in the near future.

## Example

- A simple Example can be

	```
	<?php

	use Casscoon\Connection;
	use Casscoon\Data;
	use Casscoon\DataType;
	use Monolog\Logger;
	use Monolog\Handler\StreamHandler;

	$handler = new StreamHandler('php://stdout', Logger::DEBUG);
	$logger  = new Logger(basename(__FILE__));
	$logger->pushHandler($handler);

	$logger->info('Initializing...');

	$db = new Connection(array(
		'hosts'  => '127.0.0.1',
		'logger' => $logger,
	));

	$db->query('DROP KEYSPACE IF EXISTS examples')->exec();

	$db->query(
			"CREATE KEYSPACE examples
			  WITH replication = {
					'class': 'SimpleStrategy',
					'replication_factor': '3'}"
	)->exec();

	$db->query('DROP TABLE IF EXISTS examples.simple')->exec();

	$db->query('CREATE TABLE examples.simple(key int PRIMARY KEY, value text)')->exec();

	$query = $db->query('INSERT INTO examples.simple("key", "value") VALUES (?, ?)');
	$query->prepare(array(
		Data::make(1, DataType::INTEGER), 
		Data::make("hello world!", DataType::TEXT),
	));
	$query->exec();

	$query = $db->query('INSERT INTO examples.simple("key", "value") VALUES (?, ?)');
	$query->prepare(array(
		Data::make(2, DataType::INTEGER), 
		Data::make("bye world!", DataType::TEXT),
	));
	$query->exec();

	$query = $db->query('INSERT INTO examples.simple("key", "value") VALUES (?, ?)');
	$query->prepare(array(
		Data::make(3, DataType::INTEGER), 
		Data::make("I forgot something hehe... waves awkwardly with awkward smile", DataType::TEXT),
	));
	$query->exec();

	$cql = $db->query('SELECT * FROM examples.simple');
	$cql->exec();

	while ($row = $cql->fetchObject()) {
		$logger->info('Fetched row', (array) $row);
	}

	$db->query('DROP TABLE IF EXISTS examples.simple_counters')->exec();
	$db->query(
		"CREATE TABLE examples.simple_counters(key int PRIMARY KEY, value counter, another_value counter)"
	)->exec();

	$counters = $db->query('UPDATE examples.simple_counters SET "value" = "value" + ?, "another_value" = "another_value" + ? WHERE "key" = ?');
	$counters->prepare(array(
		Data::make(1, DataType::COUNTER), 
		Data::make(10, DataType::COUNTER),
		Data::make(1, DataType::INTEGER)
	));
	$counters->exec();
	```

- For more complex data types

	```
	<?php

	use Casscoon\Connection;
	use Casscoon\Data;
	use Casscoon\DataType;
	use Monolog\Logger;
	use Monolog\Handler\StreamHandler;
	use Monolog\Formatter\LineFormatter;
	use Colors\Color;

	$c = new Color();

	$handler = new StreamHandler('php://stdout', Logger::DEBUG);
	$handler->setFormatter(new LineFormatter(
		"[%level_name%]: %message% " .
		$c("%context% ")->yellow() .
		$c("%extra% \n")->dark_gray()
	));
	$logger  = new Logger(basename(__FILE__));
	$logger->pushHandler($handler);

	$logger->info('Initializing...');

	true && set_exception_handler(function(Exception $e) use ($logger) {
		$logger->critical($e->getMessage());
	});

	$db = new Connection(array(
		'hosts'  => '127.0.0.1',
		'logger' => $logger,
	));

	$db->query('DROP KEYSPACE IF EXISTS examples')->exec();

	$db->query(
			"CREATE KEYSPACE examples
			  WITH replication = {
					'class': 'SimpleStrategy',
					'replication_factor': '3'}"
	)->exec();

	$db->query('DROP TABLE IF EXISTS examples.simple_map')->exec();
	$db->query('DROP TABLE IF EXISTS examples.simple_list')->exec();
	$db->query('DROP TABLE IF EXISTS examples.simple_set')->exec();

	$db->query('CREATE TABLE examples.simple_map(key int PRIMARY KEY, value map<text, text>)')->exec();
	$db->query('CREATE TABLE examples.simple_list(key int PRIMARY KEY, value list<text>)')->exec();
	$db->query('CREATE TABLE examples.simple_set(key int PRIMARY KEY, value set<text>)')->exec();

	$query = $db->query('INSERT INTO examples.simple_map (key, value) VALUES (?, ?)');
	$query->prepare(array(
		Data::make(1, DataType::INT),
		Data::makeMap(DataType::STRING, DataType::STRING, array(
			'example_key' => 'example_value',
			'another_example_key' => 'another example_value',
		))
	));
	$query->exec();

	$query = $db->query('INSERT INTO examples.simple_list (key, value) VALUES (?, ?)');
	$query->prepare(array(
		Data::make(1, DataType::INT),
		Data::makeList(DataType::STRING, ['carlos', 'danielo', 'carlos'])
	));
	$query->exec();

	$query = $db->query('INSERT INTO examples.simple_set (key, value) VALUES (?, ?)');
	$query->prepare(array(
		Data::make(1, DataType::INT),
		Data::makeSet(DataType::STRING, ['carlos', 'danielo', 'carlos'])
	));
	$query->exec();

	$select = $db->query('SELECT * FROM examples.simple_map');
	$select->exec();
	while ($row = $select->fetchArray()) {
		print_r($row);
	}

	$select = $db->query('SELECT * FROM examples.simple_list');
	$select->exec();
	while ($row = $select->fetchArray()) {
		print_r($row);
	}

	$select = $db->query('SELECT * FROM examples.simple_set');
	$select->exec();
	while ($row = $select->fetchArray()) {
		print_r($row);
	}
	```

### Installation

+ Check out the project
+ First install `include/cpp-driver` (read more [here](https://github.com/datastax/cpp-driver#building))

	```
	pushd include/cpp-driver
	cmake .
	make 
	make install
	popd
	```
+ Secondly, install `include/php-cpp` (read more [here](http://www.php-cpp.com/documentation/install))

	```
	pushd include/php-cpp
	make
	make install
	popd
	```
+ Lastly, install this extension

	```
	make
	make install
	```

### To compile on OS X 

And to keep compatibility across my Ubuntu VMs, replace the LINKER_FLAG line in `include/php-cpp/Makefile`

	ifeq ($(shell uname), Darwin)
	LINKER_FLAGS        =   -shared -undefined dynamic_lookup
	else
	LINKER_FLAGS        =   -shared
	endif

### Compatibility

I have tested this on default PHP installations with Ubuntu 14.04.1 LTS (5.5.9-1ubuntu4.5), on a OSX 10.10 on a MacBook Pro Retina, Mid 2012 (PHP 5.4.23), and an iMac (27-inch, Mid 2010) (PHP 5.4.32).

## Credits

+ [Datastax](https://github.com/datastax/): Very active and very helpful.
+ [PHP-CPP Project](https://github.com/CopernicaMarketingSoftware/PHP-CPP): It rules.

### Folder Structure

Taken from [Hilton Lipschitz](http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/)

+ **bin**: The output executables go here, both for the app and for any tests and spikes.
+ **build**: This folder contains all object files, and is removed on a clean.
+ **doc**: Any notes, like my assembly notes and configuration files, are here. I decided to create the development and production config files in here instead of in a separate config folder as they “document” the configuration.
+ **include**: All project header files. All necessary third-party header files that do not exist under /usr/local/include are also placed here.
+ **lib**: Any libs that get compiled by the project, third party or any needed in development. Prior to deployment, third party libraries get moved to /usr/local/lib where they belong, leaving the project clean enough to compile on our Linux deployment servers. I really use this to test different library versions than the standard.
+ **spike**: I often write smaller classes or files to test technologies or ideas, and keep them around for future reference. They go here, where they do not dilute the real application’s files, but can still be found later.
+ **src**: The application and only the application’s source files.
+ **test**: All test code files. You do write tests, no?

### The name

Cassandra + Laocoon. Inspired from [here](http://ancienthistory.about.com/od/aeneadcharacters/ss/062309AeneidCharcters.htm). He's said to have been the only one to believe Cassandra, which didn't quite turn out too well for him nonetheless. But that's another matter.

# License
Copyright (c) 2014 Basebone

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
