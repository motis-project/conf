#include <iostream>

#include "conf/configuration.h"
#include "conf/date_time.h"
#include "conf/options_parser.h"
#include "conf/simple_config.h"
#include "conf/simple_config_param.h"

#include "boost/program_options.hpp"

#define HOST "listen.host"
#define PORT "listen.port"

namespace po = boost::program_options;

class listener_settings : public conf::configuration {
public:
  friend std::ostream& operator<<(std::ostream& out,
                                  listener_settings const& options);

  listener_settings(std::string default_host, std::string default_port)
      : host(default_host), port(default_port) {}

  virtual ~listener_settings() {}

  virtual boost::program_options::options_description desc() override {
    po::options_description desc("Listener Options");
    desc.add_options()(HOST, po::value<std::string>(&host)->default_value(host),
                       "host to listen on (e.g. 0.0.0.0 or 127.0.0.1)")(
        PORT, po::value<std::string>(&port)->default_value(port),
        "port to listen on (e.g. 8080)");
    return desc;
  }

  virtual void print(std::ostream& out) const override {
    out << "  " << HOST << ": " << host << "\n"
        << "  " << PORT << ": " << port;
  }

  std::string host, port;
};

std::ostream& operator<<(std::ostream& out, const listener_settings& options) {
  options.print(out);
  return out;
}

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

class database_settings : public conf::simple_config {
public:
  database_settings(
      std::string const& url = "localhost:5432", int timeout = 30,
      bool retry = false,
      isolation_level_t isolation_lvl = isolation_level_t::SERIALIZABLE,
      std::vector<std::string> flags = {})
      : simple_config("Database Options", "db") {
    string_param(url_, url, "url", "the Database URL");
    int_param(timeout_, timeout, "timeout", "connection timeout (in seconds)");
    bool_param(retry_, retry, "retry", "retry on connection loss");
    template_param(isolation_lvl_, isolation_lvl, "isolation_lvl",
                   "default isolation level for transactions");
    multitoken_param(flags_, flags, "flags", "forwarded flags");
  }

  std::string url_;
  int timeout_;
  bool retry_;
  isolation_level_t isolation_lvl_;
  std::vector<std::string> flags_;
};

class doomsday_settings : public conf::simple_config {
public:
  doomsday_settings(
      std::time_t const apocalypse = conf::parse_date_time("2012-12-21"))
      : simple_config("Doomsday Options", "doom"), apocalypse_{apocalypse} {
    time_t_param(apocalypse_, "apocalypse", "THE END IS NEAR!");
  }

  conf::holder<std::time_t> apocalypse_;
};

int main(int argc, char* argv[]) {
  listener_settings listener_opt("0.0.0.0", "8080");
  database_settings database_opt("db1.example.com");
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