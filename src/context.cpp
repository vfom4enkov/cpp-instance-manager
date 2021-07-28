
#include "context.h"

namespace cpp_instance_manager {

template <class T>
Context<T>::Context(Context&& other) noexcept {
  core_ = other.core_;
  other.core_ = nullptr;

  instance_ptr_ = other.instance_ptr_;
  other.instance_ptr_ = nullptr;

  dependencies_ = other.dependencies_;
  other.dependencies_.clear();
}

template <class T>
Context<T>::~Context() noexcept {
  if (instance_ptr_ != nullptr)
	  delete instance_ptr_;
  
  if (core_ == nullptr)
	  return;

  for (uintptr_t dependency_uintptr : dependencies_) {
    core_->Collect(dependency_uintptr);
  }
}

template <class T>
inline T* Context<T>::InstancePtr() noexcept {
  return instance_ptr_;
}

}  // namespace cpp_instance_manager
