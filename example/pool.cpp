#include "pool.h"

#include <iostream>

namespace example {
Pool::Pool(Abstract* abstract) noexcept : abstract_(abstract) {
  std::cout << "Pool::Constructor" << std::endl;
}

Pool::~Pool() noexcept { std::cout << "Pool::Destructor" << std::endl; }

void Pool::Action() const noexcept {
  std::cout << "Pool::Action" << std::endl;
  abstract_->Action();
}
}  // namespace example
