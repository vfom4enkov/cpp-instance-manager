#ifndef CPP_INSTANCE_MANAGER_CORE_H_
#define CPP_INSTANCE_MANAGER_CORE_H_

#include <functional>
#include <string>

namespace cpp_instance_manager {

/// @brief Key for objects without key
static const std::string DEFAULT_KEY = "default";

/// @brief Contains all data about registered classes and instancess
class Core {
 public:
  ~Core() noexcept;

  /// @brief Register class T in the core
  /// @tparam T - Type of managed object
  /// @param [in] count_option - Number of copies produced object
  /// @param [in] key - Unique key for a given object type
  /// @param [in] create - Function for create instance of managed object
  /// @return Result of the operation true - success, false - fail
  template <typename T>
  bool Add(
      InstanceCountOptionEnum count_option,
      std::string key,
      std::function<T*(DependencyHelper&)>&& create) noexcept;
  // TODO VFomchenkov: Rename DependencyHelper to DependencyResolveHelper

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
};

// Implementation

template <class T>
static inline std::string Core::CreateTypeKey(const std::string& key) noexcept {
  return typeid(T).name() + " / " + key;
}

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_CORE_H_
