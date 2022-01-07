#include "mock_unit_single_instance.h"

namespace cpptoolkit {
namespace factory {

int32_t MockUnitSingleInstance::constructor_counter_ = 0;
int32_t MockUnitSingleInstance::destructor_counter_ = 0;

uintptr_t MockUnitSingleInstance::getMyPtr() {
  return reinterpret_cast<uintptr_t>(this);
}

int32_t MockUnitSingleInstance::getConstructorCounter() {
  return constructor_counter_;
}
int32_t MockUnitSingleInstance::getDestructorCounter() { return destructor_counter_; }

void MockUnitSingleInstance::reset() {
  constructor_counter_ = 0;
  destructor_counter_ = 0;
}

}  // namespace factory
}  // namespace cpptoolkit
