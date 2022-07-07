/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2021, Vladimir Fomchenkov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "test_context.h"

namespace cpptoolkit {
namespace factory {

namespace {

class Mock : public AbstractPoolInstancePutback {
 public:
  Mock() noexcept : callbackCallCounter_(0), key_(0){};
  ~Mock() noexcept = default;
  void Callback(uintptr_t key) noexcept override;
  int CallbackCallCounter() noexcept;
  uintptr_t Key() noexcept;

 private:
  int callbackCallCounter_;
  uintptr_t key_;
};

void Mock::Callback(uintptr_t key) noexcept {
  ++callbackCallCounter_;
  key_ = key;
}

int Mock::CallbackCallCounter() noexcept { return callbackCallCounter_; }

uintptr_t Mock::Key() noexcept { return key_; }

}  // namespace

BOOST_AUTO_TEST_SUITE(TestContext)

BOOST_FIXTURE_TEST_CASE(test_context_check_delete_instance_on_delete_context,
                        Fixture) {
  // arrange
  MockUnitLevel_3* item = new MockUnitLevel_3();
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(0, MockUnitLevel_3::getDestructorCounter());

  // act
  {
    Context<MockUnitLevel_3> context;
    context.SetInstance(item);
  }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

BOOST_FIXTURE_TEST_CASE(test_context_add_dependency_normal_case, Fixture) {
  // arrange
  Context<MockUnitLevel_3> context;
  context.SetInstance(new MockUnitLevel_3());

  // act
  Context<MockUnitLevel_2_B>* a_context = new Context<MockUnitLevel_2_B>();
  a_context->SetInstance(new MockUnitLevel_2_B());
  UPtr<AContext> a_context_ptr(a_context);
  context.Add(std::move(a_context_ptr));

  // assert
  BOOST_CHECK_EQUAL(1, context.dependencies_.size());
}

BOOST_FIXTURE_TEST_CASE(test_context_check_error_transfer, Fixture) {
  // arrange
  std::string error = "error";
  UPtr<ErrorContext<MockUnitLevel_3>> invalid_ctx =
      MakeUPtr<ErrorContext<MockUnitLevel_3>>(error);
  Context<MockUnitLevel_2_B> valid_ctx;
  valid_ctx.SetInstance(new MockUnitLevel_2_B());
  BOOST_CHECK(valid_ctx.IsValid());
  BOOST_CHECK(valid_ctx.Error().empty());
  BOOST_CHECK(!invalid_ctx->IsValid());
  BOOST_CHECK(!invalid_ctx->Error().empty());

  // act
  valid_ctx.Add(std::move(invalid_ctx));

  // assert
  BOOST_CHECK(!valid_ctx.IsValid());
  BOOST_CHECK(!valid_ctx.Error().empty());
  BOOST_CHECK_EQUAL(1, valid_ctx.dependencies_.size());
}

BOOST_FIXTURE_TEST_CASE(test_context_check_clearing_all_dependencies, Fixture) {
  {
    // arrange

    MockUnitLevel_3* unit_3 = new MockUnitLevel_3();
    MockUnitLevel_2* unit_2_b = new MockUnitLevel_2_B();
    MockUnitLevel_2* unit_2_a = new MockUnitLevel_2_A(unit_3);
    MockUnitLevel_1* unit_1 = new MockUnitLevel_1(unit_2_b, unit_2_a);

    Context<MockUnitLevel_3>* context_3 = new Context<MockUnitLevel_3>();
    context_3->SetInstance(unit_3);
    UPtr<AContext> context_3_ptr(context_3);

    Context<MockUnitLevel_2>* context_2_b = new Context<MockUnitLevel_2>();
    context_2_b->SetInstance(unit_2_b);
    UPtr<AContext> context_2_b_ptr(context_2_b);

    Context<MockUnitLevel_2>* context_2_a = new Context<MockUnitLevel_2>();
    context_2_a->SetInstance(unit_2_a);
    UPtr<AContext> context_2_a_ptr(context_2_a);

    Context<MockUnitLevel_1>* context_1 = new Context<MockUnitLevel_1>();
    context_1->SetInstance(unit_1);
    UPtr<Context<MockUnitLevel_1>> context_1_ptr(context_1);

    // act
    context_2_a->Add(std::move(context_3_ptr));
    context_1_ptr->Add(std::move(context_2_a_ptr));
    context_1_ptr->Add(std::move(context_2_b_ptr));
  }

  // assert
  BOOST_CHECK_EQUAL(1, MockUnitLevel_1::getDestructorCounter());
  BOOST_CHECK_EQUAL(2, MockUnitLevel_2::getDestructorCounter());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getDestructorCounter());
}

BOOST_FIXTURE_TEST_CASE(test_weak_context_create_and_delete, Fixture) {
  // arrange
  std::unique_ptr<BaseContext<MockUnitLevel_3>> context_ptr_h(
      new Context<MockUnitLevel_3>());
  context_ptr_h->SetInstance(new MockUnitLevel_3());
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(0, MockUnitLevel_3::getDestructorCounter());

  // act
  {
    UPtr<WeakContext<MockUnitLevel_3>> weak_context_ptr_h =
        MakeUPtr<WeakContext<MockUnitLevel_3>>(
            context_ptr_h.get()->GetInstance());
    BOOST_CHECK(weak_context_ptr_h->IsValid());
    BOOST_CHECK(weak_context_ptr_h->Error().empty());
  }

  // assert
  // WeakContext did not delete managed object
  BOOST_CHECK_EQUAL(1, MockUnitLevel_3::getConstructorCounter());
  BOOST_CHECK_EQUAL(0, MockUnitLevel_3::getDestructorCounter());
}

BOOST_FIXTURE_TEST_CASE(test_pool_context_call_putback, Fixture) {
  // arrange
  Mock mock;
  Context<MockUnitLevel_3> context;
  uintptr_t val = 5;

  // act
  { PoolContext<MockUnitLevel_3> poolContext(&mock, &context, val); }

  // assert
  BOOST_CHECK_EQUAL(val, mock.Key());
  BOOST_CHECK_EQUAL(1, mock.CallbackCallCounter());
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace factory
}  // namespace cpptoolkit
