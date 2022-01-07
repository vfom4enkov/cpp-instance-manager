#include "common.h"

namespace cpptoolkit {
namespace factory {

template <typename T>
using UPtrContext = std::unique_ptr<Context<T>>;

namespace {

UPtrContext<MockUnitLevel_3> GetContext() {
  UPtrContext<MockUnitLevel_3> context(new Context<MockUnitLevel_3>);
  context->SetInstance(new MockUnitLevel_3());
  UPtrContext<MockUnitLevel_2_B> d_context(new Context<MockUnitLevel_2_B>);
  d_context->SetInstance(new MockUnitLevel_2_B());
  context->Add(std::move(d_context));
  return std::move(context);
}

}  // namespace

BOOST_FIXTURE_TEST_CASE(test_deleter_normal_case, Fixture) {
  // arrange
  UPtrContext<MockUnitLevel_3> context = GetContext();

  // act
  { Deleter<MockUnitLevel_3> deleter(std::move(context)); }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_2_B::getDestructorCounter());
}

BOOST_FIXTURE_TEST_CASE(test_deleter_in_unique_ptr, Fixture) {
  // arrange
  UPtrContext<MockUnitLevel_3> context = GetContext();

  // act
  {
    MockUnitLevel_3* ptr = context->GetInstance();
    std::unique_ptr<MockUnitLevel_3, Deleter<MockUnitLevel_3>> uptr(
        ptr, Deleter<MockUnitLevel_3>(std::move(context)));
    uintptr_t ptr_value = uptr->getMyPtr();
    BOOST_CHECK(ptr_value);
  }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_2_B::getDestructorCounter());
}

}  // namespace factory
}  // namespace cpptoolkit
