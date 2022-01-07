#ifndef EXAMPLE_ABSTRACT_DEFAULT_H_
#define EXAMPLE_ABSTRACT_DEFAULT_H_

#include "abstract.h"

namespace example {

class AbstractDefault : public Abstract {
 public:
  AbstractDefault() noexcept;
  virtual ~AbstractDefault() noexcept;

  void Action() noexcept override;
};

}  // namespace example

#endif  // EXAMPLE_ABSTRACT_DEFAULT_H_
