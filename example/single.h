#ifndef EXAMPLE_SINGLE_H_
#define EXAMPLE_SINGLE_H_

#include "abstract.h"

namespace example {
class Single {
 public:
  Single(Abstract* abstract) noexcept;
  ~Single() noexcept;
  void Action() const noexcept;

 private:
  Abstract* abstract_;
};

}  // namespace example

#endif  // EXAMPLE_SINGLE_H_
