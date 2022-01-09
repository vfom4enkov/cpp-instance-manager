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
 * this list of conditions and the following disclaimer.
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
