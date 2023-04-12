#pragma once

#include <chrono>
#include <istream>
#include <ostream>

namespace conf {

struct duration {
  enum class unit { milliseconds, seconds, minutes, hours, days };

  operator bool() const { return duration_.count() != 0; }

  std::chrono::milliseconds duration_{};
  unit unit_{unit::milliseconds};
};

duration parse_duration(std::string const&);

std::istream& operator>>(std::istream&, duration&);
std::ostream& operator<<(std::ostream&, duration const&);

}  // namespace conf
