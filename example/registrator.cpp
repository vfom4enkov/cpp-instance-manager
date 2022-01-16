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

#include "abstract_custom.h"
#include "abstract_default.h"
#include "pool.h"
#include "root.h"
#include "single.h"

namespace example {
namespace cf = cpptoolkit::factory;

std::unique_ptr<cf::Core> RegisterObjects(std::string& error) {
  cf::Builder builder;
  builder
      .Register<Abstract>([](cf::Resolver& resolver) -> Abstract* {
        return cf::Create<AbstractCustom>();
      })
      .AsMultipleInstance()
      .SetKey("Custom");

  builder
      .Register<Abstract>([](cf::Resolver& resolver) -> Abstract* {
        return cf::Create<AbstractDefault>();
      })
      .AsMultipleInstance();

  builder
      .Register<Pool>([](cf::Resolver& resolver) -> Pool* {
        return cf::Create<Pool>(resolver.Get<Abstract>("Custom"));
      })
      .AsLockPoolInstance(2);

  builder
      .Register<Single>([](cf::Resolver& resolver) -> Single* {
        return cf::Create<Single>(resolver.Get<Abstract>());
      })
      .AsSingleInstance();

  builder
      .Register<Root>([](cf::Resolver& resolver) -> Root* {
        return cf::Create<Root>(resolver.Get<Pool>(), resolver.Get<Single>());
      })
      .AsMultipleInstance();

  std::unique_ptr<cf::Core> core = builder.Build();
  if (!core) {
    error = builder.Error();
  }

  return core;
}
}  // namespace example
