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

#ifndef CPP_TOOL_KIT_FACTORY_RESOLVER_H_
#define CPP_TOOL_KIT_FACTORY_RESOLVER_H_

#include <memory>
#include <string>

#include "context/base_context.h"
#include "context/dependency_container.h"

namespace cpptoolkit {
namespace factory {

class Core;

namespace engine {

template <typename T>
PtrHolder<BaseContext<T>> GetContext(
    cpptoolkit::factory::Core* core, const std::string& key) noexcept;

}  // namespace engine

/// Provides access to registered in Core objects
/// These objects will be used as dependencies
/// To get an instance of object call 'Get()' or 'Get(key)' function
/// All dependency instances will be deleted automatically after delete instance
/// of main object.
class Resolver {
 public:
  /// @brief Create instance of the helper
  /// @param core [in] pointer to the core_ with registered objects
  /// @param d_container [in] collector for save dependencies
  Resolver(Core* core, engine::DependencyContainer* d_container) noexcept
      : core_(core),
        d_container_(d_container),
        is_valid_dependency_context_(true){};

  /// @brief Get dependency, registered in the core_ with the key.
  /// @tparam T type of dependency object
  /// @param key [in] the key for access
  /// @return Pointer to dependency object or 'nullptr' in error case
  template <typename T>
  T* Get(const std::string& key = engine::DEFAULT_KEY) noexcept;

 private:
  Core* core_;
  engine::DependencyContainer* d_container_;
  bool is_valid_dependency_context_;
};

// implementation

template <typename T>
inline T* Resolver::Get(const std::string& key) noexcept {
  if (!is_valid_dependency_context_) {  // No sense to do something here,
    return nullptr;  // a dependency context already has error
  }

  engine::PtrHolder<engine::BaseContext<T>> dependency =
      engine::GetContext<T>(core_, key);

  // set error if the context is not valid
  is_valid_dependency_context_ = dependency->IsValid();
  T* instance = dependency->GetInstance();
  d_container_->Add(std::move(dependency));

  if (!is_valid_dependency_context_) {
    return nullptr;
  }

  return instance;
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_RESOLVER_H_
