#ifndef CPP_INSTANCE_MANAGER_DEPENDENCY_HELPER_H_
#define CPP_INSTANCE_MANAGER_DEPENDENCY_HELPER_H_

#include <cstdint>
#include <functional>
#include <vector>
#include <string>

#include "context.h"

namespace cpp_instance_manager {

class Core;

/// Provides access to registered in the core objects.
/// These objects will be used as dependency objects.
/// To get an instance of object call 'Get()' or 'Get(key)' function.
/// All dependency instances will be deleted automaticaly after delete instance
/// of main object.
class DependencyHelper {
 public:
  /// @brief Create instance of the helper
  /// @param [in] core - Pointer to the core with registered objects
  /// @param [in] base_context - Pointer to the context for save dependencies
  DependencyHelper(Core* core, BaseContext* context) noexcept
      : core_(core), context_(context), has_error_(false){};

  /// Get dependency, registered in the core with default key.
  /// In error case the function will return 'nullptr'.
  /// To get more info about the error call 'LastError()'.
  /// @tparam N - Type of dependency object
  /// @return Pointer to dependency object or 'nullptr' in error case
  template <typename N>
  N* Get() noexcept;

  /// Get dependency, registered in the core with the key.
  /// In error case the function will return 'nullptr'.
  /// To get more info about the error call 'LastError()'.
  /// @tparam N - Type of dependency object
  /// @param [in] key - The key for access
  /// @return Pointer to dependency object or 'nullptr' in error case
  template <typename N>
  N* Get(std::string key) noexcept;

  /// Get description for the error
  /// @return Error description
  std::string get_last_error() noexcept { return last_error_; };

 protected:
  bool has_error_;
  
private:
  BaseContext* context_;
  Core* core_;
  std::string last_error_;
};

/// @brief Checks if was error on get dependency operation and get dependencies
class DependencyHelperInspector : public DependencyHelper {
 public:
  /// @brief Create instance of the helper
  /// @param [in] core - Pointer to the core with registered objects
  /// /// @param [in] base_context - Pointer to the context for save dependencies
  DependencyHelperInspector(Core* core, BaseContext* context) noexcept : DependencyHelper(core, context) {};

  /// Ñheck if there were errors or not
  /// @return The check result
  bool has_error() noexcept { return has_error_; }
};

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_DEPENDENCY_HELPER_H_
