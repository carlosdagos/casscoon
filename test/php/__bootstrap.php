<?php
	if (file_exists('vendor/autoload.php')) {
		require 'vendor/autoload.php';
	} else {
		die('Cannot continue without dependencies, have you run "composer install"?');
	}

	require 'TestException.php';

	use Casscoon\Tests\TestException;

	set_error_handler(function ($code, $message, $file, $line, $context) {
		throw new TestException("$message at $file:$line", $code, 0, $file, $line);
	});

	date_default_timezone_set('UTC');

//	error_reporting(E_ALL & E_STRICT);
