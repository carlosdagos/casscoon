<?php
	use Casscoon\Data;
	use Casscoon\DataType;

	echo sprintf("\n %s \n", 'This is a MAP');
	$map = Data::makeMap(DataType::STRING, DataType::INT, array(
		'carlos' => 25,
		'daniel' => 25,
		'zeus'   => 24, // ¬¬
	));

	print_r($map);

	echo sprintf("\n %s \n", 'This is a SET');
	$list = Data::makeSet(DataType::TIMESTAMP, array(
		time(), // now
		time() + (24 * 60 * 60), // tomorrow this time
		time() + (7  * 24 * 60 * 60), // next week this time
	));

	print_r($list);

	echo sprintf("\n %s \n", 'This is a LIST');
	$set = Data::makeList(DataType::STRING, array(
		'carlos',
		'carlos rules',
		'yay :D'
	));

	print_r($set);