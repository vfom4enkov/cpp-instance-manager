#ifndef CPP_TOOL_KIT_FACTORY_SINGLE_INSTANCE_MANAGER_H_
#define CPP_TOOL_KIT_FACTORY_SINGLE_INSTANCE_MANAGER_H_

#include <mutex>

#include "base_instance_manager.h"
#include "error_context.h"
#include "weak_context.h"

namespace cpptoolkit {
namespace factory {

/// @brief Instance manager for single object
/// @tparam T Type of managed object
template <typename T>
class SingleInstanceManager : public BaseInstanceManager<T> {
 public:
  /// @brief Create SingleInstanceManager
  /// @param [in] class_name_key - Unique key for current manager
  /// @param [in] create - Function for create instance of managed object
  /// @param [in] core - Pointer to the core_ with registered objects
  SingleInstanceManager(std::string class_name_key,
                        std::function<T*(Resolver&)>&& create,
                        Core* core) noexcept
      : BaseInstanceManager(class_name_key, std::move(create), core),
        context_(nullptr){};

  virtual ~SingleInstanceManager() noexcept {};

  std::unique_ptr<BaseContext<T>> Get() noexcept override;

 private:
  std::unique_ptr<Context<T>> context_;
  std::mutex mutex_;
};

// Implementation

template <typename T>
inline std::unique_ptr<BaseContext<T>> SingleInstanceManager<T>::Get() noexcept {
  if (context_) {
    std::unique_ptr<WeakContext<T>> weak_context =
        MakeUnique<WeakContext<T>>(context_.get()->GetInstance());
    return weak_context;
  }

  std::unique_lock<std::mutex> locker(mutex_);
  // check the context second time,
  // for avoid the case when another thread, already created instance 
  // when current thread was locked
  if (context_) {
    std::unique_ptr<WeakContext<T>> weak_context =
        MakeUnique<WeakContext<T>>(context_.get()->GetInstance());
    return weak_context;
  }

  std::unique_ptr<Context<T>> context = MakeUnique<Context<T>>();
  Create(context.get());
  if (context->IsValid()) {
    context_ = std::move(context);
    std::unique_ptr<WeakContext<T>> weak_context =
        MakeUnique<WeakContext<T>>(context_.get()->GetInstance());
    return weak_context;
  } else {
    return context;
  }
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_SINGLE_INSTANCE_MANAGER_H_
