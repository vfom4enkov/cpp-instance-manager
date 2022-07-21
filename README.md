# CppToolKit-Factory
Header based tool provides control of lifetime for C++ business logic objects.

How to use?

## Step 1 - Add the tool to your project

Download (or clone) the tool and add the path to `src` to the system `PATH`. Or if you use `cmake` just add line to your `CMakeLists.txt`:
```
include_directories({pat_to_cpptoolkit-factory}/src)
```

## Step 2 - Register BL objects

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
        auto* file_and_db_logger = resolver.Get<AbstractLogger>("DB_AND_FILE");     // (11)
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

  std::unique_ptr<cf::Core> core = builder.BuildUnique();                           // (12)
  if (!core) {                                                                      // (13)
    error = builder.Error();
  }
```
Where
1. Create Builder (helper for registration objects)
2. Register type `DbLogger` without dependencies in the pool (check [Types of objects](#types-of-objects) for more info) with size 10
3. Register type `FileLogger` without dependencies
4. Register type `NetLogger` without dependencies as single instance 
5. Register type `AbstractLogger` with dependencies
6. Get dependency instance `FileLogger` for `AbstractLogger`
7. Get dependency instance `DbLogger` for `AbstractLogger`
8. Create object `ComplexLogger` as `AbstractLogger`
9. Add key `DB_AND_FILE` for type `AbstractLogger` (check [Keys](#Keys) for more info)
10. Register another type of `AbstractLogger` with dependencies
11. Get dependency object with key
12. Complete registration and create the Core
13. Check errors on register objects operation. If the builder contains an error core will not be created

Save `std::unique_ptr<cf::Core> core` and use when you need to create instance of your BL object.

### Types of objects
There are four types available:
- *Single* - an instance created once and used many times (shared for other instances)
- *Multiple* - (default type) an instance is created every time on request
- *Lock pool* - an instance is created if the pool is not full or the thread is blocked until another instance returns to the pool
- *Soft pool* - an instance is created if the pool is empty. At the end of usage instance returns to the pool or deleted if the pool is full

Examples:

2. registration of lock pool for 10 instances
3. registration of multiple instance by default (or add `.AsMultipleInstance()` on type registration)
4. registration of single instance

### Keys

If you need to register many types as base object (such as `(5)` and `(10)`) just add keys for these objects. `(5)` is registered with **DB_AND_FILE** key, `(10)` is registered with default key.
How to get instance of object with specific key check the [Using of factory](#using-of-factory)

## Step 3 - Use of the factory

* call the `Get<T>()` to create instance of your class
* check if the instance was created without error
* get description in fail case
* call your mehods in success case

```cpp
  auto action = core->Get<example::Action>();
  if (!action.IsValid()) {
    std::cout << "Error: " << action.Error();
    return 1;
  }

  action->Exec();
```

Create object with the key
```cpp
  auto a_logger = core->GetShared<example::AbstractLogger>("DB_AND_FILE");
```

## Requirements

* C++11
* boost 1.73 (for unit tests only)
