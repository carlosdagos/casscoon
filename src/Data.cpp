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

#include <cassandra.h>
#include <phpcpp.h>
#include <iostream>
#include "Data.h"
#include "Error.h" 

void Data::__construct(Php::Parameters &params)
{
  this->init(params[0].numericValue(), params[1]);
}

void Data::bind(CassStatement* statement, unsigned int index)
{
  switch(this->_type) {
    case DATA_TYPE_ASCII: {
      this->bindAscii(statement, index);
      break;
    }
    case DATA_TYPE_BLOB: {
      this->bindBlob(statement, index);
      break;
    }
    case DATA_TYPE_BOOLEAN: {
      this->bindBoolean(statement, index);
      break;
    }
    case DATA_TYPE_BIGINT: {
      this->bindBigInt(statement, index);
      break;
    }
    case DATA_TYPE_COUNTER: {
      this->bindCounter(statement, index);
      break;
    }
    case DATA_TYPE_DECIMAL: {
      this->bindDecimal(statement, index);
      break;
    }
    case DATA_TYPE_DOUBLE: {
      this->bindDouble(statement, index);
      break;
    }
    case DATA_TYPE_FLOAT: {
      this->bindFloat(statement, index);
      break;
    }
    case DATA_TYPE_INET: {
      this->bindInet(statement, index);
      break;
    }
    case DATA_TYPE_INTEGER: {
      this->bindInteger(statement, index);
      break;
    }
    case DATA_TYPE_LIST: {
      this->bindList(statement, index);
      break;
    }
    case DATA_TYPE_MAP: {
      this->bindMap(statement, index);
      break;
    }
    case DATA_TYPE_SET: {
      this->bindSet(statement, index);
      break;
    }
    case DATA_TYPE_TEXT: {
      this->bindText(statement, index);
      break;
    }
    case DATA_TYPE_TIMESTAMP: {
      this->bindTimestamp(statement, index);
      break;
    }
    case DATA_TYPE_UUID: {
      this->bindUUID(statement, index);
      break;
    }
    case DATA_TYPE_TIMEUUID: {
      this->bindTimeUUID(statement, index);
      break;
    }
    case DATA_TYPE_VARCHAR: {
      this->bindVarchar(statement, index);
      break;
    }
    case DATA_TYPE_VARINT: {
      this->bindVarint(statement, index);
      break;
    }
    default: {
      throw Php::Exception("Unrecognized data type");
    }
  }
}

void Data::bindAscii(CassStatement* statement, unsigned int index)
{
  CassString string = cass_string_init(this->_value.stringValue().c_str());
  cass_statement_bind_string(statement, index, string);
}

void Data::bindBlob(CassStatement* statement, unsigned int index)
{
  const unsigned char* data = (const unsigned char*) this->_value.rawValue();
  CassBytes bytes = cass_bytes_init(data, this->_value.size());
  cass_statement_bind_bytes(statement, index, bytes);
}

void Data::bindBigInt(CassStatement* statement, unsigned int index)
{
  cass_statement_bind_int64(statement, index, this->_value.numericValue());
}

void Data::bindBoolean(CassStatement* statement, unsigned int index)
{
  cass_statement_bind_bool(statement, index, this->_value.boolValue() ? cass_true : cass_false);
}

void Data::bindCounter(CassStatement* statement, unsigned int index)
{
  cass_statement_bind_int64(statement, index, this->_value.numericValue());
}

void Data::bindDecimal(CassStatement* statement, unsigned int index)
{
  // TODO: Fixed on 4 decimal spaces because wtf kind of data type is this?
  //  -- Carlos
  const unsigned char *bytes = (const unsigned char*) this->_value.rawValue();
  CassDecimal decimal = cass_decimal_init(4, cass_bytes_init(bytes, this->_value.size()));
  cass_statement_bind_decimal(statement, index, decimal);
}

void Data::bindDouble(CassStatement* statement, unsigned int index)
{
  cass_statement_bind_double(statement, index, this->_value.floatValue());
}

void Data::bindFloat(CassStatement* statement, unsigned int index)
{
  cass_statement_bind_float(statement, index, this->_value.floatValue());
}

void Data::bindInet(CassStatement* statement, unsigned int index)
{
  // cass_statement_bind_inet
}

void Data::bindInteger(CassStatement* statement, unsigned int index)
{
  cass_statement_bind_int32(statement, index, this->_value.numericValue());
}

void Data::bindList(CassStatement* statement, unsigned int index)
{
  int datatype = this->_value["datatype"];
  Php::Value listvalue = this->_value["list"];

  CassCollection *list = cass_collection_new(CASS_COLLECTION_TYPE_LIST, listvalue.size());

  for(unsigned int i = 0; i < listvalue.size(); i++) {
      Php::Value data = listvalue[i];      
      this->bindCollection(list, datatype, data);
  }

  Error::checkOK(cass_statement_bind_collection(statement, index, list));  
  cass_collection_free(list);
}

void Data::bindSet(CassStatement* statement, unsigned int index)
{
  int datatype = this->_value["datatype"];
  Php::Value setvalue = this->_value["list"];

  CassCollection *set = cass_collection_new(CASS_COLLECTION_TYPE_SET, setvalue.size());

  for(unsigned int i = 0; i < setvalue.size(); i++) {
      Php::Value data = setvalue[i];      
      this->bindCollection(set, datatype, data);
  }

  Error::checkOK(cass_statement_bind_collection(statement, index, set));  
  cass_collection_free(set);
}

void Data::bindMap(CassStatement* statement, unsigned int index)
{
  int keytype = this->_value["keytype"];
  int datatype = this->_value["datatype"];
  Php::Value mapvalue = this->_value["map"];

  CassCollection *map = cass_collection_new(CASS_COLLECTION_TYPE_MAP, mapvalue.size());

  auto m = mapvalue.mapValue();

  for(auto &i: m) {
      Php::Value key = i.first;
      Php::Value data = i.second;
      
      this->bindCollection(map, keytype, key);
      this->bindCollection(map, datatype, data);
  }

  Error::checkOK(cass_statement_bind_collection(statement, index, map));
  cass_collection_free(map);
}

void Data::bindText(CassStatement* statement, unsigned int index)
{
  CassString string = cass_string_init(this->_value.stringValue().c_str());
  cass_statement_bind_string(statement, index, string);
}

void Data::bindTimestamp(CassStatement* statement, unsigned int index)
{
  cass_statement_bind_int64(statement, index, this->_value.numericValue());
}

void Data::bindUUID(CassStatement* statement, unsigned int index)
{
  // cass_statement_bind_uuid
}

void Data::bindTimeUUID(CassStatement* statement, unsigned int index)
{
  // cass_statement_bind_uuid
}

void Data::bindVarchar(CassStatement* statement, unsigned int index)
{
  CassString string = cass_string_init(this->_value.stringValue().c_str());
  cass_statement_bind_string(statement, index, string);
}

void Data::bindVarint(CassStatement* statement, unsigned int index)
{
  const unsigned char* data = (const unsigned char*) this->_value.rawValue();
  CassBytes bytes = cass_bytes_init(data, this->_value.size());
  cass_statement_bind_bytes(statement, index, bytes);
}

void Data::bindCollection(CassCollection* collection, int type, Php::Value data)
{
    switch(type) {
    case DATA_TYPE_VARCHAR:
    case DATA_TYPE_TEXT:
    case DATA_TYPE_ASCII: {
      CassString string = cass_string_init(data.stringValue().c_str());
      cass_collection_append_string(collection, string);
      break;
    }
    case DATA_TYPE_BOOLEAN: {
      cass_collection_append_bool(collection, data.boolValue() ? cass_true : cass_false);
      break;
    }
    case DATA_TYPE_DOUBLE: 
    case DATA_TYPE_FLOAT: {
      cass_collection_append_double(collection, data.floatValue());
      break;
    }
    case DATA_TYPE_TIMESTAMP:
    case DATA_TYPE_BIGINT:
    case DATA_TYPE_COUNTER: {
      cass_collection_append_int64(collection, data.numericValue());
      break;
    }
    case DATA_TYPE_DECIMAL: {
      // cass_collection_append_decimal (not done yet)
      break;
    }
    case DATA_TYPE_INET: {
      // cass_collection_append_inet (not done yet)
      break;
    }
    case DATA_TYPE_INTEGER: {
      cass_collection_append_int32(collection, data.numericValue());
      break;
    }
    case DATA_TYPE_UUID:
    case DATA_TYPE_TIMEUUID: {
      // cass_collection_append_uuid (not done yet)
      break;
    }
    case DATA_TYPE_BLOB: 
    case DATA_TYPE_VARINT: {
      const unsigned char* data = (const unsigned char*) this->_value.rawValue();
      CassBytes bytes = cass_bytes_init(data, this->_value.size());
      cass_collection_append_bytes(collection, bytes);
      break;
    }
    case DATA_TYPE_LIST:
    case DATA_TYPE_MAP:
    case DATA_TYPE_SET: {
      throw Php::Exception("Can't have multilevel complex types");
      break;
    }
    default: {
      throw Php::Exception("Unrecognized map data type");
    }
  }
}
