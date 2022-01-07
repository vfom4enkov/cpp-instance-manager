#ifndef CPP_TOOL_KIT_FACTORY_TEST_CONTEXT_H_
#define CPP_TOOL_KIT_FACTORY_TEST_CONTEXT_H_

#include <boost/test/unit_test.hpp>

// just for tests
#define private public
#define protected public

#include <cpptoolkit/factory/manager/context/pool_context.h>
#include <cpptoolkit/factory/manager/context/weak_context.h>

#include "mock/fixture.h"

#endif  // CPP_TOOL_KIT_FACTORY_TEST_CONTEXT_H_
