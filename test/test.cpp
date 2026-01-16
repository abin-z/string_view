#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "string_view/string_view.h"

TEST_CASE("string_view basic")
{
  abin::string_view sv = "hello";
  REQUIRE(sv.size() == 5);
}