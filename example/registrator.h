#ifndef EXAMPLE_REGISTRATOR_H_
#define EXAMPLE_REGISTRATOR_H_

#include <cpptoolkit/factory/core.h>

namespace example {

std::unique_ptr<cpptoolkit::factory::Core> RegisterObjects(std::string& error);

}  // namespace example

#endif  // EXAMPLE_REGISTRATOR_H_
