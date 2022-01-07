#ifndef CPP_TOOL_KIT_FACTORY_DELETER_H_
#define CPP_TOOL_KIT_FACTORY_DELETER_H_

namespace cpptoolkit {
namespace factory {

class AContext;

/// @brief Helper for delete instance in std::unique_ptr
/// @tparam T - Type of managed object
template <typename T>
class Deleter {
 public:
  /// @brief Create deleter
  /// @param [in] context - Basic information about managed objec
  Deleter(std::unique_ptr<AContext>&& context) noexcept
      : context_(std::move(context)){};

  /// @brief Handler for delete operation of managed object
  /// @param [in] inst - Pointer to instance of managed object
  void operator()(T* inst) noexcept {
    context_.reset();
  };

 private:
  std::unique_ptr<AContext> context_;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_DELETER_H_
