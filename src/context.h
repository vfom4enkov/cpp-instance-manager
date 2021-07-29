#ifndef CPP_INSTANCE_MANAGER_CONTEXT_H_
#define CPP_INSTANCE_MANAGER_CONTEXT_H_

#include <cstdint>
#include <vector>

#include "core.h"

namespace cpp_instance_manager {

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
  Context(Core* core, T* instance_ptr, std::vector<uintptr_t> dependencies) noexcept
      : core_(core), instance_ptr_(instance_ptr), dependencies_(dependencies){};

  /// @brief Move object constructor
  /// @param other Another instance
  Context(Context&& other) noexcept;

  ~Context() noexcept;

  /// @brief Get pointer to managed object
  /// @return Pointer to managed object
  T* get_instance_ptr() noexcept;

 private:
  Core* core_;
  std::vector<uintptr_t> dependencies_;
  T* instance_ptr_;
};

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_CONTEXT_H_
