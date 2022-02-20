# CppToolKit-Factory
This header based tool and provides control of lifetime for C++ business logic objects.

## How to use

### Add the tool to your project
Download (or clone) the tool and add the path to `src` to the system `PATH`. Or if you use `cmake` just add the line to your `CMakeLists.txt`:
```
include_directories({pat_to_cpptoolkit-factory}/src)
```

### Register objects
```cpp
  namespace cf = cpptoolkit::factory;
  ...
  cf::Builder builder;                                                              // (1)
  builder.RegisterType<DbLogger>().AsLockPoolInstance(10);                          // (2)
  builder.RegisterType<FileLogger>();                                               // (3)
  builder.RegisterType<NetLogger>().AsSingleInstance();                             // (4)
  builder
      .Register<AbstractLogger>([](cf::Resolver& resolver) -> AbstractLogger* {     // (5)
        auto* file_logger = resolver.Get<FileLogger>();
        auto* db_logger = resolver.Get<DbLogger>();
        return cf::Create<ComplexLogger>(file_logger, db_logger);
      })
      .SetKey("DB_AND_FILE");
  builder
      .Register<AbstractLogger>([](cf::Resolver& resolver) -> AbstractLogger* {
        auto* file_and_db_logger = resolver.Get<AbstractLogger>("DB_AND_FILE");
        auto* net_logger = resolver.Get<NetLogger>();
        return cf::Create<ComplexLogger>(file_and_db_logger, net_logger);
      });

  builder
      .Register<Action>([](cf::Resolver& resolver) -> Action* {
        auto* logger = resolver.Get<AbstractLogger>();
        return cf::Create<Action>(logger);
      });

  builder
      .Register<Action>([](cf::Resolver& resolver) -> Action* {
        auto* logger = resolver.Get<FileLogger>();
        return cf::Create<Action>(logger);
      })
      .SetKey("LIGHT");

  std::unique_ptr<cf::Core> core = builder.Build();
  if (!core) {
    error = builder.Error();
  }

  return core;
```


If an object without dependencies:
```
#include <cpptoolkit/factory/builder.h>
...
cpptoolkit::factory::Builder builder;
builder.RegisterType<my_class>();
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
- *Single* - an instance created once and used many times (shared for other instances)
- *Multiple* - (default type) an instance is created every time on request
- *Lock pool* - an instance is created if the pool is not full or the thread is blocked until another instance returns to the pool
- *Soft pool* - an instance is created if the pool is empty. After the instance returns to the pool it is deleted if the pool is full
Example registration of lock pool for 10 instances:
```
...
builder.RegisterType<my_class>()
  .AsLockPoolInstance(10);
  // or .AsMultipleInstance() - for multiple instances
  // or .AsSingleInstance()	- for single instance
  // or .AsSoftPoolInstance(10) - for soft pool instances
```

### Using of factory
Save `std::unique_ptr<cf::Core> core_u_ptr` in public place after registration and use it:
```
std::shared_ptr<my_class> instance = core_u_ptr->GetShared<my_class>();
```
or
```
std::unique_ptr<my_class, cf::Deleter<my_class>> instance = core_u_ptr->GetUnique<my_class>();
```

TODO:
### Multiple implementations for base/abstract classes

### Error handling

### For developers
