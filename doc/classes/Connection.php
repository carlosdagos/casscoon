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
 * Connection class, represents a client session 
 *
 * @author Carlos D'Agostino <carlos@basebone.com>
 */
class Connection
{	
	/**
	 * @param $options['hosts'] (Required) Hosts string separated by commas
	 *        $options['port']             Port integer, by default set to default native port 
	 *        $options['logger']           PSR3 Logger to log info by the connection
	 */
	public function __construct(array $options) {}
	/**
	 * Initialize the query with the string parameter, throws Exception for empty strings
	 *
	 * @param string $queryString 
	 *
	 * @throws Exception
	 *
	 * @return Query
	 */
	public function query($queryString);
	/**
	 * Initialize a batch object 
	 *
	 * @return Batch
	 */
	public function batch();	
}
