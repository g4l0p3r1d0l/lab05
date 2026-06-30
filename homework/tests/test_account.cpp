#include <gtest/gtest.h>
#include "Account.h"

TEST(Account, Constructor) {
    Account acc(1, 100);
    EXPECT_EQ(acc.id(), 1);
    EXPECT_EQ(acc.GetBalance(), 100);
}

TEST(Account, ChangeBalanceThrowsIfNotLocked) {
    Account acc(1, 100);
    EXPECT_THROW(acc.ChangeBalance(50), std::runtime_error);
}

TEST(Account, ChangeBalanceWorksIfLocked) {
    Account acc(1, 100);
    acc.Lock();
    acc.ChangeBalance(50);
    EXPECT_EQ(acc.GetBalance(), 150);
}

TEST(Account, DoubleLockThrows) {
    Account acc(1, 100);
    acc.Lock();
    EXPECT_THROW(acc.Lock(), std::runtime_error);
}

TEST(Account, UnlockWorks) {
    Account acc(1, 100);
    acc.Lock();
    acc.Unlock();
    EXPECT_THROW(acc.ChangeBalance(10), std::runtime_error);
}
