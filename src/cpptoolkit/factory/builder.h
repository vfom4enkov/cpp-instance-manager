#ifndef CPP_TOOL_KIT_FACTORY_BUILDER_H_
#define CPP_TOOL_KIT_FACTORY_BUILDER_H_

#include <functional>
#include <vector>

#include "core_extension.h"
#include "tool/build_item.h"

namespace cpptoolkit {
namespace factory {

/// @brief Helper for registration objects
class Builder {
 public:
  Builder() noexcept = default;
  ~Builder() noexcept = default;

  /// @brief Create fluent helper allows to set properties for the registered
  /// object
  /// @tparam T - Type of managed object
  /// @param [in] create - Function for create instance of managed object
  /// @return Reference to helper
  template <typename T>
  BuildItem<T>& Register(std::function<T*(Resolver&)>&& create) noexcept;

  /// @brief Create fluent helper allows to set properties for the registered
  /// object without dependencies
  /// @tparam T - Type of managed object
  /// @return Reference to helper
  template <typename T>
  BuildItem<T>& RegisterType() noexcept;

  /// @brief Create Core, if Core is nullptr check 'Error()'
  /// @return Core for produce objects
  std::unique_ptr<Core> Build() noexcept {
    if (items_.size() == 0) {
      error_ = "There are no object for registration, list is empty";
      return std::unique_ptr<Core>(nullptr);
    }

    std::unique_ptr<CoreExtension> uptr_core = MakeUnique<CoreExtension>();

    CoreExtension* core = uptr_core.get();
    for (auto& item : items_) {
      if (!item->Build(core)) {
        error_ = item->Error();
        return std::unique_ptr<Core>(nullptr);
      }
    }

    items_.clear();

    return std::move(uptr_core);
  };

  /// @brief Date about last error
  /// @return Error description
  const std::string& Error() noexcept { return error_; };

 private:
  std::vector<std::unique_ptr<ABuildItem>> items_;
  std::string error_;
};

// Implementation

template <typename T>
inline BuildItem<T>& Builder::Register(
    std::function<T*(Resolver&)>&& create) noexcept {
  std::unique_ptr<BuildItem<T>> item =
      MakeUnique<BuildItem<T>>(std::move(create));
  BuildItem<T>* ptr = item.get();
  items_.push_back(std::move(item));
  return *ptr;
}

template <typename T>
inline BuildItem<T>& Builder::RegisterType() noexcept {
  std::function<T*(Resolver&)> create = [](Resolver& resolver) -> T* {
    return Create<T>();
  };
  return Register(std::move(create));
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_BUILDER_H_
