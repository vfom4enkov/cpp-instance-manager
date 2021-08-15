#ifndef CPP_INSTANCE_MANAGER_BUILD_ITEM_H_
#define CPP_INSTANCE_MANAGER_BUILD_ITEM_H_

#include <memory>
#include <vector>

#include "core.h"

namespace cpp_instance_manager {

/// @brief Base abstract class for BuildItem
class BaseBuildItem {
 public:
  virtual ~BaseBuildItem() = default;

  /// @brief Add information about type to core
  /// @param [in] core - Pointer to core
  /// @return Operation result
  virtual bool Build(Core* core) noexcept = 0;

  /// @brief Get error description about last error
  /// @return Error description
  virtual std::string LastError() noexcept = 0;
};


/// @brief Fluent helper for set property of registering object
/// @tparam T - Type of managed object
template <typename T>
class Registrator {
 public:
  /// @brief Create helper
  /// @param [in] create - Function for create instance of managed object
  Registrator(std::function<T*(DependencyHelper&)>&& create) noexcept
      : create_(std::move(create)),
        count_option_(InstanceCountOptionEnum::kMultiple),
        key_(DEFAULT_KEY),
        base_type_(typeid(T).name()){};

  /// @brief Set unique key for managed object (it needs for define multiple
  /// implementation of one abstract class)
  /// @param [in] key - Key value
  /// @return Pointer to current object
  Registrator<T>* SetKey(const std::string& key) noexcept;

  /// @brief Register type as single instance
  /// @return Pointer to current object
  Registrator<T>* AsSingleInstance() noexcept;

  /// @brief Register type as multiple instance (default value)
  /// @return Pointer to current object
  Registrator<T>* AsMultipleInstance() noexcept;

  /// @brief Register object as base object
  /// @tparam BaseT - Base class for registered object
  /// @return Pointer to current object
  template <typename BaseT>
  Registrator<T>* As() noexcept;

 protected:
  std::function<T*(DependencyHelper&)>&& create_;
  std::string key_;
  std::string base_type_;
  std::string last_error_;
  InstanceCountOptionEnum count_option_;
};

/// @brief Implementation of BaseBuildItem for special class T
/// @tparam T - Type of managed object
template <typename T>
class BuildItem : public Registrator<T>, public BaseBuildItem {
 public:
  BuildItem(std::function<T*(DependencyHelper&)>&& create) noexcept
      : Registrator(create){};
  bool Build(Core* core) noexcept override;
  std::string LastError() noexcept override;
};

// Implementation
// Implementation of Registrator
template <typename T>
Registrator<T>* Registrator<T>::SetKey(const std::string& key) noexcept {
  key_ = key;
  return this;
}

template <typename T>
Registrator<T>* Registrator<T>::AsSingleInstance() noexcept {
  count_option_ = InstanceCountOptionEnum::kSingle;
  return this;
}

template <typename T>
Registrator<T>* Registrator<T>::AsMultipleInstance() noexcept {
  count_option_ = InstanceCountOptionEnum::kMultiple;
  return this;
}

template <typename T>
template <typename BaseT>
Registrator<T>* Registrator<T>::As() noexcept {
  if (std::is_base_of<BaseT, T>::value == 1) {
    base_type_ = typeid(BaseT).name();
  } else {
    last_error_ = typeid(BaseT).name() + " is not base type for " +
                  typeid(T).name() base_type_ = "";
  }
  return this;
}

// Implementation of BuildItem

template <typename T>
inline bool BuildItem<T>::Build(Core* core) noexcept {
  if (base_type_.empty()) {
    last_error_ = "Base type for " + typeid(T).name() + " can not be empty";
    return false;
  }

  if (!core->Add(count_option_, key_, create_, base_type_)) {
    last_error_ = core->LastError();
    return false;
  }

  return true;
}

template <typename T>
std::string BuildItem<T>::LastError() noexcept {
  return last_error_;
}


}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_BUILD_ITEM_H_
