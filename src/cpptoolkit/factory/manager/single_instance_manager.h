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

#ifndef CPP_TOOL_KIT_FACTORY_SINGLE_INSTANCE_MANAGER_H_
#define CPP_TOOL_KIT_FACTORY_SINGLE_INSTANCE_MANAGER_H_

#include <mutex>

#include "base_instance_manager.h"
#include "context/error_context.h"
#include "context/weak_context.h"

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
      : BaseInstanceManager<T>(class_name_key, std::move(create), core),
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
