#ifndef CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_SINGLE_INSTANCE_H_
#define CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_SINGLE_INSTANCE_H_

#include "mock_unit_level_3.h"

namespace cpptoolkit {
namespace factory {

class MockUnitSingleInstance {
 public:
  MockUnitSingleInstance(MockUnitLevel_3* unit) : unit_(unit) {
    constructor_counter_++;
  };
  ~MockUnitSingleInstance() { destructor_counter_++; };
  uintptr_t getMyPtr();
  static int32_t getConstructorCounter();
  static int32_t getDestructorCounter();
  static void reset();

 public:
  static int32_t constructor_counter_;
  static int32_t destructor_counter_;

 private:
  MockUnitLevel_3* unit_;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_SINGLE_INSTANCE_H_
