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

#include "fixture.h"

#include <cpptoolkit/factory/builder.h>

#include <iostream>
#include <mutex>

namespace cpptoolkit {
namespace factory {
namespace engine {

std::mutex Fixture::mutex_;

Fixture::Fixture() : core_u_ptr_(nullptr) {
  mutex_.lock();

  MockUnitLevel_1::reset();
  MockUnitLevel_2::reset();
  MockUnitLevel_3::reset();
  MockUnitSingleInstance::reset();
  MockUnitSingleInstanceTop::reset();

  RegisterMockUnits();
}

Fixture::~Fixture() { mutex_.unlock(); }

void Fixture::RegisterMockUnits() {
  cf::Builder builder;
  builder.RegisterType<MockUnitLevel_3>();
  builder
      .Register<MockUnitLevel_2>([](cf::Resolver& resolver) -> MockUnitLevel_2* {
        return Create<MockUnitLevel_2_A>(resolver.Get<MockUnitLevel_3>());
      })
      .SetKey("A");
  builder
      .Register<MockUnitLevel_2>([](cf::Resolver& resolver) -> MockUnitLevel_2* {
        return Create<MockUnitLevel_2_B>();
      })
      .SetKey("B");
  builder.Register<MockUnitLevel_1>([](cf::Resolver& resolver) -> MockUnitLevel_1* {
    return Create<MockUnitLevel_1>(resolver.Get<MockUnitLevel_2>("A"),
                                   resolver.Get<MockUnitLevel_2>("B"));
  });
  builder
      .Register<MockUnitSingleInstance>(
          [](cf::Resolver& resolver) -> MockUnitSingleInstance* {
            return Create<MockUnitSingleInstance>(
                resolver.Get<MockUnitLevel_3>());
          })
      .AsSingleInstance();
  builder
      .Register<MockUnitSingleInstanceTop>(
          [](cf::Resolver& resolver) -> MockUnitSingleInstanceTop* {
            return Create<MockUnitSingleInstanceTop>(
                resolver.Get<MockUnitLevel_1>(),
                resolver.Get<MockUnitSingleInstance>());
          })
      .AsSingleInstance();
  builder.Register<MockUnitReturnNullOncreate>(
      [](cf::Resolver& resolver) -> MockUnitReturnNullOncreate* {
        return nullptr;
      });
  builder.Register<MockUnitThrowExceptionOncreate>(
      [](cf::Resolver& resolver) -> MockUnitThrowExceptionOncreate* {
        throw std::runtime_error(
            "Exception created on operation create "
            "MockUnitThrowExceptionOncreate");
      });
  builder.Register<MockUnitThrowStrOncreate>(
      [](cf::Resolver& resolver) -> MockUnitThrowStrOncreate* {
        throw "String created on operation create MockUnitThrowExceptionOncreate";
      });
  core_u_ptr_ = builder.BuildUnique();
  core_ = core_u_ptr_.get();
  if (core_ == nullptr) {
    std::cout << builder.Error() << std::endl;
  }
}

}  // namespace engine
}  // namespace factory
}  // namespace cpptoolkit
