#ifndef CPP_TOOL_KIT_FACTORY_UNIQUE_PTR_TOOL_H_
#define CPP_TOOL_KIT_FACTORY_UNIQUE_PTR_TOOL_H_

#include <memory>

namespace cpptoolkit {
namespace factory {

/// @brief Create unique_ptr (for compatibility with C++11)
/// @tparam T - The type of object to create
/// @tparam ...Args - types of arguments for T
/// @param [in] ...args - arguments for T constructor
/// @return unique_ptr with instance
template <typename T, typename... Args>
inline std::unique_ptr<T> MakeUnique(Args&&... args) noexcept {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_UNIQUE_PTR_TOOL_H_
