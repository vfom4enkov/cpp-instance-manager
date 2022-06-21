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

namespace {

template <class T>
class UPtr {
 public:
  UPtr(T* inst) noexcept : inst_(inst){};
  ~UPtr() noexcept;
  UPtr(UPtr<T>&& other) noexcept;

  template <class N, class = typename std::enable_if<
                         std::is_convertible<N*, T*>::value>::type>
  UPtr(UPtr<N>&& other) noexcept {
    inst_ = other.Relese();
  };

  template <class N, class = typename std::enable_if<std::is_convertible<N*, T*>::value>::type>
  UPtr<T>& operator=(UPtr<N>&& other) noexcept {
    inst_ = other.Relese();
  };

  UPtr<T>& operator=(UPtr<T>&& other) noexcept { inst = other.Relese(); }

  // Ban copy and assign RAII operations
  UPtr(const UPtr<T>& other) = delete;
  UPtr<T>& operator=(const UPtr<T>& other) = delete;

  T* Get() noexcept { return inst_; }

  T* Relese() noexcept {
    T* inst = inst_;
    inst_ = nullptr;
    return inst;
  }

 private:
  T* inst_;
};

template<class T>
UPtr<T>::~UPtr() noexcept {
  if (inst_ != nullptr) delete inst_;
}

template<class T>
UPtr<T>::UPtr(UPtr<T>&& other) noexcept {
  inst_ = other.Relese();
}



class A {};

class B : public A {};

class C {};

}  // namespace

int main() {
  UPtr<B> b2(new B());
  UPtr<A> a2(std::move(b2));

  UPtr<B> b(new B());
  UPtr<A> a = std::move(b);

  // error
  //UPtr<C> c(new C());
  //UPtr<A> a3(std::move(c));
  //UPtr<A> a4 = std::move(c);
  return 0;

  std::string error;
  auto core = example::RegisterObjects(error);
  if (!core) {
    std::cout << error << std::endl;
    return -1;
  }

  auto file_logger = core->GetShared<example::FileLogger>();
  auto a_logger = core->GetShared<example::AbstractLogger>("DB_AND_FILE");
  auto a_logger_2 = core->GetUnique<example::AbstractLogger>();
  auto action = core->GetShared<example::Action>();

  if (!action) {
    std::cout << "Error: " << core->LastError();
  }

  {
    const auto action = core->GetShared<example::Action>();
    action->Exec();
  }

  std::cout << std::endl;

  {
    const auto action = core->GetShared<example::Action>("LIGHT");
    action->Exec();
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