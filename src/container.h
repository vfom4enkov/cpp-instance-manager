#ifndef CPP_INSTANCE_MANAGER_CONTAINER_H_
#define CPP_INSTANCE_MANAGER_CONTAINER_H_

#include <mutex>

#include "core.h"
#include "ptr_holder.h"

namespace cpp_instance_manager {

/// @brief Provides thread safe access to registered objects
class Container {
 public:
  /// @brief Create instance of container
  /// @param [in] core - Pointer to Core with registered objects
  Container(Core* core) noexcept : core_(core), error_listener_(nullptr){};
  
  /// @brief Create instance of container
  /// @param [in] core - Pointer to Core with registered objects 
  /// @param [in] error_listener - Observer to know about all errors in the core 
  /// @return 
  Container(Core* core,
            std::function<void(const std::string)>&& error_listener) noexcept
      : core_(core), error_listener_(std::move(error_listener)){};

  ~Container() noexcept;

  /// @brief Get instance of managed object
  /// @tparam T - Type of managed object
  /// @return Pointer to object ('nullptr' in error case)
  template <class T>
  T* Get() noexcept;

  /// @brief Get instance of managed object by key
  /// @tparam T - Type of managed object
  /// @param [in] key - Key for managed object
  /// @return Pointer to object ('nullptr' in error case)
  template <class T>
  T* Get(std::string key) noexcept;

  /// @brief Get PtrHolder on instance of managed object
  /// @tparam T - Type of managed object
  /// @return PtrHolder on instance of managed object
  template <typename T>
  PtrHolder<T> GetHolder() noexcept;

  /// @brief Get PtrHolder on instance of managed object by key
  /// @tparam T - Type of managed object
  /// @param [in] key - Key for managed object
  /// @return PtrHolder on instance of managed object
  template <typename T>
  PtrHolder<T> GetHolder(const std::string& key) noexcept;

  /// @brief Delete managed object and dependencies (not actual for single instances)
  /// @tparam T - Type of managed object
  /// @param [in/out] instance - Pointer to managed object (after the operation it will be 'nullptr')
  template <class T>
  void Collect(T*& instance) noexcept;

  /// @brief Delete single instance and dependencies
  /// @tparam T - Type of managed object
  /// @param [in/out] instance - Pointer to managed object (after the operation it will be 'nullptr')
  template <class T>
  void CollectSingleInstance(T*& instance) noexcept;

  /// @brief Get description for last error
  /// @return Error description
  std::string LastError() noexcept;

 private:
  Core* core_;
  std::function<void(const std::string)> error_listener_;
  std::mutex mutex_;
  std::string last_error_;
};

// Implementation

Container::~Container() noexcept { delete core_; }

std::string Container::LastError() noexcept { return last_error_; }

template <class T>
T* Container::Get() noexcept {
  return Get<T>(DEFAULT_KEY);
}

template <class T>
T* Container::Get(std::string key) noexcept {
  const std::lock_guard<std::mutex> lock(mutex_);

  T* instance = core_->Get<T>(key);

  if (instance != nullptr) {
    mutex_.unlock();
    return instance;
  }

  last_error_ = core_->LastError();
  if (error_listener_ != nullptr) {
    try {
      error_listener_(last_error_);
    } catch (...) {
      // do nothing here!
    }
  }

  return nullptr;
}

template <typename T>
inline PtrHolder<T> Container::GetHolder() noexcept {
  return GetHolder<T>(DEFAULT_KEY);
}

template <typename T>
inline PtrHolder<T> Container::GetHolder(const std::string& key) noexcept {
  PtrHolder<T> holder(this, Get<T>(key));
  return std::move(holder);
}

template <class T>
void Container::Collect(T*& instance) noexcept {
  const std::lock_guard<std::mutex> lock(mutex_);
  core_->Collect(instance);
}

template <class T>
void Container::CollectSingleInstance(T*& instance) noexcept {
  const std::lock_guard<std::mutex> lock(mutex_);
  core_->CollectSingleInstance(instance);
}

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_CONTAINER_H_
