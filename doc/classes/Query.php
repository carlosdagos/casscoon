<?php
/**
 *  Copyright (c) 2014 Basebone
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * 
 * You may obtain a copy of the License at
 *   
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
namespace Casscoon;
	
/**
 * Query class, represents a simple query
 *
 * @author Carlos D'Agostino <carlos@basebone.com>
 */
class Query 
{
	/**
	 * Instantiation is done by Connection class
	 *
	 * @see Connection
	 * 
	 * <code> 
	 * 	$query = $db->query('SELECT * FROM example_keyspace.example_table');
	 * 	$query->exec();
	 * </code>
	 * 
	 * @param string $query
	 * @param Connection $connection
	 */ 	
	protected function __construct($query, Connection $connection) {}
	/**
	 * Binds an array of parameters to the object
	 *
	 * Will throw an exception if number of parameters don't match the number of ?'s (placeholders)
	 * Throws exception if there's any problem with binding any parameter
	 *
	 * <code>
	 * 	$query = $db->query('INSERT INTO example_keyspace.example_table(key, value) VALUES (?, ?)');
	 * 	$query->prepare(array(1, 'text example value'));
	 * 	$query->exec();
	 * </code>
	 *
	 * @param array $params Array of parameters to bind to query
	 *
	 * @throws Exception
	 */
	public function prepare(array $params) {}
	/**
	 * Send the query to the connection to be executed
	 *
	 * @throws Exception
	 */
	public function exec() {}
	/**
	 * @return number The amount of rows in the query
	 */
	public function numRows() {}
	/**
	 * Return the next array in the set
	 * 
	 * @return array
	 */
	public function fetchArray() {}
	/**
	 * Return the next object in the set
	 * 
	 * @return \stdClass
	 */
	public function fetchObject() {}
	/**
	 * Set the consistency for the query, will throw Exception if the consistency
	 * value is not recognized
	 *
	 * @param $consistency The consistency value for the query
	 * @see Consistency
	 * @throws Exception 
	 */
	public function setConsistency($consistency) {}
	/**
	 * Set the serial consistency value for the query, will throw Exceptin if the serial
	 * consistency value is not recognized
	 *	
	 * @param $serial_consistency The serial consistency value for the query
	 * @see Consistency
	 * @throws Exception
	 */
	public function setSerialConsistency($serial_consistency) {}
}
