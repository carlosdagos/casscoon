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

#include "Query.h"
#include "Connection.h"
#include "Error.h"
#include "Consistency.h"
#include "Data.h"
#include <sstream>
#include <cassandra.h>

Php::Value Query::disconnect()
{
	if (this->_result != nullptr) {
		cass_result_free(_result);
	}

	if (this->_iterator != nullptr) {
		cass_iterator_free(_iterator);
	}

	return true;
}

Php::Value Query::nr()
{
	this->_connection->log("debug", "Query::nr");
	return _rows;
}

Php::Value Query::fa()
{
	Php::Value assoc;
	this->first_output(assoc);
	return assoc;
}

Php::Value Query::fo()
{
	Php::Object object;
	this->first_output(object);

	if (object.mapValue().size() == 0) {
		return false;
	} else {
		return object;
	}
}

void Query::first_output(Php::Value &output)
{
	this->_connection->log("debug", "Query::first_output");

	if (_iterator == nullptr) {
		this->_connection->log("error", "No iterator, can't create first output");
		output = nullptr;
		return;
	}

	if (cass_iterator_next(this->_iterator)) {
		const CassRow* row = cass_iterator_get_row(this->_iterator);
		for (unsigned int i = 0; i < this->_column_count; i++) {
			CassString column_name  = cass_result_column_name(this->_result, i);
			CassValueType column_type = cass_result_column_type(this->_result, i);

			const CassValue* column_value = cass_row_get_column_by_name(row, column_name.data);

			Php::Value data;
			if (cass_value_is_null(column_value)) {
				data = nullptr;
			} else if (cass_value_is_collection(column_value)) {
				this->parse_collection(column_type, column_value, data);
			} else {
				this->parse_data(column_type, column_value, data);
			}

			output[column_name.data] = data;
		}
	} else {
		this->_connection->log("debug", "No more iterations left for iterator");
	}
}

void Query::parse_collection(CassValueType type, const CassValue* collectionvalue, Php::Value &data)
{
	switch (type) {
		case CASS_VALUE_TYPE_SET:
		case CASS_VALUE_TYPE_LIST: {
			CassIterator* items_iterator = cass_iterator_from_collection(collectionvalue);

			int i = 0;

			while (cass_iterator_next(items_iterator)) {
				const CassValue* item_value = cass_iterator_get_value(items_iterator);
				CassValueType item_type = cass_value_type(item_value);
				Php::Value value;
				this->parse_data(item_type, item_value, value);
				data[i] = value;
				i++;
			}

			cass_iterator_free(items_iterator);
			break;
		}
		case CASS_VALUE_TYPE_MAP: {
			CassIterator* items_iterator = cass_iterator_from_map(collectionvalue);

			while (cass_iterator_next(items_iterator)) {
				const CassValue* key_value = cass_iterator_get_map_key(items_iterator);
				CassValueType key_type = cass_value_type(key_value);
				const CassValue* data_value = cass_iterator_get_map_value(items_iterator);
				CassValueType data_type = cass_value_type(data_value);

				Php::Value keydata;
				Php::Value valuedata;
				this->parse_data(key_type, key_value, keydata);
				this->parse_data(data_type, data_value, valuedata);
				data[keydata] = valuedata;
			}

			cass_iterator_free(items_iterator);
			break;
		}
		default: {
			throw Php::Exception("Unrecognized column collection type");
		}
	}
}

void Query::parse_data(CassValueType type, const CassValue* value, Php::Value &data)
{
	switch (type) {
		case CASS_VALUE_TYPE_ASCII:
		case CASS_VALUE_TYPE_VARCHAR:
		case CASS_VALUE_TYPE_TEXT : {
			CassString s = cass_string_init("");
			cass_value_get_string(value, &s);
			data = s.data;
			break;
		}
		case CASS_VALUE_TYPE_FLOAT: {
			float output_float = 0;
			cass_value_get_float(value, &output_float);
			data = output_float;
			break;
		}
		case CASS_VALUE_TYPE_DOUBLE: {
			double output_double = 0;
			cass_value_get_double(value, &output_double);
			data = output_double;
			break;
		}
		case CASS_VALUE_TYPE_TIMESTAMP:
		case CASS_VALUE_TYPE_COUNTER:
		case CASS_VALUE_TYPE_BIGINT: {
			cass_int64_t output_bigint = 0;
			cass_value_get_int64(value, &output_bigint);
			data = output_bigint;
			break;
		}
		case CASS_VALUE_TYPE_INT: {
			cass_int32_t output_int = 0;
			cass_value_get_int32(value, &output_int);
			data = output_int;
			break;
		}
		case CASS_VALUE_TYPE_BOOLEAN: {
			cass_bool_t output_bool = cass_true;
			cass_value_get_bool(value, &output_bool);
			data = output_bool ? true : false;
			break;
		}
		case CASS_VALUE_TYPE_DECIMAL: {
			// cass_value_get_decimal (not sure yet);
			break;
		}
		case CASS_VALUE_TYPE_UUID:
		case CASS_VALUE_TYPE_TIMEUUID: {
			// cass_value_get_uuid (not sure yet);
			break;
		}
		case CASS_VALUE_TYPE_INET: {
			// cass_value_get_inet (not sure yet);
			break;
		}
		case CASS_VALUE_TYPE_BLOB:
		case CASS_VALUE_TYPE_VARINT: {
			// cass_value_get_bytes (not sure yet);
			break;
		}
		default: {
			throw Php::Exception("Unrecognized column value type");
		}
	}
}

Php::Value Query::setConsistency(Php::Parameters &params)
{
	int consistency = params[0];
	this->setConsistency(consistency);
	return true;
}

void Query::setConsistency(int consistency)
{
	if (!Consistency::isValid(consistency)) {
		throw Php::Exception("Invalid consistency value");
	}

	if (!Consistency::isConsistency(consistency)) {
		throw Php::Exception("Consistency value is not appropriate");
	}

	_consistency = Consistency::toCassConsistency(consistency);
}

Php::Value Query::setSerialConsistency(Php::Parameters &params)
{
	int serial_consistency = params[0];
	this->setSerialConsistency(serial_consistency);
	return true;
}

void Query::setSerialConsistency(int serial_consistency)
{
	if (!Consistency::isValid(serial_consistency)) {
		throw Php::Exception("Invalid consistency value");
	}

	if (!Consistency::isSerial(serial_consistency)) {
		throw Php::Exception("Invalid serial consistency value");
	}

	_serial_consistency = Consistency::toCassConsistency(serial_consistency);
}

Php::Value Query::prepare(Php::Parameters &params) 
{
	std::ostringstream out;
	out << "Query::prepare(" << this->_query_string.data << ", size: " << this->_param_amount << ")";

	this->_connection->log("debug", out.str());

	Php::Value prepare_values = params[0];
	unsigned int psize = prepare_values.size();
	if (psize != this->_param_amount) {
		throw Php::Exception("Wrong amount of ?'s and parameters to bind");
	}

	cass_statement_set_consistency(this->_statement, this->_consistency);
	cass_statement_set_serial_consistency(this->_statement, this->_serial_consistency);

	for (unsigned int i = 0; i < this->_param_amount; i++) {
		Php::Value prepare_value = prepare_values[i];
		if (!prepare_value.contains("type") || !prepare_value.contains("value")) {
			throw Php::Exception("Invalid data format supplied to prepare");
		}

		if (!prepare_value.get("type").isNumeric()) {
			throw Php::Exception("Invalid type supplied for data to prepare");
		}

		int type = prepare_value.get("type").numericValue();
		if (DATA_TYPE_IS_VALID(type)) {
			Data data = Data(prepare_value.get("type").numericValue(), prepare_value.get("value"));
			data.bind(this->_statement, i);
		} else {
			throw Php::Exception("Invalid data type supplied, out of data type range");
		}
	}

	return true;
}

Php::Value Query::exec()
{
	// Free the previous results in case there's multiple execs for the same object
	if (this->_result != nullptr) {
		cass_result_free(this->_result);
		this->_rows = 0;
		this->_column_count = 0;

		if (this->_iterator != nullptr) {
			cass_iterator_free(this->_iterator);
		}
	}

	// Go and fetch the results
	this->_result = this->_connection->execute(this->_statement);
	this->_rows = cass_result_row_count(this->_result);
	this->_column_count = cass_result_column_count(this->_result);

	// Only get the iterator if there's rows
	if (_rows > 0) {
		this->_iterator = cass_iterator_from_result(this->_result);
	}

	return this->_result != nullptr;
}
