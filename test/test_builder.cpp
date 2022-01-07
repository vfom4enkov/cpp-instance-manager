#include "test_builder.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestBuilder)

BOOST_AUTO_TEST_CASE(test_builder_normal_case) {
  // arrange
  Builder builder;
  builder.RegisterType<MockUnitLevel_3>();

  // act
  std::unique_ptr<Core> uptr = builder.Build();
  Core* core = uptr.get();

  // assert
  BOOST_CHECK(core);
  BOOST_CHECK_EQUAL(1, core->index_.size());
}

BOOST_AUTO_TEST_CASE(test_builder_clear_all_registered_objects_after_build) {
  // arrange
  Builder builder;
  builder.RegisterType<MockUnitLevel_3>();

  // act
  builder.Build();

  // assert
  BOOST_CHECK_EQUAL(0, builder.items_.size());
}

BOOST_AUTO_TEST_CASE(test_builder_call_build_twice) {
  // arrange
  Builder builder;
  builder.RegisterType<MockUnitLevel_3>();
  std::unique_ptr<Core> uptr_1 = builder.Build();
  Core* core = uptr_1.get();
  BOOST_CHECK(core);

  // act
  std::unique_ptr<Core> uptr_2 = builder.Build();
  core = uptr_2.get();

  // assert
  BOOST_CHECK(!core);
  BOOST_CHECK(!builder.Error().empty());
}

BOOST_AUTO_TEST_CASE(test_builder_error_transfer) {
  // arrange
  Builder builder;
  builder.RegisterType<MockUnitLevel_3>();
  builder.RegisterType<MockUnitLevel_3>();  // register one type twice

  // act
  std::unique_ptr<Core> uptr = builder.Build();
  Core* core = uptr.get();

  // assert
  BOOST_CHECK(core == nullptr);
  BOOST_CHECK(!builder.Error().empty());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
