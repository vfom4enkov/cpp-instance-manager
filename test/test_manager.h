#ifndef CPP_TOOL_KIT_FACTORY_TEST_MANAGER_H_
#define CPP_TOOL_KIT_FACTORY_TEST_MANAGER_H_

#include <boost/test/unit_test.hpp>

// just for tests
#define private public
#define protected public

#include <cpptoolkit/factory/manager/lock_pool_instance_manager.h>
#include <cpptoolkit/factory/manager/multiple_instance_manager.h>
#include <cpptoolkit/factory/manager/pool_instance_manager.h>
#include <cpptoolkit/factory/manager/single_instance_manager.h>

#include "mock/fixture.h"

#endif  // CPP_TOOL_KIT_FACTORY_TEST_MANAGER_H_
