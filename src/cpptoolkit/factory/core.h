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
  PtrHolder<BaseContext<T>> Get(const std::string& key = DEFAULT_KEY) noexcept;

 protected:
  std::unordered_map<std::string, PtrHolder<AInstanceManager>> index_;
};

// Implementation

template <typename T>
inline PtrHolder<BaseContext<T>> Core::Get(const std::string& key) noexcept {
  std::string type_key = TypeKey<T>(key);
  const auto it = index_.find(type_key);
  if (it == index_.end()) {
    std::string error = "Type: " + type_key + " is not registered";
    PtrHolder<ErrorContext<T>> error_context = MakeUPtr<ErrorContext<T>>(error);
    return error_context;
  }

  BaseInstanceManager<T>* instance_manager =
      reinterpret_cast<BaseInstanceManager<T>*>(it->second.Get());
  return instance_manager->Get();
}

/// @brief Function helper for access to Core (forward declaration)
/// @tparam T type of managed object
/// @param core [in] pointer to core
/// @param key [in] unique key for a given object type
/// @return unique_ptr with BaseContext instance of managed object
template <typename T>
inline PtrHolder<BaseContext<T>> GetContext(Core* core,
                                       const std::string& key) noexcept {
  return core->Get<T>(key);
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_CORE_H_
