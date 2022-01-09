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

#include "test_build_item.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestBuildItem)

BOOST_AUTO_TEST_CASE(test_build_item_normal_case) {
  // arrange
  CoreExtension core;
  BuildItem<MockUnitLevel_3> item([](Resolver& resolver) -> MockUnitLevel_3* {
    return new MockUnitLevel_3();
  });

  // act
  bool result = item.Build(&core);

  // assert
  BOOST_CHECK(result);
  BOOST_CHECK_EQUAL(1, core.index_.size());
}

BOOST_AUTO_TEST_CASE(test_build_item_set_count_option_single_instance) {
  // arrange
  CoreExtension core;
  BuildItem<MockUnitLevel_3> single_item(
      [](Resolver& resolver) -> MockUnitLevel_3* {
        return new MockUnitLevel_3();
      });

  // act
  single_item.AsSingleInstance();
  bool result = single_item.Build(&core);

  // assert
  BOOST_CHECK(result);
  BOOST_CHECK_EQUAL(1, core.index_.size());
  for (auto& it : core.index_) {
    std::unique_ptr<AInstanceManager>& u_ptr = it.second;
    auto manager =
        dynamic_cast<SingleInstanceManager<MockUnitLevel_3>*>(u_ptr.get());
    BOOST_CHECK(manager != nullptr);
  }
}

BOOST_AUTO_TEST_CASE(test_build_item_set_count_option_multiple_instance) {
  // arrange
  CoreExtension core;
  BuildItem<MockUnitLevel_3> single_item(
      [](Resolver& resolver) -> MockUnitLevel_3* {
        return new MockUnitLevel_3();
      });

  // act
  single_item.AsMultipleInstance();
  bool result = single_item.Build(&core);

  // assert
  BOOST_CHECK(result);
  BOOST_CHECK_EQUAL(1, core.index_.size());
  for (auto& it : core.index_) {
    std::unique_ptr<AInstanceManager>& u_ptr = it.second;
    auto manager =
        dynamic_cast<MultipleInstanceManager<MockUnitLevel_3>*>(u_ptr.get());
    BOOST_CHECK(manager != nullptr);
  }
}

BOOST_AUTO_TEST_CASE(test_build_item_set_count_option_lock_pool_instance) {
  // arrange
  uint32_t pool_size = 3;
  CoreExtension core;
  BuildItem<MockUnitLevel_3> lock_pool_item(
      [](Resolver& resolver) -> MockUnitLevel_3* {
        return new MockUnitLevel_3();
      });

  // act
  lock_pool_item.AsLockPoolInstance(pool_size);
  bool result = lock_pool_item.Build(&core);

  // assert
  BOOST_CHECK(result);
  BOOST_CHECK_EQUAL(1, core.index_.size());
  for (auto& it : core.index_) {
    std::unique_ptr<AInstanceManager>& u_ptr = it.second;
    auto manager =
        dynamic_cast<LockPoolInstanceManager<MockUnitLevel_3>*>(u_ptr.get());
    BOOST_CHECK(manager != nullptr);
    BOOST_CHECK_EQUAL(pool_size, manager->countdown_);
  }
}

BOOST_AUTO_TEST_CASE(test_build_item_set_count_option_pool_instance) {
  // arrange
  uint32_t pool_size = 3;
  CoreExtension core;
  BuildItem<MockUnitLevel_3> pool_item(
      [](Resolver& resolver) -> MockUnitLevel_3* {
        return new MockUnitLevel_3();
      });

  // act
  pool_item.AsPoolInstance(pool_size);
  bool result = pool_item.Build(&core);

  // assert
  BOOST_CHECK(result);
  BOOST_CHECK_EQUAL(1, core.index_.size());
  for (auto& it : core.index_) {
    std::unique_ptr<AInstanceManager>& u_ptr = it.second;
    auto manager =
        dynamic_cast<SoftPoolInstanceManager<MockUnitLevel_3>*>(u_ptr.get());
    BOOST_CHECK(manager != nullptr);
    BOOST_CHECK_EQUAL(pool_size, manager->size_);
  }
}

BOOST_AUTO_TEST_CASE(test_build_item_set_zero_count_option_lock_pool_instance) {
  // arrange
  uint32_t pool_size = 0;
  CoreExtension core;
  BuildItem<MockUnitLevel_3> lock_pool_item(
      [](Resolver& resolver) -> MockUnitLevel_3* {
        return new MockUnitLevel_3();
      });

  // act
  lock_pool_item.AsLockPoolInstance(pool_size);
  bool result = lock_pool_item.Build(&core);

  // assert
  BOOST_CHECK(!result);
  BOOST_CHECK(!lock_pool_item.Error().empty());
  BOOST_CHECK_EQUAL(0, core.index_.size());
}

BOOST_AUTO_TEST_CASE(test_build_item_set_zero_count_option_pool_instance) {
  // arrange
  uint32_t pool_size = 0;
  CoreExtension core;
  BuildItem<MockUnitLevel_3> pool_item(
      [](Resolver& resolver) -> MockUnitLevel_3* {
        return new MockUnitLevel_3();
      });

  // act
  pool_item.AsPoolInstance(pool_size);
  bool result = pool_item.Build(&core);

  // assert
  BOOST_CHECK(!result);
  BOOST_CHECK(!pool_item.Error().empty());
  BOOST_CHECK_EQUAL(0, core.index_.size());
}

BOOST_AUTO_TEST_CASE(test_build_item_set_empty_key) {
  // arrange
  CoreExtension core;
  BuildItem<MockUnitLevel_3> item([](Resolver& resolver) -> MockUnitLevel_3* {
    return new MockUnitLevel_3();
  });

  // act
  item.SetKey("");
  bool result = item.Build(&core);

  // assert
  BOOST_CHECK(!result);
  BOOST_CHECK(!item.Error().empty());
  BOOST_CHECK_EQUAL(0, core.index_.size());
}

BOOST_AUTO_TEST_CASE(test_build_item_register_one_type_twice) {
  // arrange
  CoreExtension core;
  BuildItem<MockUnitLevel_3> item_1([](Resolver& resolver) -> MockUnitLevel_3* {
    return new MockUnitLevel_3();
  });
  BuildItem<MockUnitLevel_3> item_2([](Resolver& resolver) -> MockUnitLevel_3* {
    return new MockUnitLevel_3();
  });
  // act
  bool result = item_1.Build(&core);
  BOOST_CHECK(result);
  result = item_2.Build(&core);

  // assert
  BOOST_CHECK(!result);
  BOOST_CHECK(!item_2.Error().empty());
  BOOST_CHECK_EQUAL(1, core.index_.size());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
