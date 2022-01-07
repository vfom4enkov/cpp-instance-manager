#ifndef CPP_TOOL_KIT_FACTORY_BASE_INSTANCE_MANAGER_H_
#define CPP_TOOL_KIT_FACTORY_BASE_INSTANCE_MANAGER_H_

#include <functional>
#include <string>

#include "context.h"
#include "error_context.h"
#include "resolver.h"
#include "unique_ptr_tool.h"

namespace cpptoolkit {
namespace factory {

/// @brief Base class for all object managers
class AInstanceManager {
 public:
  virtual ~AInstanceManager() noexcept = default;

  /// @brief Unique key for current manager
  /// @return Key
  virtual const std::string& TypeKey() noexcept = 0;
};

/// @brief Base object for instance managers
/// @tparam T Type of managed object
template <typename T>
class BaseInstanceManager : public AInstanceManager {
 public:
  /// @brief Create BaseInstanceManager
  /// @param [in] class_name_key - Unique key for current manager
  /// @param [in] create - Function for create instance of managed object
  /// @param [in] core - Pointer to the core_ with registered objects
  BaseInstanceManager(const std::string class_name_key,
                      std::function<T*(Resolver&)>&& create,
                      Core* core) noexcept
      : class_name_key_(class_name_key),
        create_(std::move(create)),
        core_(core){};

  /// @brief Create instance of managed object and save it to the context
  /// @return Context with instance of managed object
  virtual std::unique_ptr<BaseContext<T>> Get() noexcept = 0;

  const std::string& TypeKey() noexcept override;

 protected:
  inline void Create(Context<T>* context) noexcept;

 private:
  inline void AddError(Context<T>* context, std::string& error) noexcept;

 protected:
  std::function<T*(Resolver&)> create_;
  Core* core_;
  std::string class_name_key_;
};

// Implementation

template <typename T>
inline const std::string& BaseInstanceManager<T>::TypeKey() noexcept {
  return class_name_key_;
}

template <typename T>
inline void BaseInstanceManager<T>::Create(Context<T>* context) noexcept {
  Resolver dependencyHelper(core_, context);

  try {
    T* instance_ptr = create_(dependencyHelper);
    context->SetInstance(instance_ptr);

    if (instance_ptr == nullptr && context->IsValid()) {
      std::string error = "Error on create instance: " + class_name_key_ +
                          " 'Create' returned nullptr";
      AddError(context, error);
      return;
    }
  } catch (std::exception& ex) {
    // TODO add this case to documentation
    // If the context contains other error, then do not save current exception
    if (context->IsValid()) {
      std::string error = "Error on create instance: " + class_name_key_ +
                          " Error: " + ex.what();
      AddError(context, error);
    }
  } catch (...) {
    // TODO (VFomchenkov) add this case to documentation
    // The same behaviour for current case
    if (context->IsValid()) {
      std::string error =
          "Unknown error on create instance: " + class_name_key_;
      AddError(context, error);
    }
  }
}

template <typename T>
inline void BaseInstanceManager<T>::AddError(Context<T>* context,
                                             std::string& error) noexcept {
  std::unique_ptr<ErrorContext<T>> error_context =
      MakeUnique<ErrorContext<T>>(error);
  context->Add(std::move(error_context));
}

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_BASE_INSTANCE_MANAGER_H_
