#ifndef CPP_TOOL_KIT_FACTORY_MULTIPLE_INSTANCE_MANAGER_H_
#define CPP_TOOL_KIT_FACTORY_MULTIPLE_INSTANCE_MANAGER_H_

#include "base_instance_manager.h"

namespace cpptoolkit {
namespace factory {

/// @brief Instance manager for multiple objects
/// @tparam T Type of managed object
template <typename T>
class MultipleInstanceManager : public BaseInstanceManager<T> {
 public:
  /// @brief Create MultipleInstanceManager
  /// @param [in] class_name_key - Unique key for current manager
  /// @param [in] create - Function for create instance of managed object
  /// @param [in] core - Pointer to the core_ with registered objects
  MultipleInstanceManager(std::string class_name_key,
                          std::function<T*(Resolver&)>&& create,
                          Core* core) noexcept
      : BaseInstanceManager(class_name_key, std::move(create), core){};

  virtual ~MultipleInstanceManager() noexcept {};

  std::unique_ptr<BaseContext<T>> Get() noexcept override;
};

// Implementation

template <typename T>
inline std::unique_ptr<BaseContext<T>> MultipleInstanceManager<T>::Get() noexcept {
  std::unique_ptr<Context<T>> context = MakeUnique<Context<T>>();
  Create(context.get());
  return context;
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_MULTIPLE_INSTANCE_MANAGER_H_
