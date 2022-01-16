# CppToolKit-Factory
Header based tool provides control of lifetime for C++ business logic objects.

## How to use

### Add the tool to your project
Download the tool and add the path to `src` to the `PATH`. Or if you use `cmake` just add the line to your `CMakeLists.txt`:
```
include_directories({pat_to_cpptoolkit-factory}/src)
```

### Type of objects
There are available four types:
- *Single* - an instance created once and used many times
- *Multiple* - (default type) an instance is created every time on request
- *Lock pool* - an instance is created if the pool is not full or the thread is blocked until another instance returns to the pool
- *Soft pool* - an instance is created if the pool is empty. After the instance returns to the pool it is deleted if the pool is full

### Register objects
If an object without dependencies:
```
#include <cpptoolkit/factory/builder.h>
...
cpptoolkit::factory::Builder builder;
RegisterType<my_object>();
```

Object with dependencies:
```
#include <cpptoolkit/factory/builder.h>
...
namespace cf = cpptoolkit::factory;
...
cpptoolkit::factory::Builder builder;
RegisterType<my_object>();
```