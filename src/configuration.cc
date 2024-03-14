#include "conf/configuration.h"

namespace conf {

configuration::configuration(std::string name, std::string prefix)
    : name_{std::move(name)}, prefix_{std::move(prefix)} {}
configuration::~configuration() = default;

configuration::configuration(configuration&&) = default;
configuration& configuration::operator=(configuration&&) = default;

configuration::parameter::parameter(std::string&& name, std::string&& desc)
    : name_{std::move(name)}, desc_{std::move(desc)} {}
configuration::parameter::parameter(parameter const&) = default;
configuration::parameter::parameter(parameter&&) noexcept = default;
configuration::parameter& configuration::parameter::operator=(
    parameter const&) = default;
configuration::parameter& configuration::parameter::operator=(
    parameter&&) noexcept = default;
configuration::parameter::~parameter() = default;

}  // namespace conf