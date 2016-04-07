#pragma once

#include <ostream>

namespace boost {
namespace program_options {
class options_description;
class variables_map;
}
}

namespace conf {

class configuration {
public:
  virtual ~configuration() {}
  virtual boost::program_options::options_description desc() = 0;
  virtual void print(std::ostream& out) const = 0;
  virtual bool empty_config() const { return false; }

  friend std::ostream& operator<<(std::ostream& out,
                                  configuration const& conf) {
    conf.print(out);
    return out;
  }
};

}  // namespace conf
