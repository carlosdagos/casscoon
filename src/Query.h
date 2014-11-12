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

#ifndef BB_QUERY_H
#define BB_QUERY_H

#include <iostream>
#include <algorithm>
#include <phpcpp.h>
#include "Connection.h"
#include "Consistency.h"

class Query : public Php::Base
{
private:
	Connection* _connection;
	CassString _query_string;
	unsigned int _param_amount = 0;
	unsigned int _column_count = 0;
	CassStatement* _statement = nullptr;
	CassConsistency _consistency = CQL_CONSISTENCY_ONE;
	CassConsistency _serial_consistency = CASS_CONSISTENCY_LOCAL_SERIAL;
	const CassResult* _result;
	CassIterator* _iterator = nullptr;
	int _rows = 0;

public:
	/**
	 * Constructor
	 */
	Query() {}
	Query(std::string query_string, Connection* connection) {
		if (!strcmp("", query_string.c_str())) {
			throw Php::Exception("Can't create empty query");
		}

		this->_param_amount = std::count(query_string.begin(), query_string.end(), '?');
		this->_query_string = cass_string_init(query_string.c_str());
		this->_connection   = connection;
		this->_statement    = cass_statement_new(this->_query_string, this->_param_amount);
	}

	/**
	 * Destructor
	 */
	virtual ~Query() {
		this->disconnect();
	}

	void __construct(Php::Parameters &params);
	Php::Value disconnect();
	Php::Value nr();
	Php::Value fa();
	Php::Value fo();
	void first_output(Php::Value &output);
	void parse_data(CassValueType type, const CassValue* value, Php::Value &data);
	void parse_collection(CassValueType type, const CassValue* value, Php::Value &data);
	Php::Value prepare(Php::Parameters &params);

	Php::Value exec();

	Php::Value setConsistency(Php::Parameters &params);
	void setConsistency(int consistency);

	Php::Value setSerialConsistency(Php::Parameters &params);
	void setSerialConsistency(int serial_consistency);
};

#endif
