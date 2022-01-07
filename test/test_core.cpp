#include "test_core.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestCore)

BOOST_FIXTURE_TEST_CASE(test_core_extension_add_manager_normal_case, Fixture) {
  // arrange
  CoreExtension core;
  std::unique_ptr<MultipleInstanceManager<MockUnitLevel_3>> manager =
      MakeUnique<MultipleInstanceManager<MockUnitLevel_3>>(
          "MockUnitLevel_3",
          [](Resolver& resolver) -> MockUnitLevel_3* {
            return new MockUnitLevel_3();
          },
          &core);

  // act
  bool result = core.Add(std::move(manager));

  // assert
  BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(test_core_extension_add_manager_twice, Fixture) {
  // arrange
  CoreExtension core;
  std::unique_ptr<MultipleInstanceManager<MockUnitLevel_3>> manager =
      MakeUnique<MultipleInstanceManager<MockUnitLevel_3>>(
          "MockUnitLevel_3",
          [](Resolver& resolver) -> MockUnitLevel_3* {
            return new MockUnitLevel_3();
          },
          &core);

  std::unique_ptr<MultipleInstanceManager<MockUnitLevel_3>> manager_2 =
      MakeUnique<MultipleInstanceManager<MockUnitLevel_3>>(
          "MockUnitLevel_3",
          [](Resolver& resolver) -> MockUnitLevel_3* {
            return new MockUnitLevel_3();
          },
          &core);

  // act
  bool result = core.Add(std::move(manager));
  bool result_2 = core.Add(std::move(manager_2));

  // assert
  BOOST_CHECK(result);
  BOOST_CHECK(!result_2);
  BOOST_CHECK(!core.LastError().empty());
}

BOOST_FIXTURE_TEST_CASE(test_core_get_instance_normal_case, Fixture) {
  // arrange and act
  std::unique_ptr<BaseContext<MockUnitLevel_1>> ptr_holder =
      core_->Get<MockUnitLevel_1>();

  // assert
  BOOST_CHECK(ptr_holder->IsValid());
  BOOST_CHECK(ptr_holder->GetInstance() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_core_check_add_manager_to_manager_index, Fixture) {
  // arrange
  std::string error;
  CoreExtension core;
  BOOST_CHECK_EQUAL(0, core.index_.size());
  std::unique_ptr<MultipleInstanceManager<MockUnitLevel_3>> manager =
      MakeUnique<MultipleInstanceManager<MockUnitLevel_3>>(
          DEFAULT_KEY,
          [](Resolver& resolver) -> MockUnitLevel_3* {
            return new MockUnitLevel_3();
          },
          &core);

  // act
  bool operation_result = core.Add(std::move(manager));
  error = core.LastError();

  // assert
  BOOST_CHECK(operation_result);
  BOOST_CHECK(error.empty());
  BOOST_CHECK_EQUAL(1, core.index_.size());
}

BOOST_FIXTURE_TEST_CASE(test_core_try_to_register_type_twice, Fixture) {
  // arrange
  std::string error;
  CoreExtension core;
  std::unique_ptr<MultipleInstanceManager<MockUnitLevel_3>> manager_1 =
      MakeUnique<MultipleInstanceManager<MockUnitLevel_3>>(
          DEFAULT_KEY,
          [](Resolver& resolver) -> MockUnitLevel_3* {
            return new MockUnitLevel_3();
          },
          &core);
  bool operation_result = core.Add(std::move(manager_1));
  error = core.LastError();
  BOOST_CHECK(operation_result);
  BOOST_CHECK(error.empty());

  // act
  std::unique_ptr<MultipleInstanceManager<MockUnitLevel_3>> manager_2 =
      MakeUnique<MultipleInstanceManager<MockUnitLevel_3>>(
          DEFAULT_KEY,
          [](Resolver& resolver) -> MockUnitLevel_3* {
            return new MockUnitLevel_3();
          },
          &core);
  operation_result = core.Add(std::move(manager_2));
  error = core.LastError();

  // assert
  BOOST_CHECK(!operation_result);
  BOOST_CHECK(!error.empty());
}

BOOST_FIXTURE_TEST_CASE(test_core_unit_constructor_and_destructor_counts,
                        Fixture) {
  // arrange and act
  std::unique_ptr<BaseContext<MockUnitLevel_1>> ptr_holder =
      core_->Get<MockUnitLevel_1>();
  BOOST_CHECK(ptr_holder->IsValid());
  BOOST_CHECK(ptr_holder->GetInstance() != nullptr);
  ptr_holder.reset();

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_1::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_1::getDestructorCounter());
  BOOST_CHECK_EQUAL(2, MockUnitLevel_2::getConstructorCounter());
  BOOST_CHECK_EQUAL(2, MockUnitLevel_2::getDestructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

BOOST_FIXTURE_TEST_CASE(
    test_core_get_instance_if_instance_manager_not_registered, Fixture) {
  // arrange and act
  std::unique_ptr<BaseContext<MockUnitNotRegistered>> ptr_holder =
      core_->Get<MockUnitNotRegistered>();

  // assert
  BOOST_CHECK(!ptr_holder->IsValid());
  BOOST_CHECK(!ptr_holder->Error().empty());
  BOOST_CHECK(ptr_holder->GetInstance() == nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_core_get_instance_if_instance_manager_return_error,
                        Fixture) {
  // arrange and act
  std::unique_ptr<BaseContext<MockUnitThrowExceptionOncreate>> ptr_holder =
      core_->Get<MockUnitThrowExceptionOncreate>();

  // assert
  BOOST_CHECK(!ptr_holder->IsValid());
  BOOST_CHECK(!ptr_holder->Error().empty());
  BOOST_CHECK(ptr_holder->GetInstance() == nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_core_get_single_instance_twice, Fixture) {
  // arrange
  std::unique_ptr<BaseContext<MockUnitSingleInstance>> ptr_holder =
      core_->Get<MockUnitSingleInstance>();
  BOOST_CHECK_EQUAL(1, MockUnitSingleInstance::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());

  // act
  std::unique_ptr<BaseContext<MockUnitSingleInstance>> ptr_holder_2 =
      core_->Get<MockUnitSingleInstance>();
  BOOST_CHECK_EQUAL(1, MockUnitSingleInstance::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());

  // assert
  BOOST_CHECK_EQUAL(ptr_holder->GetInstance()->getMyPtr(),
                    ptr_holder_2->GetInstance()->getMyPtr());
}

BOOST_FIXTURE_TEST_CASE(test_core_get_unique_ptr, Fixture) {
  // arrange
  std::unique_ptr<MockUnitLevel_1, Deleter<MockUnitLevel_1>> uptr(
      nullptr, Deleter<MockUnitLevel_1>(nullptr));

  // act
  {
    uptr = core_->GetUnique<MockUnitLevel_1>(DEFAULT_KEY);
    uintptr_t ptr_value = uptr->getMyPtr();
    BOOST_CHECK(ptr_value);
  }
  uptr.reset();

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_1::getDestructorCounter());
  BOOST_CHECK_EQUAL(2, MockUnitLevel_2::getDestructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

BOOST_FIXTURE_TEST_CASE(test_core_get_shared_ptr, Fixture) {
  // arrange
  std::shared_ptr<MockUnitLevel_1> sptr(nullptr);

  // act
  {
    sptr = core_->GetShared<MockUnitLevel_1>(DEFAULT_KEY);
    uintptr_t ptr_value = sptr->getMyPtr();
    BOOST_CHECK(ptr_value);
  }
  sptr.reset();

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_1::getDestructorCounter());
  BOOST_CHECK_EQUAL(2, MockUnitLevel_2::getDestructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
