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

#include "test_manager.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestLockPoolHelper)

#define LOCK_POOL_INSTANCE_MANAGEG_MACRO            \
  LockPoolInstanceManager<MockUnitLevel_3> manager( \
      "MockUnitLevel_3",                            \
      [](Resolver& resolver) -> MockUnitLevel_3* {  \
        return new MockUnitLevel_3();               \
      },                                            \
      core_, 2);

BOOST_FIXTURE_TEST_CASE(test_lock_pool_normal_case, Fixture) {
  // arrange
  LOCK_POOL_INSTANCE_MANAGEG_MACRO

  // act
  {
    BOOST_CHECK_EQUAL(2, manager.countdown_);
    UPtr<BaseContext<MockUnitLevel_3>> ptr_holder_1 = manager.Get();
    BOOST_CHECK_EQUAL(1, manager.index_.size());
    BOOST_CHECK_EQUAL(1, manager.countdown_);
    UPtr<BaseContext<MockUnitLevel_3>> ptr_holder_2 = manager.Get();
    BOOST_CHECK_EQUAL(2, manager.index_.size());
  }

  // assert
  BOOST_CHECK_EQUAL(2, manager.queue_.size());
  BOOST_CHECK_EQUAL(0, manager.countdown_);
}

BOOST_FIXTURE_TEST_CASE(test_lock_pool_queue_work, Fixture) {
  // arrange
  LOCK_POOL_INSTANCE_MANAGEG_MACRO

  // act
  uintptr_t ptr = 0;
  {
    {
      UPtr<BaseContext<MockUnitLevel_3>> ptr_holder_1 =
          manager.Get();
      ptr = ptr_holder_1->GetInstance()->getMyPtr();
      BOOST_CHECK_EQUAL(0, manager.queue_.size());
      // return MockUnitLevel_3 to the pool
    }
    BOOST_CHECK_EQUAL(1, manager.queue_.size());
    UPtr<BaseContext<MockUnitLevel_3>> ptr_holder_2 = manager.Get();
    BOOST_CHECK_EQUAL(0, manager.queue_.size());
  }
  BOOST_CHECK_EQUAL(1, manager.queue_.size());

  uintptr_t ptr_2 = 0;
  {
    UPtr<BaseContext<MockUnitLevel_3>> ptr_holder_1 = manager.Get();
    ptr_2 = ptr_holder_1->GetInstance()->getMyPtr();
  }

  BOOST_CHECK(ptr > 0);
  BOOST_CHECK_EQUAL(ptr_2, ptr);
}

BOOST_FIXTURE_TEST_CASE(test_lock_pool_error_on_create, Fixture) {
  // arrange
  uint32_t pool_size = 2;
  LockPoolInstanceManager<MockUnitLevel_3> manager(
      "MockUnitLevel_3",
      [](Resolver& resolver) -> MockUnitLevel_3* {
        auto* error_item = resolver.Get<MockUnitThrowExceptionOncreate>();
        return new MockUnitLevel_3();
      },
      core_, pool_size);

  // act
  UPtr<BaseContext<MockUnitLevel_3>> item = manager.Get();

  // assert
  BOOST_CHECK(!item->IsValid());
  BOOST_CHECK_EQUAL(0, manager.index_.size());
  BOOST_CHECK_EQUAL(0, manager.queue_.size());
  BOOST_CHECK_EQUAL(pool_size, manager.countdown_);
}

#undef LOCK_POOL_INSTANCE_MANAGEG_MACRO

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
