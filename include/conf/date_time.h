#pragma once

#include <ctime>
#include <istream>
#include <ostream>

#include "conf/holder.h"

namespace conf {

std::time_t parse_date_time(std::string const&);

std::istream& operator>>(std::istream&, holder<std::time_t>&);
std::ostream& operator<<(std::ostream&, holder<std::time_t> const&);

}  // namespace conf
