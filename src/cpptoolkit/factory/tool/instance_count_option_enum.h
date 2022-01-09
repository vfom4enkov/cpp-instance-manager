#ifndef CPP_TOOL_KIT_FACTORY_INSTANCE_COUNT_OPTION_ENUM_H_
#define CPP_TOOL_KIT_FACTORY_INSTANCE_COUNT_OPTION_ENUM_H_

namespace cpptoolkit {
namespace factory {

/// @brief Number of copies produced objects
enum class InstanceCountOptionEnum {

  /// @brief Object will be created every time on request
  kMultiple = 0,

  /// @brief Object will be created once and used many times
  kSingle = 1,

  /// @brief Take object from the pool or wait while one returns back
  kLockPool = 2,

  /// @brief Take object from the pool or create new one if pool is empty
  kSoftPool = 3
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_INSTANCE_COUNT_OPTION_ENUM_H_
