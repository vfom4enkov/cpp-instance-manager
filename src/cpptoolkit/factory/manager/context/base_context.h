#ifndef CPP_TOOL_KIT_FACTORY_BASE_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_BASE_CONTEXT_H_

#include "abstract_context.h"

namespace cpptoolkit {
namespace factory {

/// @brief Holds pointer to managed object
/// @tparam T - Type of managed object
template <typename T>
class BaseContext : public AContext {
 public:
  /// @brief Create BaseContext
  /// @param is_valid - Context validation flag
  BaseContext(bool is_valid) noexcept
      : instance_ptr_(nullptr), is_valid_(is_valid){};

  virtual ~BaseContext() noexcept {};

  /// @brief Set pointer to managed object
  /// @param instance_ptr - Pointer to managed object
  void SetInstance(T* instance_ptr) noexcept;

  /// @brief Get pointer to managed object
  /// @return Pointer to managed object
  T* GetInstance() noexcept;

  bool IsValid() noexcept override;
  std::string Error() noexcept override;

 protected:
  T* instance_ptr_;
  bool is_valid_;      // has error or not
  std::string error_;  // error description
};

// Implementation

template <typename T>
void BaseContext<T>::SetInstance(T* instance_ptr) noexcept {
  instance_ptr_ = instance_ptr;
}

template <typename T>
T* BaseContext<T>::GetInstance() noexcept {
  return instance_ptr_;
}

template <typename T>
inline bool BaseContext<T>::IsValid() noexcept {
  return is_valid_;
}

template <typename T>
inline std::string BaseContext<T>::Error() noexcept {
  return error_;
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_BASE_CONTEXT_H_
