#ifndef EXAMPLE_ABSTRACT_H_
#define EXAMPLE_ABSTRACT_H_

namespace example {

class Abstract {
 public:
  Abstract() noexcept = default;
  virtual ~Abstract() noexcept = default;
  virtual void Action() noexcept = 0;
};

}  // namespace example

#endif  // EXAMPLE_ABSTRACT_H_
