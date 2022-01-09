#ifndef CPP_TOOL_KIT_FACTORY_WEAK_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_WEAK_CONTEXT_H_

#include "base_context.h"

namespace cpptoolkit {
namespace factory {

/// @brief Holds pointer to managed object but does not delete it at end of life time
/// @tparam T - Type of managed object
template <typename T>
class WeakContext : public BaseContext<T> {
 public:
  
  /// @brief Create instance
  /// @param context - Source context
  WeakContext(T* instance_ptr) noexcept;
  
  ~WeakContext() noexcept {};

  // Ban RAII operations
  WeakContext(const WeakContext&) = delete;
  WeakContext(WeakContext&& other) = delete;
  WeakContext& operator=(WeakContext&& other) = delete;
  WeakContext& operator=(const WeakContext&) = delete;
};

// Implementation

template <typename T>
WeakContext<T>::WeakContext(T* instance_ptr) noexcept
    : BaseContext<T>(true) {
  instance_ptr_ = instance_ptr;
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_WEAK_CONTEXT_H_
