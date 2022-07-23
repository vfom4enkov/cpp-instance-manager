/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2021, Vladimir Fomchenkov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CPP_TOOL_KIT_FACTORY_POOL_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_POOL_CONTEXT_H_

#include "context.h"

namespace cpptoolkit {
namespace factory {
namespace engine {

/// @brief Observer for return object back to the pool
class AbstractPoolInstancePutback {
 public:
  virtual ~AbstractPoolInstancePutback() noexcept = default;

  /// @brief Event notification, to put back object to the pool
  /// @param key [in] index key of managed object
  virtual void Callback(uintptr_t key) noexcept = 0;
};

/// @brief Contains info about instance from the pool
/// @tparam T type of managed object
template <typename T>
class PoolContext : public BaseContext<T> {
  using BaseContext<T>::instance_ptr_;

 public:
  /// @brief Create context
  /// @param putback [in] pointer to observer
  /// @param context [in] pointer to context (with object instance and validation
  /// state)
  PoolContext(AbstractPoolInstancePutback* putback, Context<T>* ctx,
              uintptr_t key) noexcept
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

}  // namespace engine
}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_POOL_CONTEXT_H_
