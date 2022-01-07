#ifndef CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_ERROR_H_
#define CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_ERROR_H_

namespace cpptoolkit {
namespace factory {

struct MockUnitNotRegistered {};

struct MockUnitReturnNullOncreate {};

struct MockUnitThrowExceptionOncreate {};

struct MockUnitThrowStrOncreate {};

}  // namespace factory
}  // namespace cpptoolkit

#endif  // CPP_TOOL_KIT_FACTORY_TEST_MOCK_UNIT_ERROR_H_
