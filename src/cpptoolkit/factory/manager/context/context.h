#ifndef CPP_TOOL_KIT_FACTORY_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_CONTEXT_H_

#include <vector>

#include "base_context.h"
#include "dependency_container.h"

namespace cpptoolkit {
namespace factory {

class Core;

/// @brief Contains pointer to managed object
/// @tparam T - Type of managed object
template <typename T>
class Context : public BaseContext<T>, public DependencyContainer {
 public:
  /// @brief Create context
  Context() noexcept : BaseContext<T>(true){};

  ~Context() noexcept;

  void Add(std::unique_ptr<AContext>&& dependency) noexcept override;

  // Ban RAII operations
  Context(const Context&) = delete;
  Context(Context&& other) = delete;
  Context& operator=(Context&& other) = delete;
  Context& operator=(const Context&) = delete;

 private:
  std::vector<std::unique_ptr<AContext>> dependencies_;
};

// implementation

template <typename T>
Context<T>::~Context() noexcept {
  if (instance_ptr_ != nullptr) {
    delete instance_ptr_;
  }
}

template <typename T>
inline void Context<T>::Add(std::unique_ptr<AContext>&& dependency) noexcept {
  bool dependency_valid = dependency->IsValid();
  if (!dependency_valid) {
    is_valid_ = dependency_valid;
    error_ = dependency->Error();
  }

  dependencies_.push_back(std::move(dependency));
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_CONTEXT_H_
