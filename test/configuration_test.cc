#include "catch.hpp"

#include <iostream>
#include <sstream>

#include "conf/configuration.h"
#include "conf/date_time.h"
#include "conf/options_parser.h"

struct my_conf : public conf::configuration {
  my_conf() : conf::configuration{"my_conf", "my"} {
    param(a_, "a", "this is the a parameter");
    param(b_, "b", "this is the b parameter");
    param(c_, "c", "this is the c parameter");
    param(d_, "d", "this is the d parameter");
    param(e_, "e", "this is the e parameter");
  }

  std::string a_;
  int b_{0U};
  char c_{'a'};
  conf::time d_{0U};
  std::vector<std::string> e_{{"a"}, {"b"}};
};

TEST_CASE("configuration", "simple") {
  char const* argv[] = {"",       "--my.a=test", "--my.b",
                        "3",      "--my.c=j",    "--my.d=2016-01-28+0100",
                        "--my.e", "x",           "y"};

  my_conf c;
  conf::options_parser parser{{&c}};
  parser.read_command_line_args(9, argv);

  std::stringstream ss;
  parser.print_help(ss);
  std::cout << ss.str() << "\n";

  ss.str("");
  parser.print_used(ss);
  std::cout << ss.str() << "\n";
}
