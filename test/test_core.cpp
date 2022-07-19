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

#include "test_core.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestCore)

BOOST_FIXTURE_TEST_CASE(test_core_extension_add_manager_normal_case, Fixture) {
  // arrange
  CoreExtension core;
  PtrHolder<MultipleInstanceManager<MockUnitLevel_3>> manager =
      MakePtrHolder<MultipleInstanceManager<MockUnitLevel_3>>(
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
  PtrHolder<MultipleInstanceManager<MockUnitLevel_3>> manager =
      MakePtrHolder<MultipleInstanceManager<MockUnitLevel_3>>(
          "MockUnitLevel_3",
          [](Resolver& resolver) -> MockUnitLevel_3* {
            return new MockUnitLevel_3();
          },
          &core);

  PtrHolder<MultipleInstanceManager<MockUnitLevel_3>> manager_2 =
      MakePtrHolder<MultipleInstanceManager<MockUnitLevel_3>>(
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
  PtrHolder<BaseContext<MockUnitLevel_1>> ptr_holder =
      core_->GetContext<MockUnitLevel_1>();

  // assert
  BOOST_CHECK(ptr_holder->IsValid());
  BOOST_CHECK(ptr_holder->GetInstance() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_core_check_add_manager_to_manager_index, Fixture) {
  // arrange
  std::string error;
  CoreExtension core;
  BOOST_CHECK_EQUAL(0, core.index_.size());
  PtrHolder<MultipleInstanceManager<MockUnitLevel_3>> manager =
      MakePtrHolder<MultipleInstanceManager<MockUnitLevel_3>>(
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
  PtrHolder<MultipleInstanceManager<MockUnitLevel_3>> manager_1 =
      MakePtrHolder<MultipleInstanceManager<MockUnitLevel_3>>(
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
  PtrHolder<MultipleInstanceManager<MockUnitLevel_3>> manager_2 =
      MakePtrHolder<MultipleInstanceManager<MockUnitLevel_3>>(
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

BOOST_FIXTURE_TEST_CASE(test_cleanup_all_dependency_objects_on_main_instance_destroy,
                        Fixture) {
  // arrange and act
  PtrHolder<BaseContext<MockUnitLevel_1>> ptr_holder =
      core_->GetContext<MockUnitLevel_1>();
  BOOST_CHECK(ptr_holder->IsValid());
  BOOST_CHECK(ptr_holder->GetInstance() != nullptr);
  ptr_holder.Reset();

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
  PtrHolder<BaseContext<MockUnitNotRegistered>> ptr_holder =
      core_->GetContext<MockUnitNotRegistered>();

  // assert
  BOOST_CHECK(!ptr_holder->IsValid());
  BOOST_CHECK(!ptr_holder->Error().empty());
  BOOST_CHECK(ptr_holder->GetInstance() == nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_core_get_instance_if_instance_manager_return_error,
                        Fixture) {
  // arrange and act
  PtrHolder<BaseContext<MockUnitThrowExceptionOncreate>> ptr_holder =
      core_->GetContext<MockUnitThrowExceptionOncreate>();

  // assert
  BOOST_CHECK(!ptr_holder->IsValid());
  BOOST_CHECK(!ptr_holder->Error().empty());
  BOOST_CHECK(ptr_holder->GetInstance() == nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_core_get_single_instance_twice, Fixture) {
  // arrange
  PtrHolder<BaseContext<MockUnitSingleInstance>> ptr_holder =
      core_->GetContext<MockUnitSingleInstance>();
  BOOST_CHECK_EQUAL(1, MockUnitSingleInstance::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());

  // act
  PtrHolder<BaseContext<MockUnitSingleInstance>> ptr_holder_2 =
      core_->GetContext<MockUnitSingleInstance>();
  BOOST_CHECK_EQUAL(1, MockUnitSingleInstance::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());

  // assert
  BOOST_CHECK_EQUAL(ptr_holder->GetInstance()->getMyPtr(),
                    ptr_holder_2->GetInstance()->getMyPtr());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
