#ifndef CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_2_H_
#define CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_2_H_

#include <cstdint>

#include "mock_unit_level_3.h"

namespace cpptoolkit {
namespace factory {

class MockUnitLevel_2 {
 public:
  MockUnitLevel_2() { constructor_counter_++; }
  ~MockUnitLevel_2() { destructor_counter_++; }
  virtual uintptr_t getMyPtr() = 0;
  static int32_t getConstructorCounter();
  static int32_t getDestructorCounter();

 public:
  static int32_t constructor_counter_;
  static int32_t destructor_counter_;
  static void reset();
};

// First implementation of Middle class
class MockUnitLevel_2_A : public MockUnitLevel_2 {
 public:
  MockUnitLevel_2_A(MockUnitLevel_3* junior)
      : MockUnitLevel_2(), junior_(junior){};
  uintptr_t getMyPtr() override;

 private:
  MockUnitLevel_3* junior_;
};

// Second implementation of Middle class
class MockUnitLevel_2_B : public MockUnitLevel_2 {
 public:
  MockUnitLevel_2_B() : MockUnitLevel_2(){};
  uintptr_t getMyPtr() override;
};
}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_2_H_
