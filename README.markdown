Discontinued :)
===============

Please see

[https://github.com/datastax/php-driver](https://github.com/datastax/php-driver)

Thanks! 

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

You could always include that into `/etc/bash.bashrc` and use `ldconfig` to reload your system library cache. You can check that the libraries exist in the path with `ldconfig -p`.

If that still doesn't work, please let me know.

## Example

- A simple Example can be
	- [View Example](https://github.com/charlydagos/casscoon/blob/master/spike/simple_cql_test.php)

- For more complex data types
	- [View Example](https://github.com/charlydagos/casscoon/blob/master/spike/complex_cql_test.php)

### Installation

+ Check out the project
+ Check out the submodules
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
