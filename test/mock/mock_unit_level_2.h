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

#ifndef CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_2_H_
#define CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_2_H_

#include <cstdint>

#include "mock_unit_level_3.h"

namespace cpptoolkit {
namespace factory {

class MockUnitLevel_2 {
 public:
  MockUnitLevel_2() { constructor_counter_++; }
  ~MockUnitLevel_2() { destructor_counter_++; }
  virtual uintptr_t getMyPtr() = 0;
  static int32_t getConstructorCounter();
  static int32_t getDestructorCounter();

 public:
  static int32_t constructor_counter_;
  static int32_t destructor_counter_;
  static void reset();
};

// First implementation of Middle class
class MockUnitLevel_2_A : public MockUnitLevel_2 {
 public:
  MockUnitLevel_2_A(MockUnitLevel_3* junior)
      : MockUnitLevel_2(), junior_(junior){};
  uintptr_t getMyPtr() override;

 private:
  MockUnitLevel_3* junior_;
};

// Second implementation of Middle class
class MockUnitLevel_2_B : public MockUnitLevel_2 {
 public:
  MockUnitLevel_2_B() : MockUnitLevel_2(){};
  uintptr_t getMyPtr() override;
};
}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_LEVEL_2_H_
