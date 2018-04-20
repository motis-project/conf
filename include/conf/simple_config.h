#pragma once

#include <ctime>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "conf/configuration.h"

namespace boost {
namespace program_options {
class options_description;
}
}

namespace conf {
class abstract_param;

template <typename T>
std::unique_ptr<abstract_param> make_param(T& mem, T const& default_value,
                                           std::string const& name,
                                           std::string const& desc);

class simple_config : public conf::configuration {
public:
  simple_config(std::string const name, std::string const prefix = "");
  virtual ~simple_config();

  simple_config(simple_config const&) = delete;
  simple_config& operator=(simple_config const&) = delete;

  virtual boost::program_options::options_description desc() override;
  virtual void print(std::ostream& out) const override;
  virtual bool empty_config() const override;

  void int_param(int& mem, std::string const& name, std::string const& desc);
  void int_param(int& mem, int const& default_value, std::string const& name,
                 std::string const& desc);

  void time_t_param(std::time_t& mem, std::string const& name,
                    std::string const& desc);
  void time_t_param(std::time_t& mem, std::time_t const& default_value,
                    std::string const& name, std::string const& desc);

  void size_t_param(size_t& mem, std::string const& name,
                    std::string const& desc);
  void size_t_param(size_t& mem, size_t const& default_value,
                    std::string const& name, std::string const& desc);

  void string_param(std::string& mem, std::string const& name,
                    std::string const& desc);
  void string_param(std::string& mem, std::string const& default_value,
                    std::string const& name, std::string const& desc);

  void bool_param(bool& mem, std::string const& name, std::string const& desc);
  void bool_param(bool& mem, bool const& default_value, std::string const& name,
                  std::string const& desc);

  void multitoken_param(std::vector<std::string>& mem, std::string const& name,
                        std::string const& desc);
  void multitoken_param(std::vector<std::string>& mem,
                        std::vector<std::string> const& default_value,
                        std::string const& name, std::string const& desc);

  void add_param(std::unique_ptr<abstract_param> param);

  // requires #include "conf/simple_config_param.h"
  template <typename T>
  void template_param(T& mem, T const& default_value, std::string const& name,
                      std::string const& desc) {
    add_param(make_param(mem, default_value, name, desc));
  }

  template <typename T>
  void template_param(T& mem, std::string const& name,
                      std::string const& desc) {
    add_param(make_param(mem, T{}, name, desc));
  }

protected:
  std::string name_;
  std::string prefix_;

  std::vector<std::unique_ptr<abstract_param>> params_;
};

}  // namespace conf
