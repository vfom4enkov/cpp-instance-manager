#include "test_manager.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestSingleInstanceManager)

template <typename T>
using UPtrBContext = std::unique_ptr<BaseContext<T>>;

#define SINGLE_INSTANCE_MANAGEG_MACRO                    \
  SingleInstanceManager<MockUnitLevel_3> manager(        \
      "MockUnitLevel_3",                                 \
      [](Resolver& resolver) -> MockUnitLevel_3* { \
        return new MockUnitLevel_3();                    \
      },                                                 \
      core_);

BOOST_FIXTURE_TEST_CASE(test_single_instance_manager_normal_case, Fixture) {
  // arrange
  SINGLE_INSTANCE_MANAGEG_MACRO;

  // act
  UPtrBContext<MockUnitLevel_3> ptr_holder = manager.Get();
  UPtrBContext<MockUnitLevel_3> ptr_holder_2 = manager.Get();

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  uintptr_t instance_uintptr =
      reinterpret_cast<uintptr_t>(ptr_holder->GetInstance());
  uintptr_t instance_uintptr_2 =
      reinterpret_cast<uintptr_t>(ptr_holder_2->GetInstance());
  BOOST_CHECK(instance_uintptr == instance_uintptr_2);
}

BOOST_FIXTURE_TEST_CASE(
    test_single_instance_manager_check_delete_single_instance_on_delete_manager,
    Fixture) {
  {
    // arrange
    SINGLE_INSTANCE_MANAGEG_MACRO;

    // act
    UPtrBContext<MockUnitLevel_3> ptr_holder = manager.Get();
  }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

#undef SINGLE_INSTANCE_MANAGEG_MACRO

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
