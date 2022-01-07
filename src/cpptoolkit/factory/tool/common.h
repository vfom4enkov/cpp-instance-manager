#ifndef CPP_TOOL_KIT_FACTORY_COMMON_H_
#define CPP_TOOL_KIT_FACTORY_COMMON_H_

#include <string>

namespace cpptoolkit {
namespace factory {

/// @brief Key for objects without key
static const std::string DEFAULT_KEY = "default";

/// @brief Create instance
/// @tparam T - The type of object to create
/// @tparam ...Args - types of arguments for T
/// @param [in] ...args - arguments for T constructor
/// @return pointer for T
template <typename T, typename... Args>
inline T* Create(Args&&... args) {
  return new T(std::forward<Args>(args)...);
}

/// @brief Generate unique key for managed object
/// @tparam T - Type of managed object
/// @param [in] key - Key value
/// @return Key for managed object
template <typename T>
inline std::string TypeKey(const std::string& key) noexcept {
  const std::string type_name = typeid(T).name();
  return type_name + "/" + key;
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_COMMON_H_
