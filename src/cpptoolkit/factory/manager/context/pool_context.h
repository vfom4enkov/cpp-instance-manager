#ifndef CPP_TOOL_KIT_FACTORY_POOL_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_POOL_CONTEXT_H_

#include "context.h"

namespace cpptoolkit {
namespace factory {

/// @brief Observer for return object back to the pool
class AbstractPoolInstancePutback {
 public:
  virtual ~AbstractPoolInstancePutback() noexcept = default;

  /// @brief Event notification, to put back object to the pool
  /// @param key - Index key of managed object
  virtual void Callback(uintptr_t key) noexcept = 0;
};

/// @brief Contains info about instance from the pool
/// @tparam T - Type of managed object
template <typename T>
class PoolContext : public BaseContext<T> {
 public:
  /// @brief Create context
  /// @param putback - Pointer to observer
  /// @param context - Pointer to context (with object instance and validation
  /// state)
  PoolContext(AbstractPoolInstancePutback* putback, Context<T>* ctx, uintptr_t key) noexcept
      : BaseContext<T>(true), putback_(putback), key_(key) {
    instance_ptr_ = ctx->GetInstance();
  };

  ~PoolContext() noexcept;

  // Ban RAII operations
  PoolContext(const PoolContext&) = delete;
  PoolContext(PoolContext&& other) = delete;
  PoolContext& operator=(PoolContext&& other) = delete;
  PoolContext& operator=(const PoolContext&) = delete;

 private:
  AbstractPoolInstancePutback* putback_;
  uintptr_t key_;
};

// Implementation

template <typename T>
PoolContext<T>::~PoolContext() noexcept {
  putback_->Callback(key_);
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_POOL_CONTEXT_H_
