#include "conf/simple_config.h"

#include <iostream>

#include "boost/program_options.hpp"

namespace po = boost::program_options;

namespace conf {
namespace detail {
class var {
public:
  var(std::string const& name, std::string const& desc)
      : name_(name), desc_(desc) {}
  virtual ~var() {}

  virtual void append_description(po::options_description_easy_init&) = 0;
  virtual std::string value() = 0;

  std::string name_;
  std::string desc_;
};

class int_var final : public var {
public:
  int_var(int& mem, int const& default_value, std::string const& name,
          std::string const& desc)
      : var(name, desc), val_(mem) {
    val_ = default_value;
  }

  void append_description(po::options_description_easy_init& builder) override {
    builder(name_.c_str(), po::value<int>(&val_)->default_value(val_),
            desc_.c_str());
  }

  std::string value() override { return std::to_string(val_); }

  int& val_;
};

class string_var final : public var {
public:
  string_var(std::string& mem, std::string const default_value,
             std::string const& name, std::string const& desc)
      : var(name, desc), val_(mem) {
    val_ = default_value;
  }

  void append_description(po::options_description_easy_init& builder) override {
    builder(name_.c_str(), po::value<std::string>(&val_)->default_value(val_),
            desc_.c_str());
  }

  std::string value() override { return val_; }

  std::string& val_;
};

class bool_var final : public var {
public:
  bool_var(bool& mem, bool const& default_value, std::string const& name,
           std::string const desc)
      : var(name, desc), val_(mem) {
    val_ = default_value;
  }

  void append_description(po::options_description_easy_init& builder) override {
    builder(name_.c_str(), po::value<bool>(&val_)->default_value(val_),
            desc_.c_str());
  }

  std::string value() override { return val_ ? "true" : "false"; }

  bool& val_;
};

class multitoken_var final : public var {
public:
  multitoken_var(std::vector<std::string>& mem,
                 std::vector<std::string> const& default_value,
                 std::string const& name, std::string const desc)
      : var(name, desc), val_(mem) {
    val_ = default_value;
  }

  void append_description(po::options_description_easy_init& builder) override {
    builder(name_.c_str(),
            po::value<std::vector<std::string>>(&val_)->multitoken(),
            desc_.c_str());
  }

  std::string value() override {
    if (val_.size() == 0) {
      return "[empty]";
    }

    std::stringstream ss;
    for (auto const& v : val_) {
      ss << v << " ";
    }
    return ss.str();
  }

  std::vector<std::string> val_;
};
}  // namespace detail

struct simple_config::impl {
  std::vector<std::unique_ptr<detail::var>> vars;
};

simple_config::simple_config(std::string const name, std::string const prefix)
    : name_(std::move(name)),
      prefix_(std::move(prefix)),
      impl_(new simple_config::impl()) {}

simple_config::~simple_config() = default;

po::options_description simple_config::desc() {
  po::options_description desc(name_);
  auto builder = desc.add_options();
  for (auto& v : impl_->vars) {
    v->append_description(builder);
  }
  return desc;
}

void simple_config::print(std::ostream& out) const {
  for (auto& v : impl_->vars) {
    out << "  " << v->name_ << ": " << v->value() << "\n";
  }
}

void simple_config::int_var(int& mem, std::string const& name,
                            std::string const& desc) {
  return int_var(mem, 0, name, desc);
}

void simple_config::int_var(int& mem, int const& default_value,
                            std::string const& name, std::string const& desc) {
  auto v = new detail::int_var(mem, default_value, prefixed(name), desc);
  impl_->vars.push_back(std::unique_ptr<detail::var>(v));
}

void simple_config::string_var(std::string& mem, std::string const& name,
                               std::string const& desc) {
  return string_var(mem, "", name, desc);
}

void simple_config::string_var(std::string& mem,
                               std::string const& default_value,
                               std::string const& name,
                               std::string const& desc) {
  auto v = new detail::string_var(mem, default_value, prefixed(name), desc);
  impl_->vars.push_back(std::unique_ptr<detail::var>(v));
}

void simple_config::bool_var(bool& mem, std::string const& name,
                             std::string const& desc) {
  return bool_var(mem, false, name, desc);
}

void simple_config::bool_var(bool& mem, bool const& default_value,
                             std::string const& name, std::string const& desc) {
  auto v = new detail::bool_var(mem, default_value, prefixed(name), desc);
  impl_->vars.push_back(std::unique_ptr<detail::var>(v));
}

void simple_config::multitoken_var(std::vector<std::string>& mem,
                                   std::string const& name,
                                   std::string const& desc) {
  return multitoken_var(mem, {}, name, desc);
}

void simple_config::multitoken_var(
    std::vector<std::string>& mem,
    std::vector<std::string> const& default_value, std::string const& name,
    std::string const& desc) {
  auto v = new detail::multitoken_var(mem, default_value, prefixed(name), desc);
  impl_->vars.push_back(std::unique_ptr<detail::var>(v));
}

std::string simple_config::prefixed(std::string const& name) const {
  if (prefix_.size() > 0) {
    return prefix_ + "." + name;
  } else {
    return name;
  }
}

}  // namespace conf
