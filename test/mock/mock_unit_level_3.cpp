#include "mock_unit_level_3.h"

namespace cpptoolkit {
namespace factory {
int32_t MockUnitLevel_3::constructor_counter_ = 0;
int32_t MockUnitLevel_3::destructor_counter_ = 0;
MockUnitLevel_3::MockUnitLevel_3() { constructor_counter_++; }

MockUnitLevel_3::~MockUnitLevel_3() { destructor_counter_++; }

uintptr_t MockUnitLevel_3::getMyPtr() {
  return reinterpret_cast<uintptr_t>(this);
}

int32_t MockUnitLevel_3::getConstructorCounter() {
  return constructor_counter_;
}

int32_t MockUnitLevel_3::getDestructorCounter() { return destructor_counter_; }

void MockUnitLevel_3::reset() {
  constructor_counter_ = 0;
  destructor_counter_ = 0;
}
}  // namespace factory
}  // namespace cpptoolkit
