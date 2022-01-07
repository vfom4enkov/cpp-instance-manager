#ifndef CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_3_H_
#define CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_3_H_

#include <cstdint>

namespace cpptoolkit {
namespace factory {
class MockUnitLevel_3 {
 public:
  MockUnitLevel_3();
  ~MockUnitLevel_3();
  uintptr_t getMyPtr();
  void EmptyMethod(){};
  static int32_t getConstructorCounter();
  static int32_t getDestructorCounter();

 public:
  static int32_t constructor_counter_;
  static int32_t destructor_counter_;
  static void reset();
};
}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_3_H_
