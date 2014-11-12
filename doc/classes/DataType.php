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
 * Data Type Constants
 *
 * @see http://www.datastax.com/documentation/cql/3.0/cql/cql_reference/cql_data_types_c.html
 *
 * There's two aliases, STRING and INT which map to TEXT and INTEGER respectively.
 * 
 * I decided to add these two due to the lexical prevalence in php. 
 *
 * @author Carlos D'Agostino <carlos@basebone.com>
 */
final class DataType
{
	const ASCII 	= "ASCII";
	const BIGINT 	= "BIGINT";
	const BLOB 		= "BLOB";
	const BOOLEAN 	= "BOOLEAN";
	const COUNTER 	= "COUNTER";
	const DECIMAL 	= "DECIMAL";
	const DOUBLE 	= "DOUBLE";
	const FLOAT 	= "FLOAT";
	const INET 		= "INET";
	const INTEGER 	= "INTEGER";
	const INT 		= "INTEGER";
	const LIST 		= "LIST";
	const MAP 		= "MAP";
	const SET 		= "SET";
	const TEXT 		= "TEXT";
	const STRING 	= "TEXT";
	const TIMESTAMP = "TIMESTAMP";
	const UUID 		= "UUID";
	const TIMEUUID 	= "TIMEUUID";
	const VARCHAR 	= "VARCHAR";
	const VARINT 	= "VARINT";
}
