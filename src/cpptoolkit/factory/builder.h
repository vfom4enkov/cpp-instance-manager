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

#ifndef CPP_TOOL_KIT_FACTORY_BUILDER_H_
#define CPP_TOOL_KIT_FACTORY_BUILDER_H_

#include <functional>
#include <vector>

#include "core_extension.h"
#include "tool/build_item.h"

namespace cpptoolkit {
namespace factory {

/// @brief Helper for registration objects
class Builder {
 public:
  Builder() noexcept = default;
  ~Builder() noexcept = default;

  /// @brief Create fluent helper allows to set properties for the registered
  /// object
  /// @tparam T - Type of managed object
  /// @param [in] create - Function for create instance of managed object
  /// @return Reference to helper
  template <typename T>
  BuildItem<T>& Register(std::function<T*(Resolver&)>&& create) noexcept;

  /// @brief Create fluent helper allows to set properties for the registered
  /// object without dependencies
  /// @tparam T - Type of managed object
  /// @return Reference to helper
  template <typename T>
  BuildItem<T>& RegisterType() noexcept;

  /// @brief Create Core, if Core is nullptr check 'Error()'
  /// @return Core for produce objects
  std::unique_ptr<Core> Build() noexcept {
    if (items_.size() == 0) {
      error_ = "There are no object for registration, list is empty";
      return std::unique_ptr<Core>(nullptr);
    }

    std::unique_ptr<CoreExtension> uptr_core(new (std::nothrow) CoreExtension());

    CoreExtension* core = uptr_core.get();
    for (auto& item : items_) {
      if (!item->Build(core)) {
        error_ = item->Error();
        return std::unique_ptr<Core>(nullptr);
      }
    }

    items_.clear();

    return std::move(uptr_core);
  };

  /// @brief Date about last error
  /// @return Error description
  const std::string& Error() noexcept { return error_; };

 private:
  std::vector<UPtr<ABuildItem>> items_;
  std::string error_;
};

// Implementation

template <typename T>
inline BuildItem<T>& Builder::Register(
    std::function<T*(Resolver&)>&& create) noexcept {
  UPtr<BuildItem<T>> item = MakeUPtr<BuildItem<T>>(std::move(create));
  BuildItem<T>* ptr = item.Get();
  items_.push_back(std::move(item));
  return *ptr;
}

template <typename T>
inline BuildItem<T>& Builder::RegisterType() noexcept {
  std::function<T*(Resolver&)> create = [](Resolver& resolver) -> T* {
    return Create<T>();
  };
  return Register(std::move(create));
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_BUILDER_H_
