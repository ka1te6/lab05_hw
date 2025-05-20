#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "banking/Account.h"

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}

    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};

TEST(AccountTest, IdMethod) {
    Account acc(42, 1000);
    EXPECT_EQ(acc.id(), 42);
}

TEST(AccountTest, LockUnlock) {
    MockAccount acc(1, 1000);
    EXPECT_CALL(acc, Lock()).Times(1);
    EXPECT_CALL(acc, Unlock()).Times(1);
    acc.Lock();
    acc.Unlock();
}

TEST(AccountTest, LockThrowsIfAlreadyLocked) {
    Account acc(1, 1000);
    acc.Lock();
    EXPECT_THROW(acc.Lock(), std::runtime_error);
    acc.Unlock();
}

TEST(AccountTest, ChangeBalanceFailsIfNotLocked) {
    Account acc(1, 1000);
    EXPECT_THROW(acc.ChangeBalance(100), std::runtime_error);
}

TEST(AccountTest, ChangeBalanceWithNegativeDiff) {
    Account acc(1, 1000);
    acc.Lock();
    acc.ChangeBalance(-100);
    acc.Unlock();
    EXPECT_EQ(acc.GetBalance(), 900);
}

TEST(AccountTest, LockUnlockAndChangeBalance) {
    Account acc(1, 100);
    acc.Lock();
    acc.ChangeBalance(50);
    acc.Unlock();
    EXPECT_EQ(acc.GetBalance(), 150);
}
