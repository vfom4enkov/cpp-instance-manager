#include "root.h"

#include <iostream>

namespace example {
Root::Root(Pool* pool, Single* single) noexcept : pool_(pool), single_(single) {
  std::cout << "Root::Constructor" << std::endl;
}

Root::~Root() noexcept { std::cout << "Root::Destructor" << std::endl; }

void Root::Action() const noexcept {
  std::cout << "Root::Action" << std::endl;
  pool_->Action();
  single_->Action();
}
}  // namespace example
