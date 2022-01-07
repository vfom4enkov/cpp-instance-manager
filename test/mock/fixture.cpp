#include "fixture.h"

#include <cpptoolkit/factory/builder.h>

#include <iostream>
#include <mutex>

namespace cpptoolkit {
namespace factory {
std::mutex Fixture::mutex_;

Fixture::Fixture() : core_u_ptr_(nullptr) {
  mutex_.lock();

  MockUnitLevel_1::reset();
  MockUnitLevel_2::reset();
  MockUnitLevel_3::reset();
  MockUnitSingleInstance::reset();
  MockUnitSingleInstanceTop::reset();

  RegisterMockUnits();
}

Fixture::~Fixture() { mutex_.unlock(); }

void Fixture::RegisterMockUnits() {
  Builder builder;
  builder.RegisterType<MockUnitLevel_3>();
  builder
      .Register<MockUnitLevel_2>([](Resolver& resolver) -> MockUnitLevel_2* {
        return Create<MockUnitLevel_2_A>(resolver.Get<MockUnitLevel_3>());
      })
      .SetKey("A");
  builder
      .Register<MockUnitLevel_2>([](Resolver& resolver) -> MockUnitLevel_2* {
        return Create<MockUnitLevel_2_B>();
      })
      .SetKey("B");
  builder.Register<MockUnitLevel_1>([](Resolver& resolver) -> MockUnitLevel_1* {
    return Create<MockUnitLevel_1>(resolver.Get<MockUnitLevel_2>("A"),
                                   resolver.Get<MockUnitLevel_2>("B"));
  });
  builder
      .Register<MockUnitSingleInstance>(
          [](Resolver& resolver) -> MockUnitSingleInstance* {
            return Create<MockUnitSingleInstance>(
                resolver.Get<MockUnitLevel_3>());
          })
      .AsSingleInstance();
  builder
      .Register<MockUnitSingleInstanceTop>(
          [](Resolver& resolver) -> MockUnitSingleInstanceTop* {
            return Create<MockUnitSingleInstanceTop>(
                resolver.Get<MockUnitLevel_1>(),
                resolver.Get<MockUnitSingleInstance>());
          })
      .AsSingleInstance();
  builder.Register<MockUnitReturnNullOncreate>(
      [](Resolver& resolver) -> MockUnitReturnNullOncreate* {
        return nullptr;
      });
  builder.Register<MockUnitThrowExceptionOncreate>(
      [](Resolver& resolver) -> MockUnitThrowExceptionOncreate* {
        throw std::exception(
            "Exception created on operation create "
            "MockUnitThrowExceptionOncreate");
      });
  builder.Register<MockUnitThrowStrOncreate>(
      [](Resolver& resolver) -> MockUnitThrowStrOncreate* {
        throw "String created on operation create MockUnitThrowExceptionOncreate";
      });
  core_u_ptr_ = builder.Build();
  core_ = core_u_ptr_.get();
  if (core_ == nullptr) {
    std::cout << builder.Error() << std::endl;
  }
}

}  // namespace factory
}  // namespace cpptoolkit
