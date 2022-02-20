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
#include <cpptoolkit/factory/builder.h>
  ..
  namespace cf = cpptoolkit::factory;
  ...
  cf::Builder builder;                                                              // (1)
  builder.RegisterType<DbLogger>().AsLockPoolInstance(10);                          // (2)
  builder.RegisterType<FileLogger>();                                               // (3)
  builder.RegisterType<NetLogger>().AsSingleInstance();                             // (4)
  builder
      .Register<AbstractLogger>([](cf::Resolver& resolver) -> AbstractLogger* {     // (5)
        auto* file_logger = resolver.Get<FileLogger>();                             // (6)
        auto* db_logger = resolver.Get<DbLogger>();                                 // (7)
        return cf::Create<ComplexLogger>(file_logger, db_logger);                   // (8)
      })
      .SetKey("DB_AND_FILE");                                                       // (9)
  builder
      .Register<AbstractLogger>([](cf::Resolver& resolver) -> AbstractLogger* {     // (10)
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

  std::unique_ptr<cf::Core> core = builder.Build();                                 // (11)
  if (!core) {                                                                      // (12)
    error = builder.Error();
  }
```
Where
1. Create helper for registration object
2. Register type `DbLogger` without dependencies and register as it as Lock pool object (check [Types of objects](#types-of-objects) for more info) with pool size 10
3. Register type `FileLogger` without dependencies
4. Register type `NetLogger` without dependencies and register as it as single instance 
5. Register type `AbstractLogger` with dependencies
6. Get dependency object `FileLogger` for `AbstractLogger`
7. Get dependency object `DbLogger` for `AbstractLogger`
8. Create inherited object `ComplexLogger` and add dependencies and use it as `AbstractLogger`
9. Add key `DB_AND_FILE` for type `AbstractLogger` (check [Keys](#Keys) for more info)
10. Register another type `AbstractLogger` with dependencies
11. Complete registration and create the Core
12. Check errors on register objects operation if the builder contains an error core will not be created

### Types of objects
There are four types available:
- *Single* - an instance created once and used many times (shared for other instances)
- *Multiple* - (default type) an instance is created every time on request
- *Lock pool* - an instance is created if the pool is not full or the thread is blocked until another instance returns to the pool
- *Soft pool* - an instance is created if the pool is empty. After the instance returns to the pool it is deleted if the pool is full

2. Example registration of lock pool for 10 instances
3. Example registration of multiple instance by default (or add `.AsMultipleInstance()`)
4. Example registration of single instance

### Keys

If you need to register many types as base object (such as `(5)` and `(10)`) just add keys for these objects. '(5)' is registered with **DB_AND_FILE** key, `(10)` is registered with default key.
How to get instance of object with specific key check the [Using of factory](#using-of-factory)

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
### Error handling

### For developers
