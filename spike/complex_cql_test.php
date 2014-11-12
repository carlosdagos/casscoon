<?php

ini_set('memory_limit', '1G');

require sprintf('%s/test/php/__bootstrap.php', dirname(__DIR__));

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