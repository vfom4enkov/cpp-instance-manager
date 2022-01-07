#include "test_manager.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestLockPoolHelper)

#define LOCK_POOL_INSTANCE_MANAGEG_MACRO                 \
  LockPoolInstanceManager<MockUnitLevel_3> manager(      \
      "MockUnitLevel_3",                                 \
      [](Resolver& resolver) -> MockUnitLevel_3* { \
        return new MockUnitLevel_3();                    \
      },                                                 \
      core_, 2);

BOOST_FIXTURE_TEST_CASE(test_lock_pool_normal_case, Fixture) {
  // arrange
  LOCK_POOL_INSTANCE_MANAGEG_MACRO

  // act
  {
    BOOST_CHECK_EQUAL(2, manager.countdown_);
    std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder_1 = manager.Get();
    BOOST_CHECK_EQUAL(1, manager.index_.size());
    BOOST_CHECK_EQUAL(1, manager.countdown_);
    std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder_2 = manager.Get();
    BOOST_CHECK_EQUAL(2, manager.index_.size());
  }

  // assert
  BOOST_CHECK_EQUAL(2, manager.queue_.size());
  BOOST_CHECK_EQUAL(0, manager.countdown_);
}

BOOST_FIXTURE_TEST_CASE(test_lock_pool_queue_work, Fixture) {
  // arrange
  LOCK_POOL_INSTANCE_MANAGEG_MACRO

  // act
  uintptr_t ptr = 0;
  {
    {
      std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder_1 =
          manager.Get();
      ptr = ptr_holder_1->GetInstance()->getMyPtr();
      BOOST_CHECK_EQUAL(0, manager.queue_.size());
      // return MockUnitLevel_3 to the pool
    }
    BOOST_CHECK_EQUAL(1, manager.queue_.size());
    std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder_2 = manager.Get();
    BOOST_CHECK_EQUAL(0, manager.queue_.size());
  }
  BOOST_CHECK_EQUAL(1, manager.queue_.size());

  uintptr_t ptr_2 = 0;
  {
    std::unique_ptr<BaseContext<MockUnitLevel_3>> ptr_holder_1 = manager.Get();
    ptr_2 = ptr_holder_1->GetInstance()->getMyPtr();
  }

  BOOST_CHECK(ptr > 0);
  BOOST_CHECK_EQUAL(ptr_2, ptr);
}

BOOST_FIXTURE_TEST_CASE(test_lock_pool_error_on_create, Fixture) {
  // arrange
  uint32_t pool_size = 2;
  LockPoolInstanceManager<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* {
        auto* error_item = resolver.Get<MockUnitThrowExceptionOncreate>();
        return new MockUnitLevel_3();
      },
      core_, pool_size);

  // act
  std::unique_ptr<BaseContext<MockUnitLevel_3>> item = manager.Get();

  // assert
  BOOST_CHECK(!item->IsValid());
  BOOST_CHECK_EQUAL(0, manager.index_.size());
  BOOST_CHECK_EQUAL(0, manager.queue_.size());
  BOOST_CHECK_EQUAL(pool_size, manager.countdown_);
}

#undef LOCK_POOL_INSTANCE_MANAGEG_MACRO

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
