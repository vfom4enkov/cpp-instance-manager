#include "mock_unit_level_2.h"

namespace cpptoolkit {
namespace factory {
int32_t MockUnitLevel_2::constructor_counter_ = 0;
int32_t MockUnitLevel_2::destructor_counter_ = 0;

int32_t MockUnitLevel_2::getConstructorCounter() {
  return constructor_counter_;
}

int32_t MockUnitLevel_2::getDestructorCounter() { return destructor_counter_; }

void MockUnitLevel_2::reset() {
  constructor_counter_ = 0;
  destructor_counter_ = 0;
}

uintptr_t MockUnitLevel_2_A::getMyPtr() {
  return reinterpret_cast<uintptr_t>(this);
}

uintptr_t MockUnitLevel_2_B::getMyPtr() {
  return reinterpret_cast<uintptr_t>(this);
}
}  // namespace factory
}  // namespace cpptoolkit
