#ifndef CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_1_H_
#define CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_1_H_

#include "mock_unit_level_2.h"

namespace cpptoolkit {
namespace factory {

// First implementation of Middle class
class MockUnitLevel_1 {
 public:
  MockUnitLevel_1(MockUnitLevel_2* unit, MockUnitLevel_2* unit_2)
      : unit_(unit), unit_2_(unit_2) {
    constructor_counter_++;
  };
  ~MockUnitLevel_1() { destructor_counter_++; };
  uintptr_t getMyPtr();
  static int32_t getConstructorCounter();
  static int32_t getDestructorCounter();
  static void reset();

 public:
  static int32_t constructor_counter_;
  static int32_t destructor_counter_;

 private:
  MockUnitLevel_2* unit_;
  MockUnitLevel_2* unit_2_;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_1_H_
