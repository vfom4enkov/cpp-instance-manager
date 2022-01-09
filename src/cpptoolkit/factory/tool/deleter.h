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

#ifndef CPP_TOOL_KIT_FACTORY_DELETER_H_
#define CPP_TOOL_KIT_FACTORY_DELETER_H_

namespace cpptoolkit {
namespace factory {

class AContext;

/// @brief Helper for delete instance in std::unique_ptr
/// @tparam T - Type of managed object
template <typename T>
class Deleter {
 public:
  /// @brief Create deleter
  /// @param [in] context - Basic information about managed objec
  Deleter(std::unique_ptr<AContext>&& context) noexcept
      : context_(std::move(context)){};

  /// @brief Handler for delete operation of managed object
  /// @param [in] inst - Pointer to instance of managed object
  void operator()(T* inst) noexcept {
    context_.reset();
  };

 private:
  std::unique_ptr<AContext> context_;
};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_DELETER_H_
