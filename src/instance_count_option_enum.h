#ifndef CPP_INSTANCE_MANAGER_INSTANCE_COUNT_OPTION_ENUM_H_
#define CPP_INSTANCE_MANAGER_INSTANCE_COUNT_OPTION_ENUM_H_

namespace cpp_instance_manager {
/// @brief Number of copies produced objects
enum class InstanceCountOptionEnum {

  /// @brief Object will be created once and used many times
  kSingle = 0,

  /// @brief Object will be created every time on request
  kMultiple = 1
};
}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_INSTANCE_COUNT_OPTION_ENUM_H_