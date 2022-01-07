#include "registrator.h"

#include <cpptoolkit/factory/builder.h>

#include "abstract_custom.h"
#include "abstract_default.h"
#include "pool.h"
#include "root.h"
#include "single.h"

namespace cf = cpptoolkit::factory;

namespace example {

std::unique_ptr<cpptoolkit::factory::Core> RegisterObjects(std::string& error) {
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
    error = builder.Error();
  }

  return core;
}
}  // namespace example
