#ifndef CPP_TOOL_KIT_FACTORY_RESOLVER_H_
#define CPP_TOOL_KIT_FACTORY_RESOLVER_H_

#include <string>
#include <memory>

#include "dependency_container.h"

namespace cpptoolkit {
namespace factory {

class Core;

/// Provides access to registered in Core objects
/// These objects will be used as dependencies
/// To get an instance of object call 'Get()' or 'Get(key)' function
/// All dependency instances will be deleted automatically after delete instance
/// of main object.
class Resolver {
 public:
  /// @brief Create instance of the helper
  /// @param [in] core - Pointer to the core_ with registered objects
  /// @param [in] d_container - Collector for save dependencies
  Resolver(Core* core, DependencyContainer* d_container) noexcept
      : core_(core),
        d_container_(d_container),
        is_valid_dependency_context_(true){};

  /// @brief Get dependency, registered in the core_ with the key.
  /// @tparam T - Type of dependency object
  /// @param [in] key - The key for access
  /// @return Pointer to dependency object or 'nullptr' in error case
  template <typename T>
  T* Get(const std::string& key = DEFAULT_KEY) noexcept;

 private:
  Core* core_;
  DependencyContainer* d_container_;
  bool is_valid_dependency_context_;
};

// implementation

template <typename T>
inline T* Resolver::Get(const std::string& key) noexcept {
  if (!is_valid_dependency_context_) {  // No sense to do something here,
    return nullptr;                     // a dependency context already has error
  }

  std::unique_ptr<BaseContext<T>> dependency = core_->Get<T>(key);

  // set error if the context is not valid
  is_valid_dependency_context_ = dependency->IsValid();
  T* instance = dependency->GetInstance();
  d_container_->Add(std::move(dependency));

  if (!is_valid_dependency_context_) {
    return nullptr;
  }

  return instance;
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_RESOLVER_H_
