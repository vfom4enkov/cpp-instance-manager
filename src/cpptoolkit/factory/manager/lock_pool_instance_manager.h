#ifndef CPP_TOOL_KIT_FACTORY_LOCK_POOL_INSTANCE_MANAGER_H_
#define CPP_TOOL_KIT_FACTORY_LOCK_POOL_INSTANCE_MANAGER_H_

#include <condition_variable>
#include <mutex>
#include <queue>
#include <unordered_map>

#include "base_instance_manager.h"
#include "context/pool_context.h"

namespace cpptoolkit {
namespace factory {

/// Locks the thread until an instance was put back to the pool (if it is empty)
/// @tparam T Type of managed object
template <typename T>
class LockPoolInstanceManager : public BaseInstanceManager<T>,
                                public AbstractPoolInstancePutback {
 public:
  /// @brief Create instance manager
  /// @param [in] class_name_key - Unique key for current manager
  /// @param [in] create - Function for create instance of managed object
  /// @param [in] core - Pointer to the core_ with registered objects
  /// @param [in] pool_size - Size of pool object
  LockPoolInstanceManager(std::string class_name_key,
                          std::function<T*(Resolver&)>&& create,
                          Core* core, uint32_t pool_size) noexcept
      : BaseInstanceManager(class_name_key, std::move(create), core),
        countdown_(pool_size),
        waiter_counter_(0){};

  virtual ~LockPoolInstanceManager() noexcept = default;

  std::unique_ptr<BaseContext<T>> Get() noexcept override;
  void Callback(uintptr_t key) noexcept override;

 private:
  uint32_t countdown_;       // counter of objects what will be created for the pool
  uint32_t waiter_counter_;  // size of waiting threads

  std::queue<uintptr_t> queue_;  // free objects in the pool
  std::unordered_map<uintptr_t, std::unique_ptr<Context<T>>> index_;  // all created pool objects

  std::condition_variable queue_cv_;
  std::mutex mutex_;
};

template <typename T>
inline std::unique_ptr<BaseContext<T>>
LockPoolInstanceManager<T>::Get() noexcept {
  std::unique_lock<std::mutex> locker(mutex_);

  if (countdown_ > 0 && queue_.empty()) {
    // create object for the pool
    std::unique_ptr<Context<T>> context = MakeUnique<Context<T>>();
    Create(context.get());

    if (!context->IsValid()) {
      return context;
    }

    uintptr_t context_key = reinterpret_cast<uintptr_t>(context.get());
    std::unique_ptr<PoolContext<T>> pool_ctx =
        MakeUnique<PoolContext<T>>(this, context.get(), context_key);
    index_.emplace(context_key, std::move(context));
    --countdown_;
    return pool_ctx;
  }

  if (queue_.empty()) {
    // need to wait when one instance will be free
    ++waiter_counter_;
    queue_cv_.wait(locker, [&]() { return !queue_.empty(); });
    --waiter_counter_;
  }

  uintptr_t key = queue_.front();
  queue_.pop();
  const auto it = index_.find(key);
  std::unique_ptr<Context<T>>& context = it->second;
  std::unique_ptr<PoolContext<T>> pool_context =
      MakeUnique<PoolContext<T>>(this, context.get(), key);
  return pool_context;
}

template <typename T>
inline void LockPoolInstanceManager<T>::Callback(uintptr_t key) noexcept {
  std::unique_lock<std::mutex> locker(mutex_);
  queue_.push(key);
  if (waiter_counter_ > 0) {
    queue_cv_.notify_one();
  }
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_LOCK_POOL_INSTANCE_MANAGER_H_
