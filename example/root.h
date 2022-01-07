#ifndef EXAMPLE_ROOT_H_
#define EXAMPLE_ROOT_H_

#include "pool.h"
#include "single.h"

namespace example {

class Root {
 public:
  Root(Pool* pool, Single* single) noexcept;
  ~Root() noexcept;
  void Action() const noexcept;

 private:
  Pool* pool_;
  Single* single_;
};

}  // namespace example

#endif  // EXAMPLE_ROOT_H_
