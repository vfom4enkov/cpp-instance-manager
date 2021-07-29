#include "dependency_helper.h"

namespace cpp_instance_manager {
template <typename N>
N* DependencyHelper::Get() noexcept {
  return Get<N>(DEFAULT_KEY);
}

template <typename N>
N* DependencyHelper::Get(std::string key) noexcept {
  if (has_error_) 
      return nullptr;

  N* item = core_->Get<N>(key);
  if (item == nullptr) {
    has_error_ = true;
    last_error_ = core_->LastError();
    return nullptr;
  }

  uintptr_t item_uintptr = reinterpret_cast<uintptr_t>(item);
  dependencies_.push_back(item_uintptr);
  return item;
}

}  // namespace cpp_instance_manager
