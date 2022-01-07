#ifndef CPP_TOOL_KIT_FACTORY_DEPENDENCY_CONTAINER_H_
#define CPP_TOOL_KIT_FACTORY_DEPENDENCY_CONTAINER_H_

#include <memory>

#include "abstract_context.h"

namespace cpptoolkit {
namespace factory {

/// @brief Contains base information about dependency
class DependencyContainer {
 public:
  virtual ~DependencyContainer() noexcept = default;

  /// @brief Add context of dependent object pointer
  /// @param dependency - Context of dependent object
  virtual void Add(std::unique_ptr<AContext>&& dependency) noexcept = 0;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_DEPENDENCY_CONTAINER_H_
