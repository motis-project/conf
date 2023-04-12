#include "./catch.hpp"

#include "conf/duration.h"

namespace conf {

TEST_CASE("duration", "duration") {
  SECTION("zero") {  //
    CHECK(!parse_duration(""));
    CHECK(!parse_duration("0ms"));
    CHECK(!parse_duration("0s"));
    CHECK(!parse_duration("0m"));
    CHECK(!parse_duration("0h"));
    CHECK(!parse_duration("0D"));
  };

  SECTION("milliseconds") {
    CHECK(parse_duration("123ms") == duration{std::chrono::milliseconds{123},
                                              duration::unit::milliseconds});
  }

  SECTION("seconds") {
    CHECK(parse_duration("30s") ==
          duration{std::chrono::seconds{30}, duration::unit::seconds});
  }

  SECTION("minutes") {
    CHECK(parse_duration("5m") ==
          duration{std::chrono::minutes{5}, duration::unit::minutes});
  }

  SECTION("hours") {
    CHECK(parse_duration("12h") ==
          duration{std::chrono::hours{12}, duration::unit::hours});
  }

  SECTION("days") {
    CHECK(parse_duration("1D") ==
          duration{std::chrono::hours{24}, duration::unit::days});
    CHECK(parse_duration("2D") ==
          duration{std::chrono::hours{48}, duration::unit::days});
  }

  SECTION("bs input") {
    REQUIRE_THROWS(parse_duration("foo"));
    REQUIRE_THROWS(parse_duration("123foo"));
  };
}

}  // namespace conf
