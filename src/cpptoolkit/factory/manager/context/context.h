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

#ifndef CPP_TOOL_KIT_FACTORY_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_CONTEXT_H_

#include <vector>

#include "base_context.h"
#include "dependency_container.h"
#include "u_ptr.h"

namespace cpptoolkit {
namespace factory {

class Core;

/// @brief Contains pointer to managed object
/// @tparam T - Type of managed object
template <typename T>
class Context : public BaseContext<T>, public DependencyContainer {
  using BaseContext<T>::instance_ptr_;
  using BaseContext<T>::is_valid_;
  using BaseContext<T>::error_;

 public:
  /// @brief Create context
  Context() noexcept : BaseContext<T>(true){};

  ~Context() noexcept;

  void Add(UPtr<AContext>&& dependency) noexcept override;

  // Ban RAII operations
  Context(const Context&) = delete;
  Context(Context&& other) = delete;
  Context& operator=(Context&& other) = delete;
  Context& operator=(const Context&) = delete;

 private:
  std::vector<UPtr<AContext>> dependencies_;
};

// implementation

template <typename T>
Context<T>::~Context() noexcept {
  if (instance_ptr_ != nullptr) {
    delete instance_ptr_;
  }
}

template <typename T>
inline void Context<T>::Add(UPtr<AContext>&& dependency) noexcept {
  bool dependency_valid = dependency->IsValid();
  if (!dependency_valid) {
    is_valid_ = dependency_valid;
    error_ = dependency->Error();
  }

  dependencies_.push_back(std::move(dependency));
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_CONTEXT_H_
