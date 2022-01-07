#include "test_manager.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestPoolHelper)

#define POOL_INSTANCE_MANAGEG_MACRO                \
  PoolInstanceManager<MockUnitLevel_3> manager(    \
      "MockUnitLevel_3",                           \
      [](Resolver& resolver) -> MockUnitLevel_3* { \
        return new MockUnitLevel_3();              \
      },                                           \
      core_, 2);

BOOST_FIXTURE_TEST_CASE(test_pool_normal_case, Fixture) {
  // arrange
  POOL_INSTANCE_MANAGEG_MACRO

  // act
  {
    std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder_1 = manager.Get();
    BOOST_CHECK_EQUAL(1, manager.index_.size());
    std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder_2 = manager.Get();
    BOOST_CHECK_EQUAL(2, manager.index_.size());
  }

  // assert
  BOOST_CHECK_EQUAL(2, manager.queue_.size());
}

BOOST_FIXTURE_TEST_CASE(test_pool_queue_work, Fixture) {
  // arrange
  POOL_INSTANCE_MANAGEG_MACRO

  // act
  uintptr_t ptr = 0;
  {
    std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder_1 = manager.Get();
    ptr = ptr_holder_1->GetInstance()->getMyPtr();
    BOOST_CHECK_EQUAL(0, manager.queue_.size());
  }
  BOOST_CHECK_EQUAL(1, manager.queue_.size());

  uintptr_t ptr_2 = 0;
  {
    std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder_1 = manager.Get();
    ptr_2 = ptr_holder_1->GetInstance()->getMyPtr();
    BOOST_CHECK_EQUAL(0, manager.queue_.size());
  }

  BOOST_CHECK(ptr > 0);
  BOOST_CHECK_EQUAL(ptr_2, ptr);
  BOOST_CHECK_EQUAL(1, manager.queue_.size());
}

BOOST_FIXTURE_TEST_CASE(test_pool_error_on_create, Fixture) {
  // arrange
  uint32_t pool_size = 2;
  PoolInstanceManager<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* {
        auto* error_item = resolver.Get<MockUnitNotRegistered>();
        return new MockUnitLevel_3();
      },
      core_, pool_size);

  // act
  std::unique_ptr<BaseContext<MockUnitLevel_3>> item = manager.Get();

  // assert
  BOOST_CHECK(!item->IsValid());
  BOOST_CHECK_EQUAL(0, manager.index_.size());
  BOOST_CHECK_EQUAL(0, manager.queue_.size());
}

#undef POOL_INSTANCE_MANAGEG_MACRO

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
