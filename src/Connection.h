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

#ifndef BB_CONNECTION_H
#define BB_CONNECTION_H

#define CASSANDRA_DEFAULT_PORT 9042

#include <cassandra.h>
#include <phpcpp.h>

class Connection : public Php::Base
{
private:
	Php::Value _options;
	Php::Value _logger = null;
	CassSession* _session = nullptr;
	const char* _host_string = nullptr;
	int64_t _port = CASSANDRA_DEFAULT_PORT;
	bool _is_open = false;

public:
	/**
	 * Constructor
	 */
	Connection() {}
	Connection(std::string host_string, int64_t port = CASSANDRA_DEFAULT_PORT);
	Connection(std::string host_string, int64_t port = CASSANDRA_DEFAULT_PORT, char* username = nullptr, char* password = nullptr);

	/**
	 * Destructor
	 */
	virtual ~Connection() {
		if (_is_open) {
			this->close();
		}
	}

	void __construct(Php::Parameters &params);
	void init(std::string host_string, int64_t port);
	bool open();
	void close();
	void log(std::string level, std::string message);
	Php::Value query(Php::Parameters &params);
	Php::Value batch();
	const CassResult* execute(CassStatement* statement);
};

#endif
