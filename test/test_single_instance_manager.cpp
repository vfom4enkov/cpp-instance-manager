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

#include "test_manager.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestSingleInstanceManager)

template <typename T>
using UPtrBContext = std::unique_ptr<BaseContext<T>>;

#define SINGLE_INSTANCE_MANAGEG_MACRO              \
  SingleInstanceManager<MockUnitLevel_3> manager(  \
      "MockUnitLevel_3",                           \
      [](Resolver& resolver) -> MockUnitLevel_3* { \
        return new MockUnitLevel_3();              \
      },                                           \
      core_);

BOOST_FIXTURE_TEST_CASE(test_single_instance_manager_normal_case, Fixture) {
  // arrange
  SINGLE_INSTANCE_MANAGEG_MACRO;

  // act
  UPtrBContext<MockUnitLevel_3> ptr_holder = manager.Get();
  UPtrBContext<MockUnitLevel_3> ptr_holder_2 = manager.Get();

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  uintptr_t instance_uintptr =
      reinterpret_cast<uintptr_t>(ptr_holder->GetInstance());
  uintptr_t instance_uintptr_2 =
      reinterpret_cast<uintptr_t>(ptr_holder_2->GetInstance());
  BOOST_CHECK(instance_uintptr == instance_uintptr_2);
}

BOOST_FIXTURE_TEST_CASE(
    test_single_instance_manager_check_delete_single_instance_on_delete_manager,
    Fixture) {
  {
    // arrange
    SINGLE_INSTANCE_MANAGEG_MACRO;

    // act
    UPtrBContext<MockUnitLevel_3> ptr_holder = manager.Get();
  }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

#undef SINGLE_INSTANCE_MANAGEG_MACRO

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
