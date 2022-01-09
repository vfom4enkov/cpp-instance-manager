#ifndef CPP_TOOL_KIT_FACTORY_ERROR_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_ERROR_CONTEXT_H_

#include <string>

#include "base_context.h"

namespace cpptoolkit {
namespace factory {

/// @brief Contains error description
/// @tparam T - Type of managed object
template <typename T>
class ErrorContext : public BaseContext<T> {
 public:
  /// @brief Create error context
  /// @param error - error description
  ErrorContext(std::string error) noexcept : BaseContext<T>(false) {
    error_ = error;
  };

  // Ban RAII operations
  ErrorContext(const ErrorContext&) = delete;
  ErrorContext(ErrorContext&& other) = delete;
  ErrorContext& operator=(ErrorContext&& other) = delete;
  ErrorContext& operator=(const ErrorContext&) = delete;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_ERROR_CONTEXT_H_
