#include "conf/date_time.h"

#include <ctime>
#include <regex>
#include <string>

#include "boost/algorithm/string.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

#include "conf/holder.h"

#ifdef _MSC_VER
#define gmt(a, b) gmtime_s(b, a)
#else
#define gmt(a, b) gmtime_r(a, b)
#endif

using namespace boost::posix_time;

namespace conf {

ptime parse_date(std::string const& date_str) {
  if (date_str.size() != 10) {
    throw std::runtime_error("unrecognized date format");
  }
  return ptime({static_cast<short unsigned>(std::stoi(date_str.substr(0, 4))),
                static_cast<short unsigned>(std::stoi(date_str.substr(5, 2))),
                static_cast<short unsigned>(std::stoi(date_str.substr(8, 2)))});
}

time_duration parse_time(std::string const& time_str) {
  if (time_str.size() == 0) {
    return hours(0);
  }

  if (time_str.size() < 6) {
    throw std::runtime_error("unrecognized time format");
  }

  auto time = hours(std::stoi(time_str.substr(1, 3))) +
              minutes(std::stoi(time_str.substr(4, 2)));

  if (time_str.size() == 9) {
    time += seconds(std::stoi(time_str.substr(7, 2)));
  }

  return time;
}

time_duration parse_offset(std::string const& offset_str) {
  static const std::map<std::string, time_duration> specials(
      {{"", hours(0)}, {"Z", hours(0)}, {"MEZ", hours(1)}, {"MESZ", hours(2)}});
  auto it = specials.find(offset_str);
  if (it != end(specials)) {
    return it->second;
  }

  static const std::regex regex{"^((?:\\+|-)[0-9]{2}):?([0-9]{2})?$"};

  std::smatch m;
  std::regex_search(offset_str, m, regex);
  if (m.empty()) {
    throw std::runtime_error("unrecognized offset format");
  }

  auto offset = hours(std::stoi(m[1]));
  if (m[2].length() > 0) {
    offset += minutes(std::stoi(m[2]));
  }
  return offset;
}

std::time_t parse_date_time(std::string const& str) {
  if (str.size() == 0) {
    return 0;
  }

  if (str == "NOW") {
    return std::time(nullptr);
  }

  if (boost::all(str, boost::is_digit())) {
    return std::stol(str);
  }

  constexpr auto re =
      "([0-9]{4}-[0-9]{2}-[0-9]{2})"  // group 1: date
      "(T[0-9]{2}:[0-9]{2}(?::[0-9]{2})?)?"  // group 2: time
      "(.+)?";  // group 3: offset
  static const std::regex regex{re};

  std::smatch m;
  std::regex_search(str, m, regex);
  if (m.empty()) {
    throw std::runtime_error("unrecognized input format");
  }

  auto t = parse_date(m[1]) + parse_time(m[2]) - parse_offset(m[3]);
  ptime epoch({1970, 1, 1});
  return (t - epoch).total_seconds();
}

std::istream& operator>>(std::istream& in, holder<std::time_t>& result) {
  std::string token;
  in >> token;
  result = parse_date_time(token);
  return in;
}

std::ostream& operator<<(std::ostream& out, holder<std::time_t> const& t) {
  auto time = static_cast<std::time_t>(t);
  char buf[sizeof "2011-10-08t07:07:09z-0430"];
  struct tm result;
  gmt(&time, &result);
  strftime(buf, sizeof buf, "%FT%TZ%z", &result);
  return out << buf;
}

}  // namespace conf
