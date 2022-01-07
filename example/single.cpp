#include "single.h"

#include <iostream>

namespace example {
Single::Single(Abstract* abstract) noexcept : abstract_(abstract) {
  std::cout << "Single::Constructor" << std::endl;
}

Single::~Single() noexcept { std::cout << "Single::Destructor" << std::endl; }

void Single::Action() const noexcept {
  std::cout << "Single::Action" << std::endl;
  abstract_->Action();
}
}  // namespace example
