#ifndef CPP_INSTANCE_MANAGER_ABSTRACT_INSTANCE_MANAGER_H_
#define CPP_INSTANCE_MANAGER_ABSTRACT_INSTANCE_MANAGER_H_

#include <cstdint>
#include <string>

#include "instance_count_option_enum.h"

namespace cpp_instance_manager {

/// @brief Base class for all object managers
class AbstractInstanceManager {
 public:
  virtual ~AbstractInstanceManager() = default;

  /// @brief Get option of number copies managed object
  /// @return The value of the option
  virtual InstanceCountOptionEnum CountOption() noexcept = 0;

  /// @brief Collect (delete) current object and dependencies
  /// @param pointer_value The value of the pointer
  virtual void Collect(uintptr_t pointer_value) noexcept = 0;

  /// @brief Get description for last error
  /// @return Error description
  virtual std::string LastError() noexcept = 0;
};

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_ABSTRACT_INSTANCE_MANAGER_H_
