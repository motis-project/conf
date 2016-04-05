#include "conf/options_parser.h"

#include <istream>
#include <fstream>

namespace po = boost::program_options;

namespace conf {

template <typename Parser>
void generic_read_command_line_args(Parser& parser,
                                    po::options_description& desc,
                                    po::variables_map& vm,
                                    std::vector<std::string>& unrecog,
                                    bool allow_unreg) {
  if (allow_unreg) {
    parser.allow_unregistered();
  }
  auto parsed = parser.options(desc).run();
  po::store(parsed, vm);
  po::notify(vm);

  auto unrecog_cmdl =
      po::collect_unrecognized(parsed.options, po::include_positional);
  unrecog.insert(unrecog.end(), unrecog_cmdl.begin(), unrecog_cmdl.end());
}

options_parser::options_parser(std::vector<configuration*> options)
    : desc_("General"), options_(std::move(options)) {
  configure_description();
}

void options_parser::configure_description() {
  // clang-format off
  desc_.add_options()
    ("help", "produce help message")
    ("options", "print options")
    ("version", "print version")
    ("config,c",
        po::value<std::string>(&file_)->default_value("config.ini"),
        "config path");
  // clang-format on

  for (auto& option : options_) {
    desc_.add(option->desc());
  }
}

void options_parser::read_command_line_args(int argc, char* argv[],
                                            bool allow_unreg) {
  auto p = po::command_line_parser(argc, argv);
  generic_read_command_line_args(p, desc_, vm_, unrecog_, allow_unreg);
}

void options_parser::read_command_line_args(
    std::vector<std::string> const& args, bool allow_unreg) {
  auto p = po::command_line_parser(args);
  generic_read_command_line_args(p, desc_, vm_, unrecog_, allow_unreg);
}

void options_parser::read_configuration_file(bool allow_unreg) {
  std::ifstream ifs(file_.c_str());
  if (ifs) {
    auto parsed_file_options = po::parse_config_file(ifs, desc_, allow_unreg);

    // Add unrecognized file options to the global unrecognized options.
    auto unrecog_file = po::collect_unrecognized(parsed_file_options.options,
                                                 po::include_positional);
    unrecog_.insert(unrecog_.end(), unrecog_file.begin(), unrecog_file.end());

    po::store(parsed_file_options, vm_);
    po::notify(vm_);
  }
}

bool options_parser::help() { return vm_.count("help") >= 1; }

bool options_parser::version() { return vm_.count("version") >= 1; }

bool options_parser::list_options() const { return vm_.count("options") >= 1; }

void options_parser::print_used(std::ostream& out) {
  out << "Used Options:\n\n";
  for (auto const& option : options_) {
    if (!option->empty_config()) {
      out << *option << "\n\n";
    }
  }
}

void options_parser::print_help(std::ostream& out) { out << desc_ << "\n"; }

void options_parser::print_unrecognized(std::ostream& out) {
  if (unrecog_.empty()) {
    return;
  }

  out << "Unrecognized Options:\n";
  for (auto& unrecog_option : unrecog_) {
    out << "  " << unrecog_option << "\n";
  }
  out << "\n";
}

void options_parser::print_options(std::ostream& out) const {
  std::vector<std::string> options;
  for (auto const& opt : desc_.options()) {
    out << opt->long_name() << ";" << opt->description();

    boost::any default_val;
    if (opt->semantic()->apply_default(default_val)) {
      if (typeid(std::string) == default_val.type()) {
        out << ";" << boost::any_cast<std::string>(default_val);
      } else if (typeid(bool) == default_val.type()) {
        out << ";" << boost::any_cast<bool>(default_val);
      } else if (typeid(int) == default_val.type()) {
        out << ";" << boost::any_cast<int>(default_val);
      } else if (typeid(unsigned) == default_val.type()) {
        out << ";" << boost::any_cast<unsigned>(default_val);
      } else {
        out << ";";
      }
    } else {
      out << ";";
    }

    out << "\n";
  }
}

}  // namespace conf
