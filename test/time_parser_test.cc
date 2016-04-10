#define CATCH_CONFIG_PREFIX_ALL
#include "./catch.hpp"

#include <ctime>

#include "conf/date_time.h"
#include "conf/holder.h"

namespace conf {

CATCH_TEST_CASE("conf", "date") {
  CATCH_SECTION("zero") {  //
    CATCH_CHECK(parse_date_time("") == 0);
  };

  CATCH_SECTION("unix timestamps") {
    CATCH_CHECK(parse_date_time("1459964657") == 1459964657);
    CATCH_CHECK(parse_date_time("1459977338") == 1459977338);
  };

  CATCH_SECTION("date only") {
    constexpr auto kMayUTC = 1463788800;
    CATCH_CHECK(parse_date_time("2016-05-21") == kMayUTC);
    CATCH_CHECK(parse_date_time("2016-05-21Z") == kMayUTC);
    CATCH_CHECK(parse_date_time("2016-05-21+00") == kMayUTC);

    constexpr auto kMayMESZ = 1463781600;
    CATCH_CHECK(parse_date_time("2016-05-21+02") == kMayMESZ);
    CATCH_CHECK(parse_date_time("2016-05-21+0200") == kMayMESZ);
    CATCH_CHECK(parse_date_time("2016-05-21+02:00") == kMayMESZ);
    CATCH_CHECK(parse_date_time("2016-05-21MESZ") == kMayMESZ);

    constexpr auto kJanUTC = 1453939200;
    CATCH_CHECK(parse_date_time("2016-01-28") == kJanUTC);

    constexpr auto kJanMEZ = 1453935600;
    CATCH_CHECK(parse_date_time("2016-01-28+0100") == kJanMEZ);

    constexpr auto kAprPST = 1460275200;
    CATCH_CHECK(parse_date_time("2016-04-10-0800") == kAprPST);
  };

  CATCH_SECTION("date and time") {
    constexpr auto kDecUTC = 1482621120;
    CATCH_CHECK(parse_date_time("2016-12-24T23:12") == kDecUTC);
    CATCH_CHECK(parse_date_time("2016-12-24T23:12Z") == kDecUTC);
    CATCH_CHECK(parse_date_time("2016-12-24T23:12+00") == kDecUTC);

    constexpr auto kDecMEZ = 1482617520;
    CATCH_CHECK(parse_date_time("2016-12-24T23:12MEZ") == kDecMEZ);
    CATCH_CHECK(parse_date_time("2016-12-24T23:12+01") == kDecMEZ);
    CATCH_CHECK(parse_date_time("2016-12-24T23:12+0100") == kDecMEZ);
    CATCH_CHECK(parse_date_time("2016-12-24T23:12+01:00") == kDecMEZ);

    constexpr auto kAugUTC = 1471091117;
    CATCH_CHECK(parse_date_time("2016-08-13T12:25:17") == kAugUTC);
    CATCH_CHECK(parse_date_time("2016-08-13T12:25:17Z") == kAugUTC);
    CATCH_CHECK(parse_date_time("2016-08-13T12:25:17+00") == kAugUTC);

    constexpr auto kAugMESZ = 1471083917;
    CATCH_CHECK(parse_date_time("2016-08-13T12:25:17MESZ") == kAugMESZ);
    CATCH_CHECK(parse_date_time("2016-08-13T12:25:17+02") == kAugMESZ);
    CATCH_CHECK(parse_date_time("2016-08-13T12:25:17+0200") == kAugMESZ);
    CATCH_CHECK(parse_date_time("2016-08-13T12:25:17+02:00") == kAugMESZ);
  };

  CATCH_SECTION("bs input") {
    CATCH_REQUIRE_THROWS(parse_date_time("foo"));
    CATCH_REQUIRE_THROWS(parse_date_time("123foo"));

    CATCH_REQUIRE_THROWS(parse_date_time("2015-42-12"));
    CATCH_REQUIRE_THROWS(parse_date_time("2015-12-42"));

    CATCH_REQUIRE_THROWS(parse_date_time("2015-12-12asfdafa"));
    CATCH_REQUIRE_THROWS(parse_date_time("2015-12-12+05asd"));
    CATCH_REQUIRE_THROWS(parse_date_time("2015-12-12-asd"));
    CATCH_REQUIRE_THROWS(parse_date_time("2015-12-12foo+12"));
  };
}

}  // namespace conf
