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

#ifndef BB_CASS_DATA_H
#define BB_CASS_DATA_H

/*
From: http://www.datastax.com/documentation/cql/3.0/cql/cql_reference/cql_data_types_c.html

ascii     strings     US-ASCII character string
bigint    integers    64-bit signed long
blob      blobs       Arbitrary bytes (no validation), expressed as hexadecimal
boolean   booleans    true or false
counter   integers    Distributed counter value (64-bit long)
decimal   integers,   floats  Variable-precision decimal
double    integers    64-bit IEEE-754 floating point
float     integers,   floats  32-bit IEEE-754 floating point
inet      strings     IP address string in IPv4 or IPv6 format*
int       integers    32-bit signed integer
list      n/a         A collection of one or more ordered elements
map       n/a         A JSON-style array of literals: { literal : literal, literal : literal ... }
set       n/a         A collection of one or more elements
text      strings     UTF-8 encoded string
timestamp integers,   strings Date plus time, encoded as 8 bytes since epoch
uuid      uuids       A UUID in standard UUID format
timeuuid  uuids       Type 1 UUID only (CQL 3)
varchar   strings     UTF-8 encoded string
varint    integers    Arbitrary-precision integer
*/

#include <cassandra.h>
#include <phpcpp.h>

#define DATA_TYPE_ASCII     1
#define DATA_TYPE_BIGINT    2
#define DATA_TYPE_BLOB      3
#define DATA_TYPE_BOOLEAN   4
#define DATA_TYPE_COUNTER   5
#define DATA_TYPE_DECIMAL   6
#define DATA_TYPE_DOUBLE    7
#define DATA_TYPE_FLOAT     8
#define DATA_TYPE_INET      9
#define DATA_TYPE_INTEGER   10
#define DATA_TYPE_LIST      11
#define DATA_TYPE_MAP       12
#define DATA_TYPE_SET       13
#define DATA_TYPE_TEXT      14
#define DATA_TYPE_TIMESTAMP 15
#define DATA_TYPE_UUID      16
#define DATA_TYPE_TIMEUUID  17
#define DATA_TYPE_VARCHAR   18
#define DATA_TYPE_VARINT    19
#define DATA_TYPE_IS_VALID(type) (1 <= type && type <= 19)

class Data : public Php::Base
{
private:
  unsigned int _type = 0;
  Php::Value _value = nullptr;

public:
	Data() {}
	~Data() {}

  Data(unsigned int type, Php::Value value) {
    init(type, value);
  }

  void init(unsigned int type, Php::Value value) {
    this->_type = type;
    this->_value = value;
  }

  static Php::Value make(Php::Parameters &params) {
  	if (params.size() < 2) {
  		throw Php::Exception("Need to provide two parameters, one with value and one with corresponding data type");
  		return nullptr;
  	}

  	Php::Value assoc;
  	assoc["value"] = params[0];
  	assoc["type"] = params[1];

  	return assoc;
  }

  static Php::Value makeMap(Php::Parameters &params) {
    if (params.size() < 3) {
      throw Php::Exception("Need to provide three parameters, one with key type, one with value type, and one with corresponding data");
      return nullptr;
    }

    Php::Value data;
    data["keytype"]  = params[0];
    data["datatype"] = params[1];
    data["map"]      = params[2];

    Php::Value assoc;
    assoc["value"] = data;
    assoc["type"] = DATA_TYPE_MAP;

    return assoc;
  }

  static Php::Value makeList(Php::Parameters &params) { 
    return makeListOrSet(params, DATA_TYPE_LIST);
  } 

  static Php::Value makeSet(Php::Parameters &params) { 
    return makeListOrSet(params, DATA_TYPE_SET);
  }

  static Php::Value makeListOrSet(Php::Parameters &params, int type) {
    if (params.size() < 2) {
      throw Php::Exception("Need to provide two parameters, one with data type, and one with corresponding data");
      return nullptr;
    }

    Php::Value data;
    data["datatype"] = params[0];
    data["list"]     = params[1];

    Php::Value assoc;
    assoc["value"] = data;
    assoc["type"]  = type;

    return assoc;
  }

  void __construct(Php::Parameters &params);
  void bind(CassStatement* statement, unsigned int index);
  void bindAscii(CassStatement* statement, unsigned int index);
  void bindBlob(CassStatement* statement, unsigned int index);
  void bindBigInt(CassStatement* statement, unsigned int index);
  void bindBoolean(CassStatement* statement, unsigned int index);
  void bindCounter(CassStatement* statement, unsigned int index);
  void bindDecimal(CassStatement* statement, unsigned int index);
  void bindDouble(CassStatement* statement, unsigned int index);
  void bindFloat(CassStatement* statement, unsigned int index);
  void bindInet(CassStatement* statement, unsigned int index);
  void bindInteger(CassStatement* statement, unsigned int index);
  void bindList(CassStatement* statement, unsigned int index);
  void bindMap(CassStatement* statement, unsigned int index);
  void bindSet(CassStatement* statement, unsigned int index);
  void bindText(CassStatement* statement, unsigned int index);
  void bindTimestamp(CassStatement* statement, unsigned int index);
  void bindUUID(CassStatement* statement, unsigned int index);
  void bindTimeUUID(CassStatement* statement, unsigned int index);
  void bindVarchar(CassStatement* statement, unsigned int index);
  void bindVarint(CassStatement* statement, unsigned int index);

  void bindCollection(CassCollection* collection, int type, Php::Value data);
};

#endif
