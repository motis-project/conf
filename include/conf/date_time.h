#pragma once

#include <ctime>
#include <istream>
#include <ostream>

namespace conf {

struct time {
  int64_t unix_time_{};
};

std::time_t parse_date_time(std::string const&);

std::istream& operator>>(std::istream&, time&);
std::ostream& operator<<(std::ostream&, time const&);

}  // namespace conf
