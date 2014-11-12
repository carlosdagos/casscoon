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

#ifndef BB_ERROR_H
#define BB_ERROR_H

#include <cassandra.h>
#include <phpcpp.h>

class Error : public Php::Base
{
public:
	static void checkFuture(CassFuture* future) {
		CassError error = cass_future_error_code(future);
		if (error != CASS_OK) {
			throw Php::Exception(cass_error_desc(error));
		}
	}

	static void checkOK(CassError error_state) {
		if (error_state != CASS_OK) {
			throw Php::Exception(cass_error_desc(error_state));
		}
	}
};

#endif
