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

#ifndef CPP_TOOL_KIT_FACTORY_WEAK_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_WEAK_CONTEXT_H_

#include "base_context.h"

namespace cpptoolkit {
namespace factory {
namespace engine {

/// @brief Holds pointer to managed object but does not delete it at end of life
/// time
/// @tparam T type of managed object
template <typename T>
class WeakContext : public BaseContext<T> {
  using BaseContext<T>::instance_ptr_;

 public:
  /// @brief Create instance
  /// @param context source context
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
WeakContext<T>::WeakContext(T* instance_ptr) noexcept : BaseContext<T>(true) {
  instance_ptr_ = instance_ptr;
}

}  // namespace engine
}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_WEAK_CONTEXT_H_
