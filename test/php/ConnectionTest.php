<?php
	namespace Casscoon\Tests;

	use PHPUnit_Framework_TestCase;
	use Casscoon\Query;
	use Casscoon\Connection;

	/**
	 * @author Carlos D'Agostino <carlos@basebone.com>
	 */
	class ConnectionTest extends PHPUnit_Framework_TestCase {
		/**
	 	 * Test that constructor requires parameters
		 * @expectedException Casscoon\Tests\TestException
		 */
		public function testCreationRequiresParameters() {
			$connection = new Connection();
		}

		public function testCreationAcceptsMandatoryParameters() {
			$connection = $this->getConnection();
			$this->assertTrue(is_object($connection));
		}

		/**
		 * Get a working connection
		 * @return Connection
		 */
		protected function getConnection() {
			return new Connection(array('hosts' => '127.0.0.1', 'port' => 9042));
		}
	}
