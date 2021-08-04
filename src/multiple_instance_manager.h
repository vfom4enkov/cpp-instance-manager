#ifndef CPP_INSTANCE_MANAGER_MULTIPLE_INSTANCE_MANAGER_H_
#define CPP_INSTANCE_MANAGER_MULTIPLE_INSTANCE_MANAGER_H_

#include <unordered_map>

#include "base_instance_manager.h"

namespace cpp_instance_manager {

/// @brief Instance manager for multiple objects
/// @tparam T Type of managed object
template <class T>
class MultipleInstanceManager : public BaseInstanceManager<T> {
 public:
  /// @brief Create MultibpleInstanceManager
  /// @param [in] class_name_key - Unique key for current manager
  /// @param [in] create - Function for create instance of managed object
  /// @param [in] core - Pointer to the core with registered objects
  MultipleInstanceManager(std::string class_name_key,
                          std::function<T*(DependencyHelper&)>&& create,
                          Core* core) noexcept
      : BaseInstanceManager(class_name_key, std::move(create), core){};

  virtual ~MultipleInstanceManager() noexcept {};

  /// @brief Get instance of managed object
  /// @return Pointer to the object
  T* Get() noexcept;

  InstanceCountOptionEnum CountOption() noexcept override;
  void Collect(uintptr_t pointer) noexcept override;

 private:
  std::unordered_map<uintptr_t, Context<T>> instance_index_;
};

template <class T>
T* MultipleInstanceManager<T>::Get() noexcept {
  Context<T> context(core_);
  bool create_result = Create(context);
  if (!create_result) {
    return nullptr;
  }

  T* ptr = context.get_instance_ptr();
  uintptr_t instance_uintptr = reinterpret_cast<uintptr_t>(ptr);
  if (instance_index_.count(instance_uintptr) == 1) {
    // pointer to the object already exist in instance_index_
    // it means incorrect working of create_ function
    // we don't know what is incide of create_ function
    // it unusual case, avoid it
    // TODO add this case to documentation
    last_error_ = "Create function for object: " + class_name_key_ +
                  " returned pointer to instance created before";
    return nullptr;
  }

  instance_index_.emplace(instance_uintptr, std::move(context));
  return ptr;
}

// Implementation
template <class T>
InstanceCountOptionEnum MultipleInstanceManager<T>::CountOption() noexcept {
  return InstanceCountOptionEnum::kMultiple;
}

template <class T>
void MultipleInstanceManager<T>::Collect(uintptr_t pointer) noexcept {
  instance_index_.erase(pointer);
}

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_MULTIPLE_INSTANCE_MANAGER_H_
