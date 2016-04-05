#pragma once

#include <ostream>
#include <string>
#include <memory>
#include <vector>

#include "conf/configuration.h"

namespace boost {
namespace program_options {
class options_description;
}
}

namespace conf {

class simple_config : public conf::configuration {
public:
  simple_config(std::string const name, std::string const prefix = "");
  virtual ~simple_config();

  simple_config(simple_config const&) = delete;
  simple_config& operator=(simple_config const&) = delete;
  simple_config(simple_config&&) = delete;
  simple_config& operator=(simple_config&&) = delete;

  virtual boost::program_options::options_description desc() override;
  virtual void print(std::ostream& out) const override;

  void int_var(int& mem, std::string const& name, std::string const& desc);
  void int_var(int& mem, int const& default_value, std::string const& name,
               std::string const& desc);

  void string_var(std::string& mem, std::string const& name,
                  std::string const& desc);
  void string_var(std::string& mem, std::string const& default_value,
                  std::string const& name, std::string const& desc);

  void bool_var(bool& mem, std::string const& name, std::string const& desc);
  void bool_var(bool& mem, bool const& default_value, std::string const& name,
                std::string const& desc);

  void multitoken_var(std::vector<std::string>& mem, std::string const& name,
                      std::string const& desc);
  void multitoken_var(std::vector<std::string>& mem,
                      std::vector<std::string> const& default_value,
                      std::string const& name, std::string const& desc);

  // std::vector<std::string>& multitoken_var(std::string const& name);

private:
  std::string prefixed(std::string const& name) const;

  std::string name_;
  std::string prefix_;

  struct impl;
  std::unique_ptr<impl> impl_;
};

}  // namespace conf
