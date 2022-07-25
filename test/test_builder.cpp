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

#include "test_builder.h"

namespace cpptoolkit {
namespace factory {

BOOST_AUTO_TEST_SUITE(TestBuilder)

BOOST_AUTO_TEST_CASE(test_builder_normal_case) {
  // arrange
  Builder builder;
  builder.RegisterType<engine::MockUnitLevel_3>();

  // act
  std::unique_ptr<Core> uptr = builder.BuildUnique();
  Core* core = uptr.get();

  // assert
  BOOST_CHECK(core);
  BOOST_CHECK_EQUAL(1, core->index_.size());
}

BOOST_AUTO_TEST_CASE(test_builder_clear_all_registered_objects_after_build) {
  // arrange
  Builder builder;
  builder.RegisterType<engine::MockUnitLevel_3>();

  // act
  builder.BuildShared();

  // assert
  BOOST_CHECK_EQUAL(0, builder.items_.size());
}

BOOST_AUTO_TEST_CASE(test_builder_call_build_twice) {
  // arrange
  Builder builder;
  builder.RegisterType<engine::MockUnitLevel_3>();
  std::unique_ptr<Core> uptr_1 = builder.BuildUnique();
  Core* core = uptr_1.get();
  BOOST_CHECK(core);

  // act
  std::unique_ptr<Core> uptr_2 = builder.BuildUnique();
  core = uptr_2.get();

  // assert
  BOOST_CHECK(!core);
  BOOST_CHECK(!builder.Error().empty());
}

BOOST_AUTO_TEST_CASE(test_builder_error_transfer) {
  // arrange
  Builder builder;
  builder.RegisterType<engine::MockUnitLevel_3>();
  builder.RegisterType<engine::MockUnitLevel_3>();  // register one type twice

  // act
  std::shared_ptr<Core> uptr = builder.BuildShared();
  Core* core = uptr.get();

  // assert
  BOOST_CHECK(core == nullptr);
  BOOST_CHECK(!builder.Error().empty());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
