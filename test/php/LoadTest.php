<?php
	namespace Casscoon\Tests;

	use PHPUnit_Framework_TestCase;

	/**
	 * @author Carlos D'Agostino <carlos@basebone.com>
	 */
	class LoadTest extends PHPUnit_Framework_TestCase {
		const BB_CASSANDRA_EXTENSION_FILE = 'bb_cassandra.so';
		const BB_CASSANDRA_EXTENSION 	  = 'bb_cassandra';

		/**
		 * Test that the php version is compatible with the extension
		 */
		public function testPhpVersion() {
			$this->assertTrue(version_compare(phpversion(), '5.3', '>='));
		}

		/**
		 * Test that the extension exists in the file system
		 */
		public function testExtensionExists() {
			$extension_dir = rtrim(trim(`php-config --extension-dir`), '/') . '/';
			$this->assertFileExists($extension_dir . self::BB_CASSANDRA_EXTENSION_FILE);
		}

		/**
		 * Test that the extension is loaded or that it can load correctly
		 * @depends testPhpVersion
		 * @depends testExtensionExists
		 */ 
		public function testExtensionLoads() {
			$this->assertTrue(extension_loaded(self::BB_CASSANDRA_EXTENSION));
		}

		/**
		 * Once the extension has loaded, test that it exists via class_exists
		 * @depends testExtensionLoads
		 */
		public function testClassesExist() {
			$this->assertTrue(class_exists('Casscoon\Query'));
			$this->assertTrue(class_exists('Casscoon\Batch'));
			$this->assertTrue(class_exists('Casscoon\Connection'));
			$this->assertTrue(class_exists('Casscoon\Consistency'));
			$this->assertTrue(class_exists('Casscoon\Data'));
			$this->assertTrue(class_exists('Casscoon\DataType'));
		}
	}
