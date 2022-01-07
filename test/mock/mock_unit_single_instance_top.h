#ifndef CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_SINGLE_INSTANCE_TOP_H_
#define CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_SINGLE_INSTANCE_TOP_H_

#include "mock_unit_level_1.h"
#include "mock_unit_single_instance.h"

namespace cpptoolkit {
namespace factory {

class MockUnitSingleInstanceTop {
 public:
  MockUnitSingleInstanceTop(MockUnitLevel_1* unit_1,
                            MockUnitSingleInstance* unit_2)
      : unit_1_(unit_1), unit_2_(unit_2) {
    constructor_counter_++;
  }
  ~MockUnitSingleInstanceTop() { destructor_counter_++; }
  static int32_t getConstructorCounter();
  static int32_t getDestructorCounter();
  static void reset();

 public:
  static int32_t constructor_counter_;
  static int32_t destructor_counter_;

 private:
  MockUnitLevel_1* unit_1_;
  MockUnitSingleInstance* unit_2_;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_SINGLE_INSTANCE_TOP_H_
