<?php
	namespace Casscoon\Tests;

	use \ErrorException;

	/**
	 * @author Carlos D'Agostino <carlos@basebone.com>
	 */
	class TestException extends ErrorException {
		public function __construct($msg, $code, $ecode, $file, $line) {
			parent::__construct($msg, $code, $ecode, $file, $line);
		}
	}