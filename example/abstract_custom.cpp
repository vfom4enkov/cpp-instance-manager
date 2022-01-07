#include "abstract_custom.h"

#include <iostream>

namespace example {
AbstractCustom::AbstractCustom() noexcept {
  std::cout << "AbstractCustom::Constructor" << std::endl;
}
AbstractCustom::~AbstractCustom() noexcept {
  std::cout << "AbstractCustom::Destructor" << std::endl;
}
void AbstractCustom::Action() noexcept {
  std::cout << "AbstractCustom::Action" << std::endl;
}
}  // namespace example
