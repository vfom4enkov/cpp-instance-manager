#ifndef CPP_TOOL_KIT_FACTORY_CORE_EXTENSION_H_
#define CPP_TOOL_KIT_FACTORY_CORE_EXTENSION_H_

#include "core.h"

namespace cpptoolkit {
namespace factory {

/// @brief Extends Core class provides registration of instance manager in Core
class CoreExtension : public Core {
 public:
  CoreExtension() = default;
  virtual ~CoreExtension() = default;

  /// @brief Add instance manager to Core, in fail case get error description
  /// call 'Error()'
  /// @param [in] manager - Instance manager
  /// @return Operation result
  bool Add(std::unique_ptr<AInstanceManager>&& mgr) noexcept {
    const std::string& type_key = mgr->TypeKey();

    if (index_.count(type_key) == 1) {
      error_ = "Type: " + type_key + " already registered";
      return false;
    }

    index_.emplace(type_key, std::move(mgr));
    return true;
  };
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_CORE_EXTENSION_H_
