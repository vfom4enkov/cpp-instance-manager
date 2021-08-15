#ifndef CPP_INSTANCE_MANAGER_PTR_HOLDER_H_
#define CPP_INSTANCE_MANAGER_PTR_HOLDER_H_

namespace cpp_instance_manager {

class Container;

/// @brief Contains pointer to object in heap, and destroy the object with Container
/// @tparam T - Type of managed object
template <class T>
class PtrHolder {
 public:
  /// @brief Create PtrHolder
  /// @param [in] container - Pointer to Container
  /// @param [in] ptr - Pointer to managed object
  /// @return 
  PtrHolder(Container* container, T* instance) noexcept
      : container_(container), instance_(instance){};

  /// @brief Move object constructor
  /// @param other Another instance
  PtrHolder(PtrHolder&& other) noexcept;

  ~PtrHolder() noexcept;

  /// @brief Get direct pointer to managed object
  /// @return Pointer to managed object
  T* get() const noexcept;

  /// @brief Overloaded access operator provides access to public fields and
  /// methods of managed object
  T* operator->() const;

  // Ban copy operations
  PtrHolder(PtrHolder& src) = delete;

  // Ban copy assignment operations
  void operator=(const PtrHolder<T>& other) = delete;

 private:
  Container* container_;
  T* instance_;
};

// implementation

template <class T>
PtrHolder<T>::PtrHolder(PtrHolder&& other) noexcept {
  container_ = other.container_;
  other.container_ = nullptr;
  instance_ = other.instance_;
  other.instance_ = nullptr;
}

template <class T>
PtrHolder<T>::~PtrHolder() noexcept {
  if (container_ == nullptr || instance_ == nullptr) return;

  container_->Collect(instance_);
}

template <class T>
T* PtrHolder<T>::get() const noexcept {
  return instance_;
}

template <class T>
T* PtrHolder<T>::operator->() const {
  return instance_;
}

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_PTR_HOLDER_H_
