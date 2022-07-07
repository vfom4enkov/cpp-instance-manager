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

#ifndef CPP_TOOL_KIT_FACTORY_BASE_INSTANCE_MANAGER_H_
#define CPP_TOOL_KIT_FACTORY_BASE_INSTANCE_MANAGER_H_

#include <functional>
#include <string>

#include "context/context.h"
#include "context/error_context.h"
#include "resolver.h"

namespace cpptoolkit {
namespace factory {

/// @brief Base class for all object managers
class AInstanceManager {
 public:
  virtual ~AInstanceManager() noexcept = default;

  /// @brief Unique key for current manager
  /// @return Key
  virtual const std::string& TypeKey() noexcept = 0;
};

/// @brief Base object for instance managers
/// @tparam T Type of managed object
template <typename T>
class BaseInstanceManager : public AInstanceManager {
 public:
  /// @brief Create BaseInstanceManager
  /// @param [in] class_name_key - Unique key for current manager
  /// @param [in] create - Function for create instance of managed object
  /// @param [in] core - Pointer to the core_ with registered objects
  BaseInstanceManager(const std::string class_name_key,
                      std::function<T*(Resolver&)>&& create,
                      Core* core) noexcept
      : class_name_key_(class_name_key),
        create_(std::move(create)),
        core_(core){};

  /// @brief Create instance of managed object and save it to the context
  /// @return Context with instance of managed object
  virtual UPtr<BaseContext<T>> Get() noexcept = 0;

  const std::string& TypeKey() noexcept override;

 protected:
  inline void Create(Context<T>* context) noexcept;

 private:
  inline void AddError(Context<T>* context, std::string& error) noexcept;

 protected:
  std::function<T*(Resolver&)> create_;
  Core* core_;
  std::string class_name_key_;
};

// Implementation

template <typename T>
inline const std::string& BaseInstanceManager<T>::TypeKey() noexcept {
  return class_name_key_;
}

template <typename T>
inline void BaseInstanceManager<T>::Create(Context<T>* context) noexcept {
  Resolver dependencyHelper(core_, context);

  try {
    T* instance_ptr = create_(dependencyHelper);
    context->SetInstance(instance_ptr);

    if (instance_ptr == nullptr && context->IsValid()) {
      std::string error = "Error on create instance: " + class_name_key_ +
                          " 'Create' returned nullptr";
      AddError(context, error);
      return;
    }
  } catch (std::exception& ex) {
    // TODO add this case to documentation
    // If the context contains other error, then do not save current
    // exception
    if (context->IsValid()) {
      std::string error = "Error on create instance: " + class_name_key_ +
                          " Error: " + ex.what();
      AddError(context, error);
    }
  } catch (...) {
    // TODO (VFomchenkov) add this case to documentation
    // The same behaviour for current case
    if (context->IsValid()) {
      std::string error =
          "Unknown error on create instance: " + class_name_key_;
      AddError(context, error);
    }
  }
}

template <typename T>
inline void BaseInstanceManager<T>::AddError(Context<T>* context,
                                             std::string& error) noexcept {
  UPtr<ErrorContext<T>> error_context = MakeUPtr<ErrorContext<T>>(error);
  context->Add(std::move(error_context));
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_BASE_INSTANCE_MANAGER_H_
