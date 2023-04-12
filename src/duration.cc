#include "conf/duration.h"

#include <sstream>
#include <stdexcept>

namespace conf {

duration parse_duration(std::string const& str) {
  std::stringstream stream;
  duration result;
  stream << str;
  stream >> result;
  return result;
}

std::istream& operator>>(std::istream& in, duration& result) {
  auto val = 0;
  std::string unit;
  in >> val;
  if (in.eof()) {
    return in;
  }
  in >> unit;
  if (in.fail() || unit.empty()) {
    throw std::runtime_error("unrecognized duration format");
  }
  if (unit.size() == 1) {
    switch (unit[0]) {
      case 'D':
        result.duration_ = std::chrono::hours{val * 24};
        result.unit_ = duration::unit::days;
        break;
      case 'h':
        result.duration_ = std::chrono::hours{val};
        result.unit_ = duration::unit::hours;
        break;
      case 'm':
        result.duration_ = std::chrono::minutes{val};
        result.unit_ = duration::unit::minutes;
        break;
      case 's':
        result.duration_ = std::chrono::seconds{val};
        result.unit_ = duration::unit::seconds;
        break;
      default: throw std::runtime_error("unrecognized duration unit");
    }
  } else {
    if (unit == "ms") {
      result.duration_ = std::chrono::milliseconds{val};
      result.unit_ = duration::unit::milliseconds;
    } else {
      throw std::runtime_error("unrecognized duration unit");
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, duration const& d) {
  switch (d.unit_) {
    case duration::unit::milliseconds:
      out << std::chrono::duration_cast<std::chrono::milliseconds>(d.duration_)
                 .count()
          << "ms";
      break;
    case duration::unit::seconds:
      out << std::chrono::duration_cast<std::chrono::seconds>(d.duration_)
                 .count()
          << "s";
      break;
    case duration::unit::minutes:
      out << std::chrono::duration_cast<std::chrono::minutes>(d.duration_)
                 .count()
          << "m";
      break;
    case duration::unit::hours:
      out << std::chrono::duration_cast<std::chrono::hours>(d.duration_).count()
          << "h";
      break;
    case duration::unit::days:
      out << (std::chrono::duration_cast<std::chrono::hours>(d.duration_)
                  .count() /
              24)
          << "D";
      break;
  }
  return out;
}

}  // namespace conf
