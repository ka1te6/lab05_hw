#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "banking/Account.h"
#include "banking/Transaction.h"
#include "MockAccount.h" 

class TransactionTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(TransactionTest, MakeThrowsIfSameAccount) {
    Account account(1, 1000);
    Transaction transaction;
    account.Unlock();
    EXPECT_THROW(transaction.Make(account, account, 300), std::logic_error);
}

TEST_F(TransactionTest, MakeCallsAccountMethodsCorrectly) {
    MockAccount from(1, 1000);
    MockAccount to(2, 500);
    Transaction transaction;


    EXPECT_CALL(from, GetBalance()).WillRepeatedly(testing::Return(1000));
    EXPECT_CALL(to, GetBalance()).WillRepeatedly(testing::Return(500));

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(from, ChangeBalance(-100)).Times(1);
    EXPECT_CALL(to, ChangeBalance(100)).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    bool result = transaction.Make(from, to, 100);
    EXPECT_TRUE(result);
}

TEST_F(TransactionTest, MakeUnlocksOnFailure) {
    MockAccount from(1, 50);
    MockAccount to(2, 500);
    Transaction transaction;
    
    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(from, ChangeBalance(testing::_)).Times(0);
    EXPECT_CALL(to, ChangeBalance(testing::_)).Times(0);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);

    bool result = transaction.Make(from, to, 100);
    EXPECT_FALSE(result);
}

TEST_F(TransactionTest, MakeThrowsIfNegativeSum) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    EXPECT_THROW(transaction.Make(from, to, -100), std::invalid_argument);
}

TEST_F(TransactionTest, MakeThrowsIfSumTooSmall) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    EXPECT_THROW(transaction.Make(from, to, 99), std::logic_error);
}

TEST_F(TransactionTest, MakeReturnsFalseIfFeeTooHigh) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction transaction;
    transaction.set_fee(60); 
    from.Unlock();
    to.Unlock();
    EXPECT_FALSE(transaction.Make(from, to, 100));
}

TEST_F(TransactionTest, MakeSuccessWithMinimumSum) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    bool result = transaction.Make(from, to, 100);
    EXPECT_TRUE(result);
    EXPECT_EQ(from.GetBalance(), 899); 
    EXPECT_EQ(to.GetBalance(), 600);   
}

TEST_F(TransactionTest, MakeFailsWhenInsufficientFunds) {
    Account from(1, 100); 
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    bool result = transaction.Make(from, to, 100);
    EXPECT_FALSE(result);
    EXPECT_EQ(from.GetBalance(), 100); 
    EXPECT_EQ(to.GetBalance(), 500);  
}

TEST_F(TransactionTest, FeeMethodsWork) {
    Transaction transaction;
    EXPECT_EQ(transaction.fee(), 1);
    transaction.set_fee(10);
    EXPECT_EQ(transaction.fee(), 10);
}

TEST_F(TransactionTest, BalancesUnchangedWhenTransactionFails) {
    Account from(1, 50); 
    Account to(2, 500);
    Transaction transaction;
    from.Unlock();
    to.Unlock();
    
    int from_initial = from.GetBalance();
    int to_initial = to.GetBalance();
    
    bool result = transaction.Make(from, to, 100);
    
    EXPECT_FALSE(result);
    EXPECT_EQ(from.GetBalance(), from_initial);
    EXPECT_EQ(to.GetBalance(), to_initial);
}
