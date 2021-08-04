#ifndef CPP_INSTANCE_MANAGER_SINGLE_INSTANCE_MANAGER_H_
#define CPP_INSTANCE_MANAGER_SINGLE_INSTANCE_MANAGER_H_

#include "base_instance_manager.h"

namespace cpp_instance_manager {

/// @brief Instance manager for single object
/// @tparam T Type of managed object
template <class T>
class SingleInstanceManager : public BaseInstanceManager<T> {
 public:
  /// @brief Create SingleInstanceManager
  /// @param [in] class_name_key - Unique key for current manager
  /// @param [in] create - Function for create instance of managed object
  /// @param [in] core - Pointer to the core with registered objects
  SingleInstanceManager(std::string class_name_key,
                        std::function<T*(DependencyHelper&)>&& create,
                        Core* core) noexcept
      : BaseInstanceManager(class_name_key, std::move(create), core) {};

  virtual ~SingleInstanceManager() noexcept {};

  /// @brief Get instance of managed object
  /// @return Pointer to the object
  T* Get() noexcept;

  InstanceCountOptionEnum CountOption() noexcept override;
  void Collect(uintptr_t pointer) noexcept override;

  /// @brief Collect (delete) current single instance and dependencies
  void Collect() noexcept;

 private:
  // TODO: Hack, review storage for context
  std::vector<Context<T>> instance_;
};

// Implementation

template <class T>
T* SingleInstanceManager<T>::Get() noexcept {
  if (instance_.size() > 0) {
    return instance_.at(0).instance_ptr_;
  }

  Context<T> context(core_);
  bool create_result = Create(context);
  if (create_result) {
    instance_.emplace_back(std::move(context));
    return instance_.at(0).instance_ptr_;
  }

  return nullptr;
}

template <class T>
InstanceCountOptionEnum SingleInstanceManager<T>::CountOption() noexcept {
  return InstanceCountOptionEnum::kSingle;
}

template <class T>
void SingleInstanceManager<T>::Collect(uintptr_t pointer) noexcept {
  // do nothing here, because it's single instance manager
  // no one needs to collect data for single instance object
}

template <class T>
void SingleInstanceManager<T>::Collect() noexcept {
  instance_.clear();
}

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_SINGLE_INSTANCE_MANAGER_H_
