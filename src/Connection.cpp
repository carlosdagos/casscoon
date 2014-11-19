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

#include <sstream>
#include "Connection.h"
#include "Error.h"
#include "Query.h"

Connection::Connection(std::string host_string, int64_t port)
{
	init(host_string, port);
}

void Connection::__construct(Php::Parameters &params)
{
	this->_options = params[0];

	Php::Value host_string = this->_options.get("hosts");
	Php::Value port = CASSANDRA_DEFAULT_PORT;

	if (this->_options.contains("port")) {
		port = this->_options.get("port");
	}

	if (!host_string.isString()) {
		throw Php::Exception("Host string parameter not a string");
	}

	if (!port.isNumeric()) {
		throw Php::Exception("Port is not a number");
	}

	if (this->_options.contains("logger")) {
		this->_logger = this->_options.get("logger");
	}

	// Call the c++ constructor with the provided php values
	init(host_string.stringValue(), port.numericValue());
}

void Connection::init(std::string host_string, int64_t port)
{
	this->_host_string = host_string.c_str();
	this->_port = port;

	if (!this->open()) {
		throw Php::Exception("Can't open connection");
	}
}

bool Connection::open()
{
	if (_is_open) {
		return true;
	}

	CassCluster* cluster = cass_cluster_new();
	
	cass_cluster_set_protocol_version(cluster, 2);
	cass_cluster_set_contact_points(cluster, _host_string);
	cass_cluster_set_port(cluster, _port);

	CassFuture* future = cass_cluster_connect(cluster);

	_session = cass_future_get_session(future);
	_is_open = cass_future_error_code(future) == CASS_OK && _session != nullptr;

	cass_future_free(future);
	cass_cluster_free(cluster);

	return _is_open;
}

void Connection::close()
{
	this->_logger.call("debug", "Connection::closing");

	if (_is_open) {
		CassFuture* future = cass_session_close(this->_session);
		if (cass_future_error_code(future) == CASS_OK) {
			_is_open = false;
		} else {
			throw Php::Exception("There was an error when closing connection");
		}
	} else {
		throw Php::Exception("Can't close unopened connection");
	}

	this->_logger.call("debug", "Connection::closed");
}

Php::Value Connection::query(Php::Parameters &params)
{
	std::ostringstream out;
	out << params[0].stringValue();
	std::string str = out.str();
	
	this->log("debug", "Connection::query Creating query");
	this->log("debug", str);

	Query* query = new Query(str, this);
	return Php::Object("Casscoon\\Query", query);
}

Php::Value Connection::batch()
{
	return 0;
}

const CassResult* Connection::execute(CassStatement* statement)
{
	this->log("debug", "Connection::execute");

	CassFuture* result = cass_session_execute(this->_session, statement);
	CassError error = cass_future_error_code(result);

	if (error != CASS_OK) {		
		CassString message = cass_future_error_message(result);
		cass_future_free(result);

		std::ostringstream out;
		out << cass_error_desc(error) << ": " << message.data;

		this->log("error", out.str());
		throw Php::Exception(out.str());
		return nullptr;
	}

	const CassResult* res = cass_future_get_result(result);

	if (res == nullptr) {
		throw Php::Exception("Error executing query, result is null");
	}

	cass_future_free(result);

	return res;
}

void Connection::log(std::string level, std::string message)
{
	if (this->_logger.size() > 0) {
		this->_logger.call(level.c_str(), message);
	}
}
