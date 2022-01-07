#ifndef EXAMPLE_ABSTRACT_CUSTOM_H_
#define EXAMPLE_ABSTRACT_CUSTOM_H_

#include "abstract.h"

namespace example {
class AbstractCustom : public Abstract {
 public:
  AbstractCustom() noexcept;
  virtual ~AbstractCustom() noexcept;

  void Action() noexcept override;
};

}  // namespace example

#endif  // EXAMPLE_ABSTRACT_CUSTOM_H_
