#ifndef CPP_TOOL_KIT_FACTORY_ABSTRACT_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_ABSTRACT_CONTEXT_H_

#include <string>

namespace cpptoolkit {
namespace factory {

/// @brief Contains basic information about creation of managed object
class AContext {
 public:
  virtual ~AContext() noexcept = default;

  /// @brief Check is current dependency is valid
  /// @return Result of the checking
  virtual bool IsValid() noexcept = 0;

  /// @brief Get error description
  /// @return Error message
  virtual std::string Error() noexcept = 0;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_ABSTRACT_CONTEXT_H_
