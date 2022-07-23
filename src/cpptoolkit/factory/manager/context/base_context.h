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

#ifndef CPP_TOOL_KIT_FACTORY_BASE_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_BASE_CONTEXT_H_

#include "abstract_context.h"

namespace cpptoolkit {
namespace factory {

/// @brief Holds pointer to managed object or error description in fail case
/// @tparam T type of managed object
template <typename T>
class BaseContext : public AContext {
 public:
  /// @brief Create BaseContext
  /// @param is_valid context validation flag
  BaseContext(bool is_valid) noexcept
      : instance_ptr_(nullptr), is_valid_(is_valid){};

  virtual ~BaseContext() noexcept {};

  /// @brief Set pointer to managed object
  /// @param instance_ptr pointer to managed object
  void SetInstance(T* instance_ptr) noexcept;

  /// @brief Get pointer to managed object
  /// @return pointer to managed object
  T* GetInstance() noexcept;

  bool IsValid() noexcept override;
  std::string Error() noexcept override;

  // Ban RAII operations
  BaseContext(const BaseContext&) = delete;
  BaseContext(BaseContext&& other) = delete;
  BaseContext& operator=(BaseContext&& other) = delete;
  BaseContext& operator=(const BaseContext&) = delete;

 protected:
  T* instance_ptr_;
  bool is_valid_;      // has error or not
  std::string error_;  // error description
};

// Implementation

template <typename T>
void BaseContext<T>::SetInstance(T* instance_ptr) noexcept {
  instance_ptr_ = instance_ptr;
}

template <typename T>
T* BaseContext<T>::GetInstance() noexcept {
  return instance_ptr_;
}

template <typename T>
inline bool BaseContext<T>::IsValid() noexcept {
  return is_valid_;
}

template <typename T>
inline std::string BaseContext<T>::Error() noexcept {
  return error_;
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_BASE_CONTEXT_H_
