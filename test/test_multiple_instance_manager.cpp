#include "test_manager.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestMultipleInstanceManager)

#define MULTIPLE_INSTANCE_MANAGEG_MACRO             \
  MultipleInstanceManager<MockUnitLevel_3> manager( \
      "MockUnitLevel_3",                            \
      [](Resolver& resolver) -> MockUnitLevel_3* {  \
        return new MockUnitLevel_3();               \
      },                                            \
      core_);

BOOST_FIXTURE_TEST_CASE(test_multiple_instance_manager_normal_case, Fixture) {
  // arrange
  MULTIPLE_INSTANCE_MANAGEG_MACRO;

  // act
  std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder = manager.Get();

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK(ptr_holder->IsValid());
}

BOOST_FIXTURE_TEST_CASE(test_multiple_instance_manager_check_delete_instance,
                        Fixture) {
  // arrange
  MULTIPLE_INSTANCE_MANAGEG_MACRO;
  {
    // act
    std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder = manager.Get();
    BOOST_CHECK(ptr_holder->IsValid());
  }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

#undef MULTIPLE_INSTANCE_MANAGEG_MACRO

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
