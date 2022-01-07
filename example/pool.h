#ifndef EXAMPLE_POOL_H_
#define EXAMPLE_POOL_H_

#include "abstract.h"

namespace example {
class Pool {
 public:
  Pool(Abstract* abstract) noexcept;
  ~Pool() noexcept;
  void Action() const noexcept;

 private:
  Abstract* abstract_;
};

}  // namespace example

#endif  // EXAMPLE_POOL_H_
