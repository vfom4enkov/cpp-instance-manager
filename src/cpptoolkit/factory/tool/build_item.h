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

#ifndef CPP_TOOL_KIT_FACTORY_BUILD_ITEM_H_
#define CPP_TOOL_KIT_FACTORY_BUILD_ITEM_H_

#include <memory>
#include <vector>

#include "../manager/lock_pool_instance_manager.h"
#include "../manager/multiple_instance_manager.h"
#include "../manager/single_instance_manager.h"
#include "../manager/soft_pool_instance_manager.h"
#include "common.h"
#include "instance_count_option_enum.h"

namespace cpptoolkit {
namespace factory {

class CoreExtension;
class Resolver;

template <typename T, typename... Args>
std::unique_ptr<T> MakeUnique(Args&&... args) noexcept;

/// @brief Interface for collecting data for registration object in Core
class ABuildItem {
 public:
  virtual ~ABuildItem() noexcept = default;

  /// @brief Register type in Core, in false case check 'Error()'
  /// @param [in] core - Pointer to for type registration
  /// @return Operation result
  virtual bool Build(CoreExtension* core) noexcept = 0;

  /// @brief Get error description
  /// @return Error description
  virtual const std::string& Error() noexcept = 0;
};

/// @brief Provides fluent interface for type registation
/// @tparam T - Type of managed object
template <typename T>
class BuildItem : public ABuildItem {
 public:
  /// @brief Create instance
  /// @param [in] create - Function for create instance of managed object
  /// @tparam T - Type of managed object
  BuildItem(std::function<T*(Resolver&)>&& create) noexcept
      : create_(std::move(create)),
        count_option_(InstanceCountOptionEnum::kMultiple),
        key_(DEFAULT_KEY),
        pool_size_(0){};

  virtual ~BuildItem() noexcept = default;

  virtual bool Build(CoreExtension* core) noexcept override;

  virtual const std::string& Error() noexcept override;

  /// @brief Set unique key for a given object type
  /// @param [in] key - Key value
  /// @return Pointer to current instance
  BuildItem<T>& SetKey(const std::string& key) noexcept;

  // Count option section

  /// @brief Register as single instance
  /// @return Pointer to current instance
  BuildItem<T>& AsSingleInstance() noexcept;

  /// @brief Register as multiple instance
  /// @return Pointer to current instance
  BuildItem<T>& AsMultipleInstance() noexcept;

  /// @brief Register as lock pool instance
  /// @param [in] pool_size - Pool size
  /// @return Pointer to current instance
  BuildItem<T>& AsLockPoolInstance(uint32_t pool_size) noexcept;

  /// @brief Register as pool instance
  /// @param [in] pool_size - Pool size
  /// @return Pointer to current instance
  BuildItem<T>& AsPoolInstance(uint32_t pool_size) noexcept;

 private:
  std::function<T*(Resolver&)> create_;
  InstanceCountOptionEnum count_option_;
  std::string key_;
  uint32_t pool_size_;  // only for pool instances
  std::string error_;
};

// Implementation

template <typename T>
bool BuildItem<T>::Build(CoreExtension* core) noexcept {
  const std::string type_name = typeid(T).name();
  if (key_.empty()) {
    error_ = type_name + ": key can not be empty";
    return false;
  }

  std::string type_key = TypeKey<T>(key_);

  bool result = true;
  switch (count_option_) {
    case InstanceCountOptionEnum::kMultiple: {
      std::unique_ptr<MultipleInstanceManager<T>> m_manager =
          MakeUnique<MultipleInstanceManager<T>>(type_key, std::move(create_),
                                                 core);
      result = core->Add(std::move(m_manager));
      break;
    }
    case InstanceCountOptionEnum::kSingle: {
      std::unique_ptr<SingleInstanceManager<T>> s_manager =
          MakeUnique<SingleInstanceManager<T>>(type_key, std::move(create_),
                                               core);
      result = core->Add(std::move(s_manager));
      break;
    }
    case InstanceCountOptionEnum::kSoftPool: {
      if (pool_size_ == 0) {
        error_ = type_name + ": pool size can not be 0";
        return false;
      }
      std::unique_ptr<SoftPoolInstanceManager<T>> p_manager =
          MakeUnique<SoftPoolInstanceManager<T>>(type_key, std::move(create_), core,
                                             pool_size_);
      result = core->Add(std::move(p_manager));
      break;
    }
    case InstanceCountOptionEnum::kLockPool: {
      if (pool_size_ == 0) {
        error_ = type_name + ": pool size can not be 0";
        return false;
      }
      std::unique_ptr<LockPoolInstanceManager<T>> lp_manager =
          MakeUnique<LockPoolInstanceManager<T>>(type_key, std::move(create_),
                                                 core, pool_size_);
      result = core->Add(std::move(lp_manager));
      break;
    }
    default:
      int option = static_cast<int>(count_option_);
      error_ = "Unknown instance count option: " + option;
      return false;
  }

  if (!result) {
    error_ = core->LastError();
  }
  return result;
}

template <typename T>
const std::string& BuildItem<T>::Error() noexcept {
  return error_;
}

template <typename T>
BuildItem<T>& BuildItem<T>::SetKey(const std::string& key) noexcept {
  key_ = key;
  return *this;
}

template <typename T>
BuildItem<T>& BuildItem<T>::AsSingleInstance() noexcept {
  count_option_ = InstanceCountOptionEnum::kSingle;
  return *this;
}

template <typename T>
BuildItem<T>& BuildItem<T>::AsMultipleInstance() noexcept {
  count_option_ = InstanceCountOptionEnum::kMultiple;
  return *this;
}

template <typename T>
BuildItem<T>& BuildItem<T>::AsLockPoolInstance(uint32_t pool_size) noexcept {
  count_option_ = InstanceCountOptionEnum::kLockPool;
  pool_size_ = pool_size;
  return *this;
}

template <typename T>
BuildItem<T>& BuildItem<T>::AsPoolInstance(uint32_t pool_size) noexcept {
  count_option_ = InstanceCountOptionEnum::kSoftPool;
  pool_size_ = pool_size;
  return *this;
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_BUILD_ITEM_H_
