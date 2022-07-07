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

#include <cpptoolkit/factory/core.h>

#include <iostream>

#include "action.h"
#include "log/file_logger.h"
#include "registrator.h"

namespace cf = cpptoolkit::factory;

int main() {
  std::string error;
  auto core = example::RegisterObjects(error);
  if (!core) {
    std::cout << error << std::endl;
    return -1;
  }

  auto file_logger = core->Get<example::FileLogger>();
  auto a_logger = core->Get<example::AbstractLogger>("DB_AND_FILE");
  auto a_logger_2 = core->Get<example::AbstractLogger>();
  auto action = core->Get<example::Action>();

  if (!action->IsValid()) {
    std::cout << "Error: " << action->Error();
  }

  {
    const auto action = core->Get<example::Action>();
    action->GetInstance()->Exec();
  }

  std::cout << std::endl;

  {
    const auto action = core->Get<example::Action>("LIGHT");
    action->GetInstance()->Exec();
  }

  std::cout << "End program" << std::endl;
}

// Save message(Start action) to file
// Save message(Start action) to db
// Send message(Start action) over network
// Save message(Stop action) to file
// Save message(Stop action) to db
// Send message(Stop action) over network
//
// Save message(Start action) to file
// Save message(Stop action) to file
// End program