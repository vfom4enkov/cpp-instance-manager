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

#include <memory>

namespace cpptoolkit {
namespace factory {

///@brief Contains pointer to object in heap, provides only move operations
///@tparam T type of managed object
template <class T>
class UPtr {
 public:
  ///@brief Construct a new UPtr object
  ///@param inst [in] pointer to managed object
  UPtr(T *inst) noexcept : instance_ptr_(inst){};

  ///@brief Construct a new UPtr object
  ///@param other another instance of UPtr
  UPtr(UPtr<T> &&other) noexcept;

  ///@brief Destroy the UPtr object and delete managed object (if it is not
  /// nullptr)
  ~UPtr() noexcept;

  ///@brief Construct a new UPtr object
  ///@tparam N type of base object
  ///@param other another instance of UPtr
  template <class N, class = typename std::enable_if<
                         std::is_convertible<N *, T *>::value>::type>
  UPtr(UPtr<N> &&other) noexcept {
    instance_ptr_ = other.Relese();
  };

  ///@brief Assign on move operation
  ///@tparam N type of base object
  ///@param other another instance of UPtr
  template <class N, class = typename std::enable_if<
                         std::is_convertible<N *, T *>::value>::type>
  UPtr<T> &operator=(UPtr<N> &&other) noexcept {
    instance_ptr_ = other.Relese();
    return *this;
  };

  // Block copy and assign RAII operations
  UPtr(const UPtr<T> &other) = delete;
  UPtr<T> &operator=(const UPtr<T> &other) = delete;

  ///@brief Get access to public methods and fields of managed object
  ///@return Pointer to managed object
  T *operator->() const noexcept;

  ///@brief Get pointer to managed object
  ///@return Pointer to managed object
  T *Get() noexcept;

  ///@brief Reset pointer to managed object without deleting
  ///@return Pointer to managed object
  T *Relese() noexcept;

  ///@brief Delete managed object
  void Reset() noexcept;

 private:
  T *instance_ptr_;
};

/// @brief Create UPtr (RAII wrapper)
/// @tparam T - The type of object to create
/// @tparam ...Args - types of arguments for T
/// @param [in] ...args - arguments for T constructor
/// @return UPtr with instance
template <typename T, typename... Args>
inline UPtr<T> MakeUPtr(Args &&...args) noexcept {
  UPtr<T> uptr(new T(std::forward<Args>(args)...));
  return std::move(uptr);
}

template <class T>
UPtr<T>::~UPtr() noexcept {
  Reset();
}

template <class T>
UPtr<T>::UPtr(UPtr<T> &&other) noexcept {
  instance_ptr_ = other.Relese();
}

template <class T>
T *UPtr<T>::operator->() const noexcept {
  return instance_ptr_;
}

template <class T>
T *UPtr<T>::Get() noexcept {
  return instance_ptr_;
}

template <class T>
T *UPtr<T>::Relese() noexcept {
  T *inst = instance_ptr_;
  instance_ptr_ = nullptr;
  return inst;
}

template <class T>
void UPtr<T>::Reset() noexcept {
  if (instance_ptr_ != nullptr) {
    delete instance_ptr_;
    instance_ptr_ = nullptr;
  }
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_CONTEXT_H_
