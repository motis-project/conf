#pragma once

namespace conf {

template <typename T>
struct holder {
  holder() = default;
  holder(T val) : val_(std::move(val)) {}

  operator T&() { return val_; }
  operator T() const { return val_; }

  T val_;
};

}  // namespace conf
