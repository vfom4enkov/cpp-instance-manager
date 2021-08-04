#ifndef CPP_INSTANCE_MANAGER_CORE_H_
#define CPP_INSTANCE_MANAGER_CORE_H_

#include <functional>
#include <string>
#include <unordered_map>

#include "dependency_helper.h"
#include "instance_count_option_enum.h"
#include "multiple_instance_manager.h"
#include "single_instance_manager.h"

namespace cpp_instance_manager {

/// @brief Key for objects without key
static const std::string DEFAULT_KEY = "default";

/// @brief Contains all data about registered classes and instancess
class Core {
 public:
  /// @brief Create Core
  Core() noexcept : last_error_(""){};

  ~Core() noexcept;

  /// @brief Register class T in the core
  /// @tparam T - Type of managed object
  /// @param [in] count_option - Number of copies produced object
  /// @param [in] key - Unique key for a given object type
  /// @param [in] create - Function for create instance of managed object
  /// @return Result of the operation true - success, false - fail
  template <typename T>
  bool Add(InstanceCountOptionEnum count_option, std::string key,
           std::function<T*(DependencyHelper&)>&& create) noexcept;

  /// @brief Get instance of managed object
  /// @tparam T - Type of managed object
  /// @param [in] key - Unique key for a given object type
  /// @return Pointer to managed object in success case or 'nullptr' in
  /// error case
  template <class T>
  T* Get(const std::string& key) noexcept;

  /// @brief Collect instance and all dependencies (not actual for single
  /// instances)
  /// @tparam T - Type of managed object
  /// @param [in] instance - Pointer to managed object (after the
  /// operation it will be 'nullptr')
  template <class T>
  void Collect(T*& instance) noexcept;

  /// @brief Collect instance and all dependencies (not actual for single
  /// instances)
  /// @param [in] instance_uintptr_t - Value of pointer to managed object
  ///	(after the operation it will be 'nullptr')
  void Collect(uintptr_t instance_uintptr_t) noexcept;

  /// @brief Collect (delete) single instance and dependencies
  /// @tparam T - Type of managed object
  /// @param [in] instance - Pointer to managed object (after the
  /// operation it will be 'nullptr')
  template <class T>
  void CollectSingleInstance(T*& instance) noexcept;

  /// @brief Get description for last error
  /// @return Error description
  std::string LastError() noexcept;

 private:
  template <class T>
  static inline std::string CreateTypeKey(const std::string& key) noexcept;

 private:
  std::unordered_map<std::string, AbstractInstanceManager*> manager_index_;
  std::unordered_map<uintptr_t, AbstractInstanceManager*> instance_index_;
  std::unordered_map<uintptr_t, AbstractInstanceManager*>
      single_instance_index_;

  std::string last_error_;
};

// Implementation

Core::~Core() noexcept {
  for (typename std::unordered_map<std::string,
                                   AbstractInstanceManager*>::iterator it =
           manager_index_.begin();
       it != manager_index_.end(); ++it) {
    delete it->second;
  }
}

template <typename T>
bool Core::Add(InstanceCountOptionEnum count_option, std::string key,
               std::function<T*(DependencyHelper&)>&& create) noexcept {
  std::string type_key = CreateTypeKey<T>(key);

  if (manager_index_.count(type_key) == 1) {
    // the instance manager for the key 'type_key' already registered
    last_error_ = "Type: " + type_key + " already registered";
    return false;
  }

  AbstractInstanceManager* instance_manager = nullptr;
  switch (count_option) {
    case InstanceCountOptionEnum::kSingle: {
      instance_manager = new (std::nothrow)
          SingleInstanceManager<T>(type_key, std::move(create), this);
      break;
    }
    case InstanceCountOptionEnum::kMultiple: {
      instance_manager = new (std::nothrow)
          MultipleInstanceManager<T>(type_key, std::move(create), this);
      break;
    }
    default: {
      std::stringstream str_stream;
      last_error_ = "Unknown count option: " + (int)instance_count;
      return false;
    }
  }

  if (instance_manager == nullptr) {
    last_error_ = "RAM is over";
    return false;
  }

  manager_index_.emplace(type_key, instance_manager);
  return true;
}

template <class T>
T* Core::Get(const std::string& key) noexcept {
  std::string type_key = CreateTypeKey<T>(key);
  const auto it = manager_index_.find(type_key);
  if (it == manager_index_.end()) {
    last_error_ = "Type: " + type_key + " is not registered";
    return nullptr;
  }

  T* object_instance = nullptr;
  AbstractInstanceManager* a_instance_manager = it->second;
  InstanceCountOptionEnum count_option = a_instance_manager->CountOption();
  switch (count_option) {
    case InstanceCountOptionEnum::kSingle: {
      auto* single_manager = reinterpret_cast<SingleInstanceManager<T>*>(a_instance_manager);
      object_instance = single_manager->Get();
      break;
    }
    case InstanceCountOptionEnum::kMultiple: {
      auto* multiple_manager = reinterpret_cast<MultipleInstanceManager<T>*>(a_instance_manager);
      object_instance = multiple_manager->Get();
      break;
    }
  }

  if (object_instance == nullptr) {
    last_error_ = a_instance_manager->LastError();
    return nullptr;
  }

  uintptr_t object_uintptr = reinterpret_cast<uintptr_t>(object_instance);
  instance_index_.emplace(object_uintptr, a_instance_manager);

  if (count_option == InstanceCountOptionEnum::kSingle)
    if (single_instance_index_.find(object_uintptr) ==
        single_instance_index_.end()) {
      // Save the object to single_instance_index_
      // for CollectSingleInstance operation if future
      single_instance_index_.emplace(object_uintptr, a_instance_manager);
    }

  return object_instance;
}

template <class T>
void Core::Collect(T*& instance) noexcept {
  uintptr_t instance_uintptr = reinterpret_cast<uintptr_t>(instance);
  Collect(instance_uintptr);
  instance = nullptr;
}

template <class T>
void Core::CollectSingleInstance(T*& instance) noexcept {
  uintptr_t instance_uintptr = reinterpret_cast<uintptr_t>(instance);
  auto it = single_instance_index_.find(instance_uintptr);
  if (it == single_instance_index_.end()) return;

  // instance is not single instance
  AbstractInstanceManager* a_instance_manager = it->second;
  SingleInstanceManager<T>* manager = static_cast<SingleInstanceManager<T>*>(a_instance_manager);
  manager->Collect();
  single_instance_index_.erase(instance_uintptr);

  instance = nullptr;
}

template <class T>
static inline std::string Core::CreateTypeKey(const std::string& key) noexcept {
  return typeid(T).name() + "/" + key;
}

void Core::Collect(uintptr_t instance_uintptr_t) noexcept {
  const auto it = instance_index_.find(instance_uintptr_t);
  if (it == instance_index_.end())
    // the object has removed, usual case for single instances
    return;

  AbstractInstanceManager* a_instance_manager = it->second;
  instance_index_.erase(instance_uintptr_t);
  a_instance_manager->Collect(instance_uintptr_t);
}

std::string Core::LastError() noexcept { return last_error_; }

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_CORE_H_
