# CppToolKit-Factory
Header based tool provides control of lifetime for C++ business logic objects.

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
cpptoolkit::factory::Builder builder;
RegisterType<my_class>();
```

Object with dependencies:
```
#include <cpptoolkit/factory/builder.h>
...
namespace cf = cpptoolkit::factory;
cf::Builder builder;
builder
  .Register<my_object>([](cf::Resolver& resolver) -> my_object* {
    auto* dependency_1 = resolver.Get<dependency_object>();
    auto* dependency_2 = resolver.Get<dependency_another_object>();
    return cf::Create<my_object>(dependency_1, dependency_2);
  });
```
Instances `dependency_1` and `dependency_2` will be deleted with `my_object` instance

Then complete registration and check errors:
```
std::unique_ptr<cf::Core> core_u_ptr = builder.Build();
  if (!core_u_ptr) {
    std::cout << builder.Error() << std::endl;
  }
```

### Type of objects
There are available four types:
- *Single* - an instance created once and used many times
- *Multiple* - (default type) an instance is created every time on request
- *Lock pool* - an instance is created if the pool is not full or the thread is blocked until another instance returns to the pool
- *Soft pool* - an instance is created if the pool is empty. After the instance returns to the pool it is deleted if the pool is full

### Using of factory
Save `std::unique_ptr<cf::Core> core_u_ptr` in public place after registration and use it:
```
std::shared_ptr<my_class> instance = core_u_ptr->GetShared<my_class>();
```
or
```
std::unique_ptr<my_class, cf::Deleter<my_class>> instance = core_u_ptr->GetUnique<my_class>();
```


