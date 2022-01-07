#include "mock_unit_level_1.h"

namespace cpptoolkit {
namespace factory {

int32_t MockUnitLevel_1::constructor_counter_ = 0;
int32_t MockUnitLevel_1::destructor_counter_ = 0;

uintptr_t MockUnitLevel_1::getMyPtr() {
  return reinterpret_cast<uintptr_t>(this);
}

int32_t MockUnitLevel_1::getConstructorCounter() {
  return constructor_counter_;
}

int32_t MockUnitLevel_1::getDestructorCounter() { return destructor_counter_; }

void MockUnitLevel_1::reset() {
  constructor_counter_ = 0;
  destructor_counter_ = 0;
}

}  // namespace factory
}  // namespace cpptoolkit
