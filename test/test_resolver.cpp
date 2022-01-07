#include "common.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestResolver)

BOOST_FIXTURE_TEST_CASE(test_resolver_normal_case, Fixture) {
  // arrange
  Context<MockUnitLevel_3> base_context;
  Resolver dh(core_, &base_context);
  
  // act
  MockUnitLevel_3* item = dh.Get<MockUnitLevel_3>();

  // assert
  BOOST_CHECK(item != nullptr);
  BOOST_CHECK_EQUAL(1, base_context.dependencies_.size());
}

BOOST_FIXTURE_TEST_CASE(resolver_factory_returns_error, Fixture) {
  // arrange
  Context<MockUnitLevel_3> context;
  Resolver helper(core_, &context);

  // act
  BOOST_CHECK(context.Error().empty());
  MockUnitNotRegistered* instance = helper.Get<MockUnitNotRegistered>();

  // assert
  BOOST_CHECK(!context.Error().empty());
  BOOST_CHECK(!context.IsValid());
  BOOST_CHECK_EQUAL(1, context.dependencies_.size());
  BOOST_CHECK_EQUAL(nullptr, instance);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
