#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "boost/program_options.hpp"

#include "conf/configuration.h"

namespace conf {

class options_parser {
public:
  explicit options_parser(std::vector<configuration*> options);

  void configure_description();
  void read_command_line_args(int argc, char const* argv[],
                              bool allow_unreg = true);
  void read_command_line_args(std::vector<std::string> const& args,
                              bool allow_unreg = true);
  void read_configuration_file(bool allow_unreg = true);
  void read_environment(std::string const& prefix);
  bool help();
  void print_help(std::ostream& out);
  bool version();
  void print_used(std::ostream& out);
  void print_unrecognized(std::ostream& out);
  bool list_options() const;
  void print_options(std::ostream& out) const;
  std::string file() const;

private:
  std::string file_, system_config_file_;
  boost::program_options::options_description desc_;
  std::vector<std::string> unrecog_;
  boost::program_options::variables_map vm_;
  std::vector<configuration*> options_;
};

}  // namespace conf
