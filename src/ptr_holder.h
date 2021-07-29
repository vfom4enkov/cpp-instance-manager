#ifndef CPP_INSTANCE_MANAGER_PTR_HOLDER_H_
#define CPP_INSTANCE_MANAGER_PTR_HOLDER_H_

namespace cpp_instance_manager {
/// @brief Contains pointer to heap object, on destroy the object will be
/// deleted from heap
/// @tparam T - Type of managed object
template <class T>
class PtrHolder {
 public:
  /// @brief Create PtrHolder
  /// @param [in] ptr - pointer to managed object
  PtrHolder(T* ptr) noexcept;

  /// @brief Move object constructor
  /// @param other Another instance
  PtrHolder(PtrHolder&& other) noexcept;

  ~PtrHolder() noexcept;

  /// @brief Get direct pointer to managed object
  /// @return Pointer to managed object
  T* get() const noexcept;

  /// @brief Overloaded access operator provides access to public fields and
  /// methods of T class
  T* operator->() const;

  // Ban copy operations
  PtrHolder(PtrHolder& src) = delete;

  // Ban copy assignment operations
  void operator=(const PtrHolder<T>& other) = delete;

 private:
  T* instance_;
};

}  // namespace cpp_instance_manager

#endif  // CPP_INSTANCE_MANAGER_PTR_HOLDER_H_
