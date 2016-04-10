#include "conf/simple_config.h"

#include <iostream>

#include "boost/program_options.hpp"

#include "conf/simple_config_param.h"

namespace po = boost::program_options;

namespace conf {
namespace detail {

class bool_param final : public template_param<bool> {
public:
  template <typename... Args>
  bool_param(Args&&... args)
      : template_param(std::forward<Args>(args)...) {}
  void print(std::ostream& o) const override { o << (val_ ? "true" : "false"); }
};

class multitoken_param final : public abstract_param {
public:
  multitoken_param(std::vector<std::string>& mem,
                   std::vector<std::string> const& default_value,
                   std::string const& name, std::string const& desc)
      : abstract_param(name, desc), val_(mem) {
    val_ = default_value;
  }

  void append_description(
      boost::program_options::options_description_easy_init& builder) override {
    builder(name_.c_str(),
            po::value<std::vector<std::string>>(&val_)->multitoken(),
            desc_.c_str());
  }

  void print(std::ostream& os) const override {
    if (val_.size() == 0) {
      os << "[empty]";
    } else {
      for (auto const& v : val_) {
        os << v << " ";
      }
    }
  }

  std::vector<std::string>& val_;
};

}  // namespace detail

simple_config::simple_config(std::string name, std::string prefix)
    : name_(std::move(name)), prefix_(std::move(prefix)) {}

simple_config::~simple_config() = default;

po::options_description simple_config::desc() {
  po::options_description desc(name_);
  auto builder = desc.add_options();
  for (auto& v : params_) {
    v->append_description(builder);
  }
  return desc;
}

void simple_config::print(std::ostream& out) const {
  for (auto& v : params_) {
    out << "  " << v->name_ << ": " << *v << "\n";
  }
}

bool simple_config::empty_config() const { return params_.size() == 0; }

void simple_config::int_param(int& mem, std::string const& name,
                              std::string const& desc) {
  int_param(mem, 0, name, desc);
}

void simple_config::int_param(int& mem, int const& default_value,
                              std::string const& name,
                              std::string const& desc) {
  template_param(mem, default_value, name, desc);
}

void simple_config::string_param(std::string& mem, std::string const& name,
                                 std::string const& desc) {
  string_param(mem, "", name, desc);
}

void simple_config::string_param(std::string& mem,
                                 std::string const& default_value,
                                 std::string const& name,
                                 std::string const& desc) {
  template_param(mem, default_value, name, desc);
}

void simple_config::bool_param(bool& mem, std::string const& name,
                               std::string const& desc) {
  bool_param(mem, false, name, desc);
}

void simple_config::bool_param(bool& mem, bool const& default_value,
                               std::string const& name,
                               std::string const& desc) {
  add_param(std::unique_ptr<abstract_param>(
      new detail::bool_param(mem, default_value, name, desc)));
}

void simple_config::multitoken_param(std::vector<std::string>& mem,
                                     std::string const& name,
                                     std::string const& desc) {
  multitoken_param(mem, {}, name, desc);
}

void simple_config::multitoken_param(
    std::vector<std::string>& mem,
    std::vector<std::string> const& default_value, std::string const& name,
    std::string const& desc) {
  add_param(std::unique_ptr<abstract_param>(
      new detail::multitoken_param(mem, default_value, name, desc)));
}

void simple_config::add_param(std::unique_ptr<abstract_param> param) {
  if (prefix_.size() > 0) {
    param->name_ = prefix_ + "." + param->name_;
  }
  params_.emplace_back(std::move(param));
}

}  // namespace conf
