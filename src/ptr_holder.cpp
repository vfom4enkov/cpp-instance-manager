
#include "ptr_holder.h"

namespace cpp_instance_manager {

template <class T>
PtrHolder<T>::PtrHolder(T* ptr) noexcept {
  instance_ = ptr;
}

template <class T>
PtrHolder<T>::PtrHolder(PtrHolder&& other) noexcept {
  instance_ = other.instance_;
  other.instance_ = nullptr;
}

template <class T>
PtrHolder<T>::~PtrHolder() noexcept {
  if (instance_ != nullptr) {
    delete instance_;
  }
}

template <class T>
T* PtrHolder<T>::Get() const noexcept {
  return instance_;
}

template <class T>
T* PtrHolder<T>::operator->() const {
  return instance_;
}

}  // namespace cpp_instance_manager
