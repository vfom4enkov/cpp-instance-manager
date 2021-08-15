#ifndef CPP_INSTANCE_MANAGER_BUILDER_H_
#define CPP_INSTANCE_MANAGER_BUILDER_H_

#include <memory>
#include <vector>

#include "build_item.h"
#include "container.h"

namespace cpp_instance_manager {

/// @brief Helper for register objects
class Builder {
 public:
  /// @brief Create helper
  Builder() noexcept {};
  ~Builder() noexcept {};

  /// @brief Create fluent helper for set property of registering object
  /// @tparam T - Type of managed object
  /// @param [in] create - Function for create instance of managed object
  template <typename T>
  Registrator<T>* Register(std::function<T*(DependencyHelper&)>&& create);

  /// @brief Create fluent helper for set property of registering object
  /// @tparam T - Type of managed object
  /// @param [in] create - Function for create instance of managed object
  template <typename T>
  Registrator<T>* RegisterType();

  /// @brief Create container with thread safe access to registered objects
  /// @return Shared pointer with container or empty shared pointer in error case
  std::shared_ptr<Container> Build() noexcept;
  
  /// @brief Get description of last error
  /// @return Description of last error
  std::string LastError() noexcept;

 private:
  std::vector<std::shared_ptr<BaseBuildItem>> build_items_;
  std::string last_error_;
};

// Implementation

template <typename T>
Registrator<T>* Builder::Register(
    std::function<T*(DependencyHelper&)>&& create) {
  std::shared_ptr<BuildItem<T>> build_item = std::make_shared<BuildItem<T>>(create);
  Registrator<T>* item = build_item.get();
  build_items_.push_back(std::move(build_item));
  return item;
}

template <typename T>
Registrator<T>* Builder::RegisterType() {
  return Register(
      [](DependencyHelper& helper) -> T* { return new (std::nothrow) T(); });
}

std::shared_ptr<Container> Builder::Build() noexcept {
  Core* core_ptr = new (std::nothrow) Core();
  if (core_ptr == nullptr) {
    last_error_ = "RAM is over";
    return std::shared_ptr<Container>();
  }

  std::shared_ptr<Container> container = std::make_shared<Container>(core_ptr);

  for (auto& buld_item : build_items_) {
    if (!buld_item->Build(core_ptr)) {
      last_error_ = buld_item->LastError();
      return std::shared_ptr<Container>();
    }
  }

  return container;
}

std::string Builder::LastError() noexcept { return last_error_; }

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_BUILDER_H_
