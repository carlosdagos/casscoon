/**
  Copyright (c) 2014 Basebone

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.

  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

#include <stdio.h>
#include <phpcpp.h>
#include <string.h>
#include <cassandra.h>
#include <iostream>

#include "Consistency.h"
#include "Batch.h"
#include "Query.h"
#include "Connection.h"
#include "Data.h"

extern "C" {
	PHPCPP_EXPORT void *get_module()
	{
		static Php::Extension extension("bb_cassandra", "1.0");

		/**
		* Add the CQL class
		*/
		Php::Class<Query> cql("Casscoon\\Query");

		cql.method("exec", &Query::exec, {});

		cql.method("prepare", &Query::prepare, {
			Php::ByVal("variables", Php::Type::Array)
		});

		cql.method("setConsistency", &Query::setConsistency, {
			Php::ByVal("consistency", Php::Type::Numeric)
		});

		cql.method("setSerialConsistency", &Query::setSerialConsistency, {
			Php::ByVal("serial_consistency", Php::Type::Numeric)
		});

		cql.method("nr", &Query::nr);
		cql.method("numRows", &Query::nr);

		cql.method("fa", &Query::fa);
		cql.method("fetchArray", &Query::fa);

		cql.method("fo", &Query::fo);
		cql.method("fetchObject", &Query::fo);

		extension.add(std::move(cql));

		/**
		* Add the Batch class
		*/
		Php::Class<Batch> batch("Casscoon\\Batch");

		batch.method("add", &Batch::add, {
			Php::ByVal("query", "Casscoon\\Query")
		});

		batch.method("apply", &Batch::apply);

		extension.add(std::move(batch));

		/**
		* Add the Connection class
		*/
		Php::Class<Connection> connection("Casscoon\\Connection");

		connection.method("__construct", &Connection::__construct, {
			Php::ByVal("options", Php::Type::Array)
		});

		connection.method("query", &Connection::query, {
			Php::ByVal("querystring", Php::Type::String)
		});

		connection.method("batch", &Connection::batch, {});

		extension.add(std::move(connection));

		/**
		* Add the Consistency constants
		*/
		Php::Class<Consistency> consistency("Casscoon\\Consistency");

		consistency.property("ANY", CQL_CONSISTENCY_ANY, Php::Const);
		consistency.property("ONE", CQL_CONSISTENCY_ONE, Php::Const);
		consistency.property("TWO", CQL_CONSISTENCY_TWO, Php::Const);
		consistency.property("THREE", CQL_CONSISTENCY_THREE, Php::Const);
		consistency.property("QUORUM", CQL_CONSISTENCY_QUORUM, Php::Const);
		consistency.property("ALL", CQL_CONSISTENCY_ALL, Php::Const);
		consistency.property("LOCAL_QUORUM", CQL_CONSISTENCY_LOCAL_QUORUM, Php::Const);
		consistency.property("EACH_QUORUM", CQL_CONSISTENCY_EACH_QUORUM, Php::Const);
		consistency.property("SERIAL", CQL_CONSISTENCY_SERIAL, Php::Const);
		consistency.property("LOCAL_SERIAL", CQL_CONSISTENCY_LOCAL_SERIAL, Php::Const);
		consistency.property("LOCAL_ONE", CQL_CONSISTENCY_LOCAL_ONE, Php::Const);

		extension.add(std::move(consistency));

		/**
		* Add the Data Types
		*/
		Php::Class<Data> dataType("Casscoon\\DataType", Php::Final);

		dataType.property("ASCII", DATA_TYPE_ASCII, Php::Const);
		dataType.property("BIGINT", DATA_TYPE_BIGINT, Php::Const);
		dataType.property("BLOB", DATA_TYPE_BLOB, Php::Const);
		dataType.property("BOOLEAN", DATA_TYPE_BOOLEAN, Php::Const);
		dataType.property("COUNTER", DATA_TYPE_COUNTER, Php::Const);
		dataType.property("DECIMAL", DATA_TYPE_DECIMAL, Php::Const);
		dataType.property("DOUBLE", DATA_TYPE_DOUBLE, Php::Const);
		dataType.property("FLOAT", DATA_TYPE_FLOAT, Php::Const);
		dataType.property("INET", DATA_TYPE_INET, Php::Const);
		dataType.property("INTEGER", DATA_TYPE_INTEGER, Php::Const);
		dataType.property("INT", DATA_TYPE_INTEGER, Php::Const);
		dataType.property("LIST", DATA_TYPE_LIST, Php::Const);
		dataType.property("MAP", DATA_TYPE_MAP, Php::Const);
		dataType.property("SET", DATA_TYPE_SET, Php::Const);
		dataType.property("TEXT", DATA_TYPE_TEXT, Php::Const);
		dataType.property("STRING", DATA_TYPE_TEXT, Php::Const);
		dataType.property("TIMESTAMP", DATA_TYPE_TIMESTAMP, Php::Const);
		dataType.property("UUID", DATA_TYPE_UUID, Php::Const);
		dataType.property("TIMEUUID", DATA_TYPE_TIMEUUID, Php::Const);
		dataType.property("VARCHAR", DATA_TYPE_VARCHAR, Php::Const);
		dataType.property("VARINT", DATA_TYPE_VARINT, Php::Const);

		extension.add(std::move(dataType));

		Php::Class<Data> data("Casscoon\\Data");
		
		data.method("make", &Data::make);
		data.method("makeMap", &Data::makeMap);
		data.method("makeSet", &Data::makeSet);
		data.method("makeList", &Data::makeList);

		extension.add(std::move(data));

		return extension;
	}
}
