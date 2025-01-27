![Unix Build](https://github.com/motis-project/conf/workflows/Unix%20Build/badge.svg)
![Windows Build](https://github.com/motis-project/conf/workflows/Windows%20Build/badge.svg)

Enables to have programs with modular configuration structs.

Different parts define modules that are then gathered and configured together in the main executable(s) of the project.


# Example

## Somewhere in the database module
```cpp
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
```


## Somewhere totally different
```cpp
struct doomsday_settings : public conf::configuration {
  doomsday_settings() : configuration("Doomsday Options", "doom") {
    param(apocalypse_, "apocalypse", "THE END IS NEAR!");
  }

  conf::holder<std::time_t> apocalypse_{conf::parse_date_time("2012-12-21")};
};
```


## Main
```cpp
int main(int argc, char const** argv) {
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

  // Initialize application and so on
}
```
