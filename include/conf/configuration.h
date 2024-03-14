#pragma once

#include <memory>
#include <ostream>
#include <string>

#include "boost/program_options/options_description.hpp"

namespace std {  // NOLINT(cert-dcl58-cpp)

template <typename T>
ostream& operator<<(ostream& out, vector<T> const& v) {
  for (auto i = 0U; i < v.size(); ++i) {
    if (i != 0) {
      out << ", ";
    }
    out << v[i];
  }
  return out;
}

}  // namespace std

namespace conf {

class configuration {
public:
  explicit configuration(std::string name, std::string prefix = "");
  virtual ~configuration();

  configuration(configuration&&);
  configuration& operator=(configuration&&);

  configuration(configuration const&) = delete;
  configuration& operator=(configuration const&) = delete;

  boost::program_options::options_description desc() {
    boost::program_options::options_description desc(name_);
    auto builder = desc.add_options();
    for (auto& v : params_) {
      v->append_description(builder);
    }
    return desc;
  }

  void print(std::ostream& out) const {
    for (auto const& v : params_) {
      out << "  " << v->name_ << ": ";
      v->print(out);
      out << "\n";
    }
  }

  bool empty() const { return params_.empty(); }

  friend std::ostream& operator<<(std::ostream& out,
                                  configuration const& conf) {
    conf.print(out);
    return out;
  }

  std::string const& prefix() const { return prefix_; }
  std::string const& name() const { return name_; }

protected:
  template <typename T>
  // NOLINTNEXTLINE(performance-unnecessary-value-param)
  void param(T& p, std::string name, std::string desc) {
    auto& new_param = params_.emplace_back(std::unique_ptr<parameter>(
        new template_param<T>(p, std::move(name), std::move(desc))));
    if (!prefix_.empty()) {
      new_param->name_ = prefix_ + "." + new_param->name_;
    }
  }

private:
  struct parameter {
    parameter(std::string&& name, std::string&& desc);
    parameter(parameter const&);
    parameter(parameter&&) noexcept;
    parameter& operator=(parameter const&);
    parameter& operator=(parameter&&) noexcept;
    virtual ~parameter();

    virtual void append_description(
        boost::program_options::options_description_easy_init&) = 0;
    virtual void print(std::ostream&) const = 0;

    std::string name_, desc_;
  };

  template <typename T>
  class template_param : public parameter {
    template <class T1>
    struct is_vector {
      static bool const value = false;
    };
    template <class T1>
    struct is_vector<std::vector<T1>> {
      static bool const value = true;
    };

  public:
    template_param(T& mem, std::string&& name, std::string&& desc)
        : parameter{std::move(name), std::move(desc)}, val_{mem} {}
    template_param(template_param const&) = default;
    template_param(template_param&&) noexcept = default;
    template_param& operator=(template_param const&) = default;
    template_param& operator=(template_param&&) noexcept = default;
    ~template_param() override = default;

    void append_description(
        boost::program_options::options_description_easy_init& builder)
        override {
      if constexpr (is_vector<T>::value) {
        builder(name_.c_str(),
                boost::program_options::value<T>(&val_)
                    ->default_value(val_)
                    ->multitoken(),
                desc_.c_str());
      } else {
        builder(name_.c_str(),
                boost::program_options::value<T>(&val_)->default_value(val_),
                desc_.c_str());
      }
    }

    void print(std::ostream& os) const override { os << val_; }

    T& val_;
  };

  std::string name_, prefix_;
  std::vector<std::unique_ptr<parameter>> params_;
};

}  // namespace conf
