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

#ifndef BB_CONSISTENCY_H
#define BB_CONSISTENCY_H

#include <cassandra.h>
#include <phpcpp.h>


#define CQL_CONSISTENCY_ANY CASS_CONSISTENCY_ANY
#define CQL_CONSISTENCY_ONE CASS_CONSISTENCY_ONE
#define CQL_CONSISTENCY_TWO CASS_CONSISTENCY_TWO
#define CQL_CONSISTENCY_THREE CASS_CONSISTENCY_THREE
#define CQL_CONSISTENCY_QUORUM CASS_CONSISTENCY_QUORUM
#define CQL_CONSISTENCY_ALL CASS_CONSISTENCY_ALL
#define CQL_CONSISTENCY_LOCAL_QUORUM CASS_CONSISTENCY_LOCAL_QUORUM
#define CQL_CONSISTENCY_EACH_QUORUM CASS_CONSISTENCY_EACH_QUORUM
#define CQL_CONSISTENCY_SERIAL CASS_CONSISTENCY_SERIAL
#define CQL_CONSISTENCY_LOCAL_SERIAL CASS_CONSISTENCY_LOCAL_SERIAL
#define CQL_CONSISTENCY_LOCAL_ONE CASS_CONSISTENCY_LOCAL_ONE

class Consistency : public Php::Base
{
public:
	Consistency() {}
	~Consistency() {}

	static bool isValid(int consistency) {
		return
		  consistency == CQL_CONSISTENCY_ANY 		  ||
		  consistency == CQL_CONSISTENCY_ONE 		  ||
		  consistency == CQL_CONSISTENCY_TWO 		  ||
		  consistency == CQL_CONSISTENCY_THREE 		  ||
		  consistency == CQL_CONSISTENCY_QUORUM 	  ||
		  consistency == CQL_CONSISTENCY_ALL 		  ||
		  consistency == CQL_CONSISTENCY_LOCAL_QUORUM ||
		  consistency == CQL_CONSISTENCY_EACH_QUORUM  ||
  		  consistency == CQL_CONSISTENCY_LOCAL_ONE	  ||
		  consistency == CQL_CONSISTENCY_SERIAL 	  ||
		  consistency == CQL_CONSISTENCY_LOCAL_SERIAL;
	}

	static bool isConsistency(int consistency) {
		return
		  consistency == CQL_CONSISTENCY_ANY 		  ||
		  consistency == CQL_CONSISTENCY_ONE 		  ||
		  consistency == CQL_CONSISTENCY_TWO 		  ||
		  consistency == CQL_CONSISTENCY_THREE 		  ||
		  consistency == CQL_CONSISTENCY_QUORUM 	  ||
		  consistency == CQL_CONSISTENCY_ALL 		  ||
		  consistency == CQL_CONSISTENCY_LOCAL_QUORUM ||
		  consistency == CQL_CONSISTENCY_EACH_QUORUM  ||
  		  consistency == CQL_CONSISTENCY_LOCAL_ONE;
	}

	static bool isSerial(int consistency) {
		return
		  consistency == CQL_CONSISTENCY_SERIAL 	  ||
		  consistency == CQL_CONSISTENCY_LOCAL_SERIAL;
	}

	static CassConsistency toCassConsistency(int consistency) {
		switch (consistency) {
			  case CQL_CONSISTENCY_ANY: return CASS_CONSISTENCY_ANY;
			  case CQL_CONSISTENCY_ONE: return CASS_CONSISTENCY_ONE;
			  case CQL_CONSISTENCY_TWO: return CASS_CONSISTENCY_TWO;
			  case CQL_CONSISTENCY_THREE: return CASS_CONSISTENCY_THREE;
			  case CQL_CONSISTENCY_QUORUM: return CASS_CONSISTENCY_QUORUM;
			  case CQL_CONSISTENCY_ALL: return CASS_CONSISTENCY_ALL;
			  case CQL_CONSISTENCY_LOCAL_QUORUM: return CASS_CONSISTENCY_LOCAL_QUORUM;
			  case CQL_CONSISTENCY_EACH_QUORUM: return CASS_CONSISTENCY_EACH_QUORUM;
			  case CQL_CONSISTENCY_SERIAL: return CASS_CONSISTENCY_SERIAL;
			  case CQL_CONSISTENCY_LOCAL_SERIAL: return CASS_CONSISTENCY_LOCAL_SERIAL;
			  case CQL_CONSISTENCY_LOCAL_ONE: return CASS_CONSISTENCY_LOCAL_ONE;
		}

		throw Php::Exception("Invalid consistency value");
	}
};

#endif
