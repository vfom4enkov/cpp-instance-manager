#include "common.h"

namespace cpptoolkit {
namespace factory {

namespace {
// Special object for testing BaseInstanceManager
template <typename T>
class Mock : public BaseInstanceManager<T> {
 public:
  Mock(const std::string class_name_key,
                          std::function<T*(Resolver&)>&& create,
                          Core* core)
      : BaseInstanceManager(class_name_key, std::move(create), core){};

  std::unique_ptr<BaseContext<T>> Get() noexcept override;
};

template <typename T>
std::unique_ptr<BaseContext<T>> Mock<T>::Get() noexcept {
  std::unique_ptr<BaseContext<T>> item(new Context<T>());
  return std::move(item);
}
}

BOOST_AUTO_TEST_SUITE(TestBaseInstanceManager)

BOOST_FIXTURE_TEST_CASE(test_base_instance_manager_normal_case, Fixture) {
  // arrange
  Core core;
  Mock<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* {
        return new MockUnitLevel_3();
      },
      core_);
  Context<MockUnitLevel_3> context;

  // act
  manager.Create(&context);

  // assert
  BOOST_CHECK(context.IsValid());
  BOOST_CHECK(context.GetInstance() != nullptr);
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
}

BOOST_FIXTURE_TEST_CASE(test_base_instance_manager_dependency_helper_has_error,
                        Fixture) {
  // arrange
  Core core;
  Mock<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* {
        MockUnitNotRegistered* item = resolver.Get<MockUnitNotRegistered>();
        return new MockUnitLevel_3();
      },
      core_);
  Context<MockUnitLevel_3> context;

  // act
  manager.Create(&context);

  // assert
  BOOST_CHECK(!context.IsValid());
  BOOST_CHECK(!context.Error().empty());
  BOOST_CHECK(context.GetInstance() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_base_instance_manager_create_returns_null,
                        Fixture) {
  // arrange
  Core core;
  Mock<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* { return nullptr; },
      &core);
  Context<MockUnitLevel_3> context;

  // act
  manager.Create(&context);

  // assert
  BOOST_CHECK(!context.IsValid());
  BOOST_CHECK(!context.Error().empty());
  BOOST_CHECK(context.GetInstance() == nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_base_instance_manager_create_throws_exception,
                        Fixture) {
  // arrange
  Mock<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* {
        throw std::exception("empty_exception");
        return new MockUnitLevel_3();
      },
      core_);
  Context<MockUnitLevel_3> context;

  // act
  manager.Create(&context);

  // assert
  BOOST_CHECK(!context.IsValid());
  BOOST_CHECK(!context.Error().empty());
  BOOST_CHECK_EQUAL(nullptr, context.GetInstance());
}

BOOST_FIXTURE_TEST_CASE(
    test_base_instance_manager_create_throws_exception_if_context_already_has_error,
    Fixture) {
  // arrange
  Mock<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* {
        throw std::exception("empty_exception");
        return new MockUnitLevel_3();
      },
      core_);
  Context<MockUnitLevel_3> context;
  std::string error = "Custom error for the context";
  context.is_valid_ = false;
  context.error_ = error;

  // act
  manager.Create(&context);

  // assert
  BOOST_CHECK(!context.IsValid());
  BOOST_CHECK(!context.Error().empty());
  BOOST_CHECK_EQUAL(nullptr, context.GetInstance());
  BOOST_CHECK_EQUAL(error, context.Error());
}

BOOST_FIXTURE_TEST_CASE(test_base_instance_manager_create_throws_something,
                        Fixture) {
  // arrange
  Mock<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* {
        throw "string_message";
        return new MockUnitLevel_3();
      },
      core_);
  Context<MockUnitLevel_3> context;

  // act
  manager.Create(&context);

  // assert
  BOOST_CHECK(!context.IsValid());
  BOOST_CHECK(!context.Error().empty());
  BOOST_CHECK(context.GetInstance() == nullptr);
}

BOOST_FIXTURE_TEST_CASE(
    test_base_instance_manager_create_throws_something_if_context_already_has_error,
                        Fixture) {
  // arrange
  Mock<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* {
        throw "string_message";
        return new MockUnitLevel_3();
      },
      core_);
  Context<MockUnitLevel_3> context;
  std::string error = "Custom error for the context";
  context.is_valid_ = false;
  context.error_ = error;

  // act
  manager.Create(&context);

  // assert
  BOOST_CHECK(!context.IsValid());
  BOOST_CHECK(!context.Error().empty());
  BOOST_CHECK_EQUAL(nullptr, context.GetInstance());
  BOOST_CHECK_EQUAL(error, context.Error());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
