#pragma once

#include <ostream>
#include <string>
#include <sstream>

#include "boost/program_options.hpp"

namespace conf {

class abstract_param {
public:
  abstract_param(std::string const& name, std::string const& desc)
      : name_(name), desc_(desc) {}
  virtual ~abstract_param() = default;

  virtual void append_description(
      boost::program_options::options_description_easy_init&) = 0;
  virtual void print(std::ostream&) const = 0;

  std::string name_;
  std::string desc_;
};

inline std::ostream& operator<<(std::ostream& out, abstract_param const& p) {
  p.print(out);
  return out;
}

template <typename T>
class template_param : public abstract_param {
public:
  template_param(T& mem, T const& default_value, std::string const& name,
                 std::string const& desc)
      : abstract_param(name, desc), val_(mem) {
    val_ = default_value;
  }
  virtual ~template_param() = default;

  virtual void append_description(
      boost::program_options::options_description_easy_init& builder) override {
    builder(name_.c_str(),
            boost::program_options::value<T>(&val_)->default_value(val_),
            desc_.c_str());
  }

  virtual void print(std::ostream& os) const override { os << val_; }

  T& val_;
};

template <typename T>
std::unique_ptr<abstract_param> make_param(T& mem, T const& default_value,
                                           std::string const& name,
                                           std::string const& desc) {
  return std::unique_ptr<abstract_param>(
      new template_param<T>(mem, default_value, name, desc));
}

}  // conf
