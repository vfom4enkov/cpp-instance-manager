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

#ifndef CPP_TOOL_KIT_FACTORY_U_PTR_H_
#define CPP_TOOL_KIT_FACTORY_U_PTR_H_

#include "manager/context/base_context.h"
#include "manager/context/ptr_holder.h"

namespace cpptoolkit {
namespace factory {

/// @brief RAII wrapper for managed object with move semantic
/// @tparam T type of managed object
template <class T>
class UPtr {
 public:
  /// @brief Construct a new UPtr object
  /// @param ptrHolder PtrHolder with Context of managed object
  UPtr(engine::PtrHolder<engine::BaseContext<T>>&& ptrHolder) noexcept
      : instance_(ptrHolder.Get()->GetInstance()),
        context_(std::move(ptrHolder)){};

  /// @brief Construct a new UPtr object
  /// @tparam N type of inherited object
  /// @param other another instance of UPtr
  template <class N, class = typename std::enable_if<
                         std::is_convertible<N*, T*>::value>::type>
  UPtr(UPtr<N>&& other) noexcept
      : instance_(other.instance_), context_(std::move(other.context_)) {
    other.instance_ = nullptr;
  };

  // Block default constructor, copy and assign RAII operations
  UPtr() = delete;
  UPtr(const UPtr<T>& other) = delete;
  UPtr<T>& operator=(const UPtr<T>& other) = delete;

  /// @brief Reset and assign on move operation
  /// @tparam N type of inherided object
  /// @param other another instance of UPtr
  template <class N, class = typename std::enable_if<
                         std::is_convertible<N*, T*>::value>::type>
  UPtr<T>& operator=(UPtr<N>&& other) noexcept {
    Reset();
    instance_ = other.instance_;
    other.instance_ = nullptr;
    context_ = std::move(other.context_);
    return *this;
  };

  ///@brief Provides access to public methods and fields of managed object
  T* operator->() const noexcept;

  ///@brief Get direct pointer of managed object
  ///@return Pointer of managed object
  T* Get() const noexcept;

  /// @brief Delete managed object
  void Reset() noexcept;

  ///@brief Check if managed object was created properly
  ///@return Check result
  bool IsValid() noexcept;

  ///@brief Get description if object was created with error
  ///@return Error description
  std::string Error() const noexcept;

 private:
  T* instance_;
  engine::PtrHolder<engine::AContext> context_;
};

// Implementation

template <class T>
T* UPtr<T>::Get() const noexcept {
  return instance_;
}

template<class T>
void UPtr<T>::Reset() noexcept {
  context_.Reset();
  instance_ = nullptr;
}

template <class T>
bool UPtr<T>::IsValid() noexcept {
  if (context_.Get() == nullptr) {
    return false;
  }

  return context_->IsValid();
}

template <class T>
std::string UPtr<T>::Error() const noexcept {
  if (context_.Get() == nullptr) {
    return "Instance is not initialized";
  }

  return context_->Error();
}

template <class T>
T* UPtr<T>::operator->() const noexcept {
  return instance_;
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_U_PTR_H_
