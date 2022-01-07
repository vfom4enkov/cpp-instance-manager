#ifndef CPP_TOOL_KIT_FACTORY_TEST_FIXTURE_H_
#define CPP_TOOL_KIT_FACTORY_TEST_FIXTURE_H_

#include <cpptoolkit/factory/core.h>

#include <mutex>

#include "mock_unit_error.h"
#include "mock_unit_level_1.h"
#include "mock_unit_level_2.h"
#include "mock_unit_level_3.h"
#include "mock_unit_single_instance.h"
#include "mock_unit_single_instance_top.h"

namespace cpptoolkit {
namespace factory {

class Fixture {
 public:
  Fixture();
  ~Fixture();

 private:
  void RegisterMockUnits();

 private:
  static std::mutex mutex_;
  std::unique_ptr<Core> core_u_ptr_;

 public:
  Core* core_;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_TEST_FIXTURE_H_
