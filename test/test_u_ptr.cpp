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

BOOST_AUTO_TEST_SUITE(TestUPtr)

#define CREATE_PTR_HOLDER_MACRO                       \
  PtrHolder<Context<MockUnitLevel_3>> ptrHolder(      \
      new (std::nothrow) Context<MockUnitLevel_3>()); \
  ptrHolder.Get()->SetInstance(new (std::nothrow) MockUnitLevel_3());

BOOST_FIXTURE_TEST_CASE(test_u_ptr_normal_case, Fixture) {
  {
    // arrange and act
    CREATE_PTR_HOLDER_MACRO;
    UPtr<MockUnitLevel_3> uptr(std::move(ptrHolder));

    BOOST_CHECK_EQUAL(true, uptr.IsValid());
    BOOST_CHECK(uptr.Error().empty());
    BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
    BOOST_CHECK_EQUAL(0, MockUnitLevel_3::getDestructorCounter());
  }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

BOOST_FIXTURE_TEST_CASE(test_u_ptr_check_validation_after_move, Fixture) {
  // arrange
  CREATE_PTR_HOLDER_MACRO;
  UPtr<MockUnitLevel_3> uptr(std::move(ptrHolder));

  // act
  UPtr<MockUnitLevel_3> uptr_2 = std::move(uptr);

  // assert
  BOOST_CHECK_EQUAL(false, uptr.IsValid());
  BOOST_CHECK(!uptr.Error().empty());
}

BOOST_FIXTURE_TEST_CASE(test_u_ptr_move_ctor_from_ptr_holder, Fixture) {
  // arrange
  CREATE_PTR_HOLDER_MACRO;

  // act
  UPtr<MockUnitLevel_3> uptr(std::move(ptrHolder));

  // assert
  BOOST_CHECK(ptrHolder.Get() == nullptr);
  BOOST_CHECK(uptr.Get() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_u_ptr_move_ctor_for_inherited_object, Fixture) {
  // arrange
  PtrHolder<Context<MockUnitLevel_2_B>> ptrHolder(
      new (std::nothrow) Context<MockUnitLevel_2_B>());
  ptrHolder.Get()->SetInstance(new (std::nothrow) MockUnitLevel_2_B());
  UPtr<MockUnitLevel_2_B> uptr_src(std::move(ptrHolder));

  // act
  UPtr<MockUnitLevel_2> uptr(std::move(uptr_src));

  // assert
  BOOST_CHECK(uptr_src.Get() == nullptr);
  BOOST_CHECK(uptr_src.context_.Get() == nullptr);
  BOOST_CHECK(uptr.Get() != nullptr);
  BOOST_CHECK(uptr.context_.Get() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_u_ptr_move_assign, Fixture) {
  // arrange
  CREATE_PTR_HOLDER_MACRO;
  UPtr<MockUnitLevel_3> uptr_src(std::move(ptrHolder));
  PtrHolder<BaseContext<MockUnitLevel_3>> ptr_h(new Context<MockUnitLevel_3>());

  // act
  UPtr<MockUnitLevel_3> uptr(std::move(ptr_h));
  uptr = std::move(uptr_src);

  // assert
  BOOST_CHECK(uptr_src.Get() == nullptr);
  BOOST_CHECK(uptr_src.context_.Get() == nullptr);
  BOOST_CHECK(uptr.Get() != nullptr);
  BOOST_CHECK(uptr.context_.Get() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_u_ptr_move_assign_for_inherited_object, Fixture) {
  // arrange
  PtrHolder<Context<MockUnitLevel_2_B>> ptrHolder(
      new (std::nothrow) Context<MockUnitLevel_2_B>());
  ptrHolder.Get()->SetInstance(new (std::nothrow) MockUnitLevel_2_B());
  UPtr<MockUnitLevel_2_B> uptr_src(std::move(ptrHolder));

  PtrHolder<BaseContext<MockUnitLevel_2>> ptr_h(new Context<MockUnitLevel_2>());

  // act
  UPtr<MockUnitLevel_2> uptr(std::move(ptr_h));
  uptr = std::move(uptr_src);

  // assert
  BOOST_CHECK(uptr_src.Get() == nullptr);
  BOOST_CHECK(uptr_src.context_.Get() == nullptr);
  BOOST_CHECK(uptr.Get() != nullptr);
  BOOST_CHECK(uptr.context_.Get() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_u_ptr_access_operator, Fixture) {
  // arrange
  CREATE_PTR_HOLDER_MACRO;
  UPtr<MockUnitLevel_3> uptr_src(std::move(ptrHolder));

  // act
  uintptr_t ptr_value = uptr_src->getMyPtr();

  // assert
  BOOST_CHECK(ptr_value != 0);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
