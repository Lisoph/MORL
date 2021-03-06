#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

template <typename T, typename... Args>
std::unique_ptr<T> MakeUnique(Args && ...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

void SleepMs(unsigned ms);

class ScopeGuard {
public:
  using Janitor = std::function<void()>;

  ScopeGuard(Janitor const &janitor)
    : mJanitor(janitor)
  {}

  ScopeGuard(ScopeGuard const &) = delete;

  ScopeGuard(ScopeGuard && other)
    : mJanitor(other.mJanitor) {
    other.mJanitor = []{};
  }

  ~ScopeGuard() {
    mJanitor();
  }
private:
  Janitor mJanitor;
};

template <typename T>
size_t TypeId() {
  return typeid(T).hash_code();
}