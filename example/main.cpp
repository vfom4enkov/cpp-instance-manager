#include <cpptoolkit/factory/builder.h>

#include <iostream>

#include "abstract_custom.h"
#include "abstract_default.h"
#include "pool.h"
#include "root.h"
#include "single.h"

namespace cf = cpptoolkit::factory;

namespace example {

std::unique_ptr<cf::Core> RegisterObjects(std::string& error_out) {
  cf::Builder builder;
  builder
      .Register<Abstract>([](cf::Resolver& resolver) -> Abstract* {
        return cf::Create<AbstractCustom>();
      })
      .AsMultipleInstance()
      .SetKey("Custom");

  builder
      .Register<Abstract>([](cf::Resolver& resolver) -> Abstract* {
        return cf::Create<AbstractDefault>();
      })
      .AsMultipleInstance();

  builder
      .Register<Pool>([](cf::Resolver& resolver) -> Pool* {
        return cf::Create<Pool>(resolver.Get<Abstract>("Custom"));
      })
      .AsLockPoolInstance(2);

  builder
      .Register<Single>([](cf::Resolver& resolver) -> Single* {
        return cf::Create<Single>(resolver.Get<Abstract>());
      })
      .AsSingleInstance();

  builder
      .Register<Root>([](cf::Resolver& resolver) -> Root* {
        return cf::Create<Root>(resolver.Get<Pool>(), resolver.Get<Single>());
      })
      .AsMultipleInstance();

  std::unique_ptr<cf::Core> core = builder.Build();
  if (!core) {
    error_out = builder.Error();
  }

  return core;
}

}  // namespace example

int main() {
  std::string error;
  auto core = example::RegisterObjects(error);
  if (!core) {
    std::cout << error << std::endl;
    return -1;
  }

  {
    const auto root = core->GetShared<example::Root>();
    root->Action();
  }

  std::cout << "End the program" << std::endl;
}

//	AbstractDefault::Constructor
//	Single::Constructor
//	AbstractCustom::Constructor
//	Pool::Constructor
//	Root::Constructor
//	Root::Action
//	Pool::Action
//	AbstractCustom::Action
//	Single::Action
//	AbstractDefault::Action
//	Root::Destructor
//	End the program
//	Single::Destructor
//	AbstractDefault::Destructor
//	Pool::Destructor
//	AbstractCustom::Destructor
