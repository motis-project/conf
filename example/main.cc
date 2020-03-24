#include <iostream>

#include "conf/configuration.h"
#include "conf/date_time.h"
#include "conf/options_parser.h"

#include "boost/program_options.hpp"

#define HOST "listen.host"
#define PORT "listen.port"

namespace po = boost::program_options;

struct listener_settings : public conf::configuration {
  listener_settings() : configuration("Listener Options", "listener") {
    param(host, "host", "Hostname");
    param(port, "port", "Port number");
  }

  std::string host{"0.0.0.0"}, port{"8080"};
};

enum class isolation_level_t {
  SERIALIZABLE,
  REPEATED_READS,
  READ_COMITTED,
  READ_UNCOMITTED
};

constexpr auto kIsolationLevelSerializable = "serialiable";
constexpr auto kIsolationLevelRepeatedReads = "repeated_reads";
constexpr auto kIsolationLevelReadComitted = "read_comitted";
constexpr auto kIsolationLevelReadUncomitted = "read_uncomitted";

std::istream& operator>>(std::istream& in, isolation_level_t& lvl) {
  std::string token;
  in >> token;

  if (token == kIsolationLevelSerializable) {
    lvl = isolation_level_t::SERIALIZABLE;
  } else if (token == kIsolationLevelRepeatedReads) {
    lvl = isolation_level_t::REPEATED_READS;
  } else if (token == kIsolationLevelReadComitted) {
    lvl = isolation_level_t::READ_COMITTED;
  } else if (token == kIsolationLevelReadUncomitted) {
    lvl = isolation_level_t::READ_UNCOMITTED;
  } else {
    throw std::runtime_error("unknown serialization level.");
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, isolation_level_t const& lvl) {
  switch (lvl) {
    case isolation_level_t::SERIALIZABLE:
      out << kIsolationLevelSerializable;
      break;
    case isolation_level_t::REPEATED_READS:
      out << kIsolationLevelRepeatedReads;
      break;
    case isolation_level_t::READ_COMITTED:
      out << kIsolationLevelReadComitted;
      break;
    case isolation_level_t::READ_UNCOMITTED:
      out << kIsolationLevelReadUncomitted;
      break;
    default: out << "unknown"; break;
  }
  return out;
}

struct database_settings : public conf::configuration {
  database_settings() : configuration("Database Options", "db") {
    param(url_, "url", "the Database URL");
    param(timeout_, "timeout", "connection timeout (in seconds)");
    param(retry_, "retry", "retry on connection loss");
    param(isolation_lvl_, "isolation_lvl",
          "default isolation level for transactions");
    param(flags_, "flags", "forwarded flags");
  }

  std::string url_{"localhost:5432"};
  int timeout_{30};
  bool retry_{false};
  isolation_level_t isolation_lvl_{isolation_level_t::SERIALIZABLE};
  std::vector<std::string> flags_;
};

struct doomsday_settings : public conf::configuration {
  doomsday_settings() : configuration("Doomsday Options", "doom") {
    param(apocalypse_, "apocalypse", "THE END IS NEAR!");
  }

  conf::holder<std::time_t> apocalypse_{conf::parse_date_time("2012-12-21")};
};

int main(int argc, char const* argv[]) {
  listener_settings listener_opt;
  database_settings database_opt;
  doomsday_settings doomsday_opt;
  conf::options_parser parser({&listener_opt, &database_opt, &doomsday_opt});
  parser.read_command_line_args(argc, argv);

  if (parser.help()) {
    parser.print_help(std::cout);
    return 0;
  } else if (parser.version()) {
    return 0;
  }

  parser.read_configuration_file();

  parser.print_unrecognized(std::cout);
  parser.print_used(std::cout);

  std::cout << "Starting server to listen on " << listener_opt.host << ":"
            << listener_opt.port << "\n";

  std::cout << "Connecting to Database on " << database_opt.url_ << " (timeout "
            << database_opt.timeout_ << "s)\n";

  std::cout << "Apocalypse scheduled for: " << doomsday_opt.apocalypse_ << "\n";
}