#include "./catch.hpp"

#include <ctime>

#include "conf/date_time.h"
#include "conf/holder.h"

namespace conf {

TEST_CASE("conf", "date") {
  SECTION("zero") {  //
    CHECK(parse_date_time("") == 0);
  };

  SECTION("now") {
    auto const now = time(nullptr);
    CHECK(parse_date_time("NOW") > now - 60);
    CHECK(parse_date_time("NOW") < now + 60);
  };

  SECTION("unix timestamps") {
    CHECK(parse_date_time("1459964657") == 1459964657);
    CHECK(parse_date_time("1459977338") == 1459977338);
  };

  SECTION("date only") {
    constexpr auto kMayUTC = 1463788800;
    CHECK(parse_date_time("2016-05-21") == kMayUTC);
    CHECK(parse_date_time("2016-05-21Z") == kMayUTC);
    CHECK(parse_date_time("2016-05-21+00") == kMayUTC);

    constexpr auto kMayMESZ = 1463781600;
    CHECK(parse_date_time("2016-05-21+02") == kMayMESZ);
    CHECK(parse_date_time("2016-05-21+0200") == kMayMESZ);
    CHECK(parse_date_time("2016-05-21+02:00") == kMayMESZ);
    CHECK(parse_date_time("2016-05-21MESZ") == kMayMESZ);

    constexpr auto kJanUTC = 1453939200;
    CHECK(parse_date_time("2016-01-28") == kJanUTC);

    constexpr auto kJanMEZ = 1453935600;
    CHECK(parse_date_time("2016-01-28+0100") == kJanMEZ);

    constexpr auto kAprPST = 1460275200;
    CHECK(parse_date_time("2016-04-10-0800") == kAprPST);
  };

  SECTION("date and time") {
    constexpr auto kDecUTC = 1482621120;
    CHECK(parse_date_time("2016-12-24T23:12") == kDecUTC);
    CHECK(parse_date_time("2016-12-24T23:12Z") == kDecUTC);
    CHECK(parse_date_time("2016-12-24T23:12+00") == kDecUTC);

    constexpr auto kDecMEZ = 1482617520;
    CHECK(parse_date_time("2016-12-24T23:12MEZ") == kDecMEZ);
    CHECK(parse_date_time("2016-12-24T23:12+01") == kDecMEZ);
    CHECK(parse_date_time("2016-12-24T23:12+0100") == kDecMEZ);
    CHECK(parse_date_time("2016-12-24T23:12+01:00") == kDecMEZ);

    constexpr auto kAugUTC = 1471091117;
    CHECK(parse_date_time("2016-08-13T12:25:17") == kAugUTC);
    CHECK(parse_date_time("2016-08-13T12:25:17Z") == kAugUTC);
    CHECK(parse_date_time("2016-08-13T12:25:17+00") == kAugUTC);

    constexpr auto kAugMESZ = 1471083917;
    CHECK(parse_date_time("2016-08-13T12:25:17MESZ") == kAugMESZ);
    CHECK(parse_date_time("2016-08-13T12:25:17+02") == kAugMESZ);
    CHECK(parse_date_time("2016-08-13T12:25:17+0200") == kAugMESZ);
    CHECK(parse_date_time("2016-08-13T12:25:17+02:00") == kAugMESZ);
  };

  SECTION("bs input") {
    REQUIRE_THROWS(parse_date_time("foo"));
    REQUIRE_THROWS(parse_date_time("123foo"));

    REQUIRE_THROWS(parse_date_time("2015-42-12"));
    REQUIRE_THROWS(parse_date_time("2015-12-42"));

    REQUIRE_THROWS(parse_date_time("2015-12-12asfdafa"));
    REQUIRE_THROWS(parse_date_time("2015-12-12+05asd"));
    REQUIRE_THROWS(parse_date_time("2015-12-12-asd"));
    REQUIRE_THROWS(parse_date_time("2015-12-12foo+12"));
  };
}

}  // namespace conf
