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

#ifndef CPP_TOOL_KIT_FACTORY_TEST_FIXTURE_H_
#define CPP_TOOL_KIT_FACTORY_TEST_FIXTURE_H_

#include <cpptoolkit/factory/core.h>

#include <mutex>

#include "mock_unit_error.h"
#include "mock_unit_level_1.h"
#include "mock_unit_level_2.h"
#include "mock_unit_level_3.h"
#include "mock_unit_single_instance.h"
#include "mock_unit_single_instance_top.h"

namespace cpptoolkit {
namespace factory {

class Fixture {
 public:
  Fixture();
  ~Fixture();

 private:
  void RegisterMockUnits();

 private:
  static std::mutex mutex_;
  std::unique_ptr<Core> core_u_ptr_;

 public:
  Core* core_;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_TEST_FIXTURE_H_
