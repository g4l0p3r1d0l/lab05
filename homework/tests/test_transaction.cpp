#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Transaction.h"
#include "Account.h"

using ::testing::Return;

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}

    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
    MOCK_METHOD1(ChangeBalance, void(int));
    MOCK_CONST_METHOD0(GetBalance, int());
};

TEST(Transaction, DefaultFee) {
    Transaction tr;
    ASSERT_EQ(tr.fee(), 1);
}

TEST(Transaction, SetFee) {
    Transaction tr;
    tr.set_fee(10);
    ASSERT_EQ(tr.fee(), 10);
}

TEST(Transaction, SameAccountThrows) {
    Transaction tr;
    Account acc(1, 500);
    ASSERT_THROW(tr.Make(acc, acc, 200), std::logic_error);
}

TEST(Transaction, NegativeSumThrows) {
    Transaction tr;
    Account from(1, 500);
    Account to(2, 500);
    ASSERT_THROW(tr.Make(from, to, -100), std::invalid_argument);
}

TEST(Transaction, TooSmallSumThrows) {
    Transaction tr;
    Account from(1, 500);
    Account to(2, 500);
    ASSERT_THROW(tr.Make(from, to, 50), std::logic_error);
}

TEST(Transaction, FeeTooBigReturnsFalse) {
    Transaction tr;
    tr.set_fee(60);
    Account from(1, 500);
    Account to(2, 500);
    ASSERT_FALSE(tr.Make(from, to, 100));
}

TEST(Transaction, SuccessTransfer) {
    Transaction tr;
    Account from(1, 1000);
    Account to(2, 500);
    ASSERT_TRUE(tr.Make(from, to, 200));
}

TEST(Transaction, CallsAccountMethodsWithMock) {
    Transaction tr;
    MockAccount from(1, 1000);
    MockAccount to(2, 500);

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(to, ChangeBalance(200)).Times(1);
    EXPECT_CALL(to, GetBalance()).Times(2).WillRepeatedly(Return(1000));
    EXPECT_CALL(to, ChangeBalance(-201)).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    tr.Make(from, to, 200);
}
