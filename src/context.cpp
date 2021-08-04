
#include "context.h"

namespace cpp_instance_manager {

void BaseContext::add_dependency(uintptr_t dependency) {
  dependencies_.emplace_back(dependency);
}

template <class T>
Context<T>::Context(Context&& other) noexcept {
  other.core_ = nullptr;
  other.instance_ptr_ = nullptr;

  dependencies_ = std::move(other.dependencies_);
}

template <class T>
Context<T>::~Context() noexcept {
  if (instance_ptr_ != nullptr) delete instance_ptr_;

  if (core_ == nullptr) return;

  for (uintptr_t dependency_uintptr : dependencies_) {
    core_->Collect(dependency_uintptr);
  }
}

template <class T>
inline T* Context<T>::get_instance_ptr() noexcept {
  return instance_ptr_;
}

template <class T>
void Context<T>::set_instance_ptr(T* instance_ptr) noexcept {
  instance_ptr_ = instance_ptr;
}

}  // namespace cpp_instance_manager
