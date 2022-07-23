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

#ifndef CPP_TOOL_KIT_FACTORY_CORE_H_
#define CPP_TOOL_KIT_FACTORY_CORE_H_

#include <functional>
#include <string>
#include <unordered_map>

#include "manager/base_instance_manager.h"
#include "tool/common.h"
#include "u_ptr.h"

namespace cpptoolkit {
namespace factory {

/// @brief Contains all data about registered classes
class Core {
 public:
  Core() = default;
  virtual ~Core() = default;

  /// @brief Get context with managed object
  /// @tparam T type of managed object
  /// @param key [in] unique key for a given object type
  /// @return unique_ptr with BaseContext instance of managed object
  template <typename T>
  engine::PtrHolder<engine::BaseContext<T>> GetContext(
      const std::string& key) noexcept;

  /// @brief Get instance of BL object in RAII wrapper
  /// @tparam T type of managed object
  /// @param key [in] unique key for a given object type
  /// @return Instance of BL object in UPtr wrapper
  template <typename T>
  UPtr<T> Get(const std::string& key = engine::DEFAULT_KEY) noexcept;

 protected:
  std::unordered_map<std::string, engine::PtrHolder<engine::AInstanceManager>>
      index_;
};

// Implementation

template <typename T>
inline engine::PtrHolder<engine::BaseContext<T>> Core::GetContext(
    const std::string& key) noexcept {
  std::string type_key = TypeKey<T>(key);
  const auto it = index_.find(type_key);
  if (it == index_.end()) {
    std::string error = "Type: " + type_key + " is not registered";
    engine::PtrHolder<engine::ErrorContext<T>> error_context =
        engine::MakePtrHolder<engine::ErrorContext<T>>(error);
    return error_context;
  }

  engine::BaseInstanceManager<T>* instance_manager =
      reinterpret_cast<engine::BaseInstanceManager<T>*>(it->second.Get());
  return instance_manager->Get();
}

template <typename T>
UPtr<T> Core::Get(const std::string& key) noexcept {
  engine::PtrHolder<engine::BaseContext<T>> ptr_holder = GetContext<T>(key);
  UPtr<T> uptr(std::move(ptr_holder));
  return std::move(uptr);
}

namespace engine {

/// @brief Function helper for access to Core (forward declaration)
/// @tparam T type of managed object
/// @param core [in] pointer to core
/// @param key [in] unique key for a given object type
/// @return unique_ptr with BaseContext instance of managed object
template <typename T>
inline PtrHolder<BaseContext<T>> GetContext(
    cpptoolkit::factory::Core* core, const std::string& key) noexcept {
  return core->GetContext<T>(key);
}

}  // namespace engine

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_CORE_H_
