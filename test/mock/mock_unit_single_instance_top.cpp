#include "mock_unit_single_instance_top.h"

namespace cpptoolkit {
namespace factory {

int32_t MockUnitSingleInstanceTop::constructor_counter_ = 0;
int32_t MockUnitSingleInstanceTop::destructor_counter_ = 0;

int32_t MockUnitSingleInstanceTop::getConstructorCounter() {
  return constructor_counter_;
}
int32_t MockUnitSingleInstanceTop::getDestructorCounter() {
  return destructor_counter_;
}

void MockUnitSingleInstanceTop::reset() {
  constructor_counter_ = 0;
  destructor_counter_ = 0;
}

}  // namespace factory
}  // namespace cpptoolkit
