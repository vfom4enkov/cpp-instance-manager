#ifndef CPP_INSTANCE_MANAGER_CONTEXT_H_
#define CPP_INSTANCE_MANAGER_CONTEXT_H_

#include <cstdint>
#include <vector>

namespace cpp_instance_manager {

class Core;

class BaseContext {
 public:
  BaseContext(){};
  void add_dependency(uintptr_t dependency);

 protected:
  std::vector<uintptr_t> dependencies_;
};

/// @brief Contains pointer to managed object and dependencies
/// @tparam T - Type of managed object
template <class T>
class Context : public BaseContext {
 public:
  /// @brief Create context
  /// @tparam T Type of managed object
  /// @param core Pointer to core
  /// @param instance_ptr Pointer to managed object
  /// @param dependencies Values of dependent objects pointers
  template <class T>
  Context(Core* core) noexcept
      : core_(core), instance_ptr_(nullptr) {};

  /// @brief Move object constructor
  /// @param other Another instance
  Context(Context&& other) noexcept
      : core_(other.core_), instance_ptr_(other.instance_ptr_);

  ~Context() noexcept;

  /// @brief Get pointer to managed object
  /// @return Pointer to managed object
  T* get_instance_ptr() noexcept;

  /// @brief Set pointer to managed object
  /// @param instance_ptr Pointer to managed object
  void set_instance_ptr(T* instance_ptr) noexcept;

  // Ban copy operations
  Context(const Context&) = delete;

  // Ban copy assignment operations
  Context& operator=(const Context&) = delete;

 private:
  Core* core_;
  T* instance_ptr_;
};

// implementation

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

#endif  // CPP_INSTANCE_MANAGER_CONTEXT_H_
