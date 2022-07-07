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

#include "registrator.h"

#include <cpptoolkit/factory/builder.h>

#include "action.h"
#include "log/abstract_logger.h"
#include "log/complex_logger.h"
#include "log/db_logger.h"
#include "log/file_logger.h"
#include "log/net_logger.h"

namespace example {
namespace cf = cpptoolkit::factory;

std::unique_ptr<cf::Core> RegisterObjects(std::string& error) {
  cf::Builder builder;
  builder.RegisterType<DbLogger>().AsLockPoolInstance(10);
  builder.RegisterType<FileLogger>();
  builder.RegisterType<NetLogger>().AsSingleInstance();
  builder
      .Register<AbstractLogger>([](cf::Resolver& resolver) -> AbstractLogger* {
        auto* file_logger = resolver.Get<FileLogger>();
        auto* db_logger = resolver.Get<DbLogger>();
        return cf::Create<ComplexLogger>(file_logger, db_logger);
      })
      .SetKey("DB_AND_FILE");
  builder.Register<AbstractLogger>(
      [](cf::Resolver& resolver) -> AbstractLogger* {
        auto* file_and_db_logger = resolver.Get<AbstractLogger>("DB_AND_FILE");
        auto* net_logger = resolver.Get<NetLogger>();
        return cf::Create<ComplexLogger>(file_and_db_logger, net_logger);
      });

  builder.Register<Action>([](cf::Resolver& resolver) -> Action* {
    auto* logger = resolver.Get<AbstractLogger>();
    return cf::Create<Action>(logger);
  });

  builder
      .Register<Action>([](cf::Resolver& resolver) -> Action* {
        auto* logger = resolver.Get<FileLogger>();
        return cf::Create<Action>(logger);
      })
      .SetKey("LIGHT");

  std::unique_ptr<cf::Core> core = builder.Build();
  if (!core) {
    error = builder.Error();
  }

  return core;
}
}  // namespace example
