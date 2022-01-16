# CppToolKit-Factory
The tool provides control of lifetime for business logic C++ objects.

## How to use

### Add the tool to your project
Download the tool and add the path to `src` to the `PATH`. Or if you use `cmake` just add the line to your `CMakeLists.txt`:
```
include_directories({pat_to_cpptoolkit-factory}/src)
```

### Register objects
If an object without dependencies:
```
#include <cpptoolkit/factory/builder.h>
...
namespace cf = cpptoolkit::factory;
...
cf::Builder builder;
RegisterType<my_object>();
```

Object with dependencies: