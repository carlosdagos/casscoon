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
 * Consistency constants
 *
 * @see http://www.datastax.com/documentation/cassandra/2.0/cassandra/dml/dml_config_consistency_c.html
 *
 * @author Carlos D'Agostino <carlos@basebone.com>
 */
final class Consistency
{
	const ANY = "ANY";
	const ONE = "ONE";
	const TWO = "TWO";
	const THREE = "THREE";
	const QUORUM = "QUORUM";
	const ALL = "ALL";
	const LOCAL_QUORUM = "LOCAL_QUORUM";
	const SERIAL = "SERIAL";
	const LOCAL_SERIAL = "LOCAL_SERIAL";
	const LOCAL_ONE = "LOCAL_ONE";
}
