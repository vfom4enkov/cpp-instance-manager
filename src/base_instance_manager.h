#ifndef CPP_INSTANCE_MANAGER_BASE_INSTANCE_MANAGER_H_
#define CPP_INSTANCE_MANAGER_BASE_INSTANCE_MANAGER_H_

#include <cstdint>
#include <functional>
#include <string>

#include "abstract_instance_manager.h"
#include "context.h"
#include "dependency_helper.h"

namespace cpp_instance_manager {

/// @brief Base object for instance managers
/// @tparam T Type of managed object
template <class T>
class BaseInstanceManager : public AbstractInstanceManager {
  /// @brief Create BaseInstanceManager
  /// @param [in] class_name_key - Unique key for current manager
  /// @param [in] create - Function for create instance of managed object
  /// @param [in] core - Pointer to the core with registered objects
  BaseInstanceManager(std::string class_name_key,
                      std::function<T*(DependencyHelper&)>&& create,
                      Core* core) noexcept
      : class_name_key_(class_name_key), create_(create), core_(core), last_error_(""){};

  std::string LastError() noexcept override;

 protected:
  inline bool Create(Context<T>& context) noexcept;

 protected:
  std::function<T*(DependencyHelper&)> create_;
  Core* core_;
  std::string class_name_key_;
  std::string last_error_;
};

template <class T>
inline std::string BaseInstanceManager<T>::LastError() noexcept {
  return last_error_;
}

template <class T>
inline bool BaseInstanceManager<T>::Create(Context<T>& context) noexcept {
  DependencyHelperInspector dependencyHelper(core_, &context);
  try {
    T* instance_ptr = create_(dependencyHelper);
    context.set_instance_ptr(instance_ptr);

    if (dependencyHelper.has_error()) {
      last_error_ = dependencyHelper.get_last_error();
      return false;
    }

    if (instance_ptr == nullptr) {
      last_error_ = "Error on create instance: " + class_name_key_ + " 'Create' return nullptr";
      return false;
    }

    return true;

  } catch (std::exception& ex) {
    // TODO add this case to documentation
    last_error_ = "Error on create instance: " + class_name_key_ + " Error: " + ex.what();
  } catch (...) {
    // TODO add this case to documentation
    last_error_ = "Unknown error on create instance: " + class_name_key_;
  }
  return false;
}


}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_BASE_INSTANCE_MANAGER_H_
