#include "abstract_default.h"

#include <iostream>

namespace example {
AbstractDefault::AbstractDefault() noexcept {
  std::cout << "AbstractDefault::Constructor" << std::endl;
}

AbstractDefault::~AbstractDefault() noexcept {
  std::cout << "AbstractDefault::Destructor" << std::endl;
}

void AbstractDefault::Action() noexcept {
  std::cout << "AbstractDefault::Action" << std::endl;
}
}  // namespace example
