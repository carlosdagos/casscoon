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
 * Class to make data types that the driver understands
 *
 * @author Carlos D'Agostino <carlos@basebone.com>
 */
class Data
{
	/**
	 * Use this function to turn the variable into a data type that the driver
	 * can understand.
	 *
	 * It is very important to set the data type appropriate to the CQL definition
	 * because otherwise the internal binding will be wrong and you will get 
	 * 'Invalid Query' errors.
	 *
	 * @see  DataType
	 *
	 * @param mixed $var			Variable to turn into readable data type
	 * @param const $dataTypeConstant 	Constant from DataType which belongs to the variable
	 *
	 * @return array
	 */
	public static function make($var, $dataTypeConstant) {}
	/**
	 * Use this function to turn the variable into a map type that the driver
	 * can understand.
	 *
	 * It is very important to set the key and data types to the appropriate to the CQL definition
	 * because otherwise the internal binding will be wrong and you will get  'Invalid Query' errors.
	 *
	 * @see  DataType
	 *
	 * @param const $keyType 	Constant from DataType which belongs to the key variable
	 * @param const $dataType 	Constant from DataType which belongs to the data variable
	 * @param array $data 		PHP map with associated data
	 *
	 * @return array
	 */
	public static function makeMap($keyType, $dataType, array $data) {}
	/**
	 * Use this function to turn the variable into a set type that the driver
	 * can understand.
	 *
	 * It is very important to set the data type to the appropriate to the CQL definition
	 * because otherwise the internal binding will be wrong and you will get  'Invalid Query' errors.
	 *
	 * @see  DataType
	 *
	 * @param const $dataType 	Constant from DataType which belongs to the data variable
	 * @param array $data 		PHP map with associated data
	 *
	 * @return array
	 */
	public static function makeSet($dataType, array $data) {}
	/**
	 * Use this function to turn the variable into a list type that the driver
	 * can understand.
	 *
	 * It is very important to set the data type to the appropriate to the CQL definition
	 * because otherwise the internal binding will be wrong and you will get  'Invalid Query' errors.
	 *
	 * @see  DataType
	 *
	 * @param const $dataType 	Constant from DataType which belongs to the data variable
	 * @param array $data 		PHP map with associated data
	 *
	 * @return array
	 */
	public static function makeList($dataType, array $data) {}
}
