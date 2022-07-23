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
namespace engine {

BOOST_AUTO_TEST_SUITE(TestPtrHolder)

BOOST_FIXTURE_TEST_CASE(test_ptr_holder_delete_instance, Fixture) {
  // arrange
  {
    PtrHolder<MockUnitLevel_3> uptr(new (std::nothrow) MockUnitLevel_3());
    BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
    BOOST_CHECK_EQUAL(0, MockUnitLevel_3::getDestructorCounter());
    // act: delete object at end of the block
  }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

BOOST_FIXTURE_TEST_CASE(test_ptr_holder_move_ctor, Fixture) {
  // arrange
  PtrHolder<MockUnitLevel_3> uptr(new (std::nothrow) MockUnitLevel_3());

  // act
  PtrHolder<MockUnitLevel_3> uptr_2(std::move(uptr));

  // assert
  BOOST_CHECK_EQUAL(nullptr, uptr.Get());
  BOOST_CHECK(uptr_2.Get() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_ptr_holder_move_assign, Fixture) {
  // arrange
  PtrHolder<MockUnitLevel_3> uptr(new (std::nothrow) MockUnitLevel_3());

  // act
  PtrHolder<MockUnitLevel_3> uptr_2(nullptr);
  uptr_2 = std::move(uptr);

  // assert
  BOOST_CHECK_EQUAL(nullptr, uptr.Get());
  BOOST_CHECK(uptr_2.Get() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_ptr_holder_move_ctor_for_inherited_object,
                        Fixture) {
  // arrange
  PtrHolder<MockUnitLevel_2_B> uptr(new (std::nothrow) MockUnitLevel_2_B());

  // act
  PtrHolder<MockUnitLevel_2> uptr_2(std::move(uptr));

  // assert
  BOOST_CHECK_EQUAL(nullptr, uptr.Get());
  BOOST_CHECK(uptr_2.Get() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_ptr_holder_move_assign_for_inherited_object,
                        Fixture) {
  // arrange
  PtrHolder<MockUnitLevel_2_B> uptr(new (std::nothrow) MockUnitLevel_2_B());

  // act
  PtrHolder<MockUnitLevel_2> uptr_2(nullptr);
  uptr_2 = std::move(uptr);

  // assert
  BOOST_CHECK_EQUAL(nullptr, uptr.Get());
  BOOST_CHECK(uptr_2.Get() != nullptr);
}

BOOST_FIXTURE_TEST_CASE(test_ptr_holder_move_to_vector, Fixture) {
  // arrange
  std::vector<PtrHolder<MockUnitLevel_2>> vector;

  // act
  {
    PtrHolder<MockUnitLevel_2_B> uptr(new (std::nothrow) MockUnitLevel_2_B());
    vector.push_back(std::move(uptr));
  }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_2::getConstructorCounter());
  BOOST_CHECK_EQUAL(0, MockUnitLevel_2::getDestructorCounter());
  BOOST_CHECK_EQUAL(1, vector.size());
}

BOOST_FIXTURE_TEST_CASE(test_ptr_holder_access_operator, Fixture) {
  // arrange
  PtrHolder<MockUnitLevel_2_B> uptr(new (std::nothrow) MockUnitLevel_2_B());

  // act
  uintptr_t ptr_value = uptr->getMyPtr();

  // assert
  BOOST_CHECK(ptr_value != 0);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace engine
}  // namespace factory
}  // namespace cpptoolkit
