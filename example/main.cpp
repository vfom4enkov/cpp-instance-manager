/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2021, Vladimir Fomchenkov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cpptoolkit/factory/core.h>

#include <iostream>

#include "root.h"
#include "registrator.h"

namespace cf = cpptoolkit::factory;

int main() {
  std::string error;
  auto core = example::RegisterObjects(error);
  if (!core) {
    std::cout << error << std::endl;
    return -1;
  }

  {
    const auto root = core->GetShared<example::Root>();
    root->Action();
  }

  std::cout << "End program" << std::endl;
}

//	AbstractDefault::Constructor
//	Single::Constructor
//	AbstractCustom::Constructor
//	Pool::Constructor
//	Root::Constructor
//	Root::Action
//	Pool::Action
//	AbstractCustom::Action
//	Single::Action
//	AbstractDefault::Action
//	Root::Destructor
//	End the program
//	Single::Destructor
//	AbstractDefault::Destructor
//	Pool::Destructor
//	AbstractCustom::Destructor
