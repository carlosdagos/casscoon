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
