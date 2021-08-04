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

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_CONTEXT_H_
