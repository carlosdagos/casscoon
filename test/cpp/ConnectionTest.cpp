#include "Connection.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Connection can be opened and closed", "[connection]" ) {
    Connection* connection = new Connection("127.0.0.1", 9042);

    SECTION("Can be opened") {
    	REQUIRE(connection->openConnection());
    }
}