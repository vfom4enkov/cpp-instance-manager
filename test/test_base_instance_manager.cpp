/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2021, Vladimir Fomchenkov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
