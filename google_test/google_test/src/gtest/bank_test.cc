#include <iostream>
#include "gtest/gtest.h"

using namespace std;

struct BankAccount
{
    /* data */
    int balance = 0;
    BankAccount() {

    }

    explicit BankAccount(const int balance):balance(balance){

    }

    void deposit(int money){
        balance += money;
    }

    bool withdraw(int money){
        if (money <= balance) {
            balance -= money;
            return true;
        }
        return false;
    }
};

TEST(AcconutTest, BankAccountEmpty){
    BankAccount account;
    EXPECT_EQ(0, account.balance);
}

//夹具类型
struct BankAccountTest:testing::Test
{
    BankAccount* account;
    BankAccountTest(){
        cout << "BankAccountTest construct" <<endl;
        account = new BankAccount;
    }
    ~BankAccountTest(){
        cout << "BankAccountTest deconstruct" <<endl;
        delete account;
    }
};

TEST_F(BankAccountTest, BankAccountEmpty){
    EXPECT_EQ(0, account->balance);
}

TEST_F(BankAccountTest, canDepositMoney){
    account->deposit(100);
    EXPECT_EQ(100, account->balance);
}

TEST_F(BankAccountTest, DepositMoneyContinue){
    account->deposit(100);
    EXPECT_EQ(100, account->balance) << "--->" << account->balance;
}

//夹具类型的值参数化抽象测试
struct account_state
{
    /* data */
    int initial_balance;
    int withdraw_amount;
    int final_balance;
    bool success;

    friend std::ostream& operator<<(std::ostream& os, const account_state& obj){
        return os
            << "initial_balance: " << obj.initial_balance
            << "withdraw_amount: " << obj.withdraw_amount
            << "final_balance: " << obj.final_balance
            << "success: " << obj.success;
    }
};

struct WithdrawAccountTest : BankAccountTest, testing::WithParamInterface<account_state>
{
    /* data */
    WithdrawAccountTest() {
        account->balance = GetParam().initial_balance;
    }
};

TEST_P(WithdrawAccountTest, FinalBalance) {
    auto as = GetParam();
    auto success = account->withdraw(as.withdraw_amount);
    EXPECT_EQ(as.final_balance, account->balance);
    EXPECT_EQ(as.success, success);
}

INSTANTIATE_TEST_SUITE_P(Default, WithdrawAccountTest, 
    testing::Values(account_state{100,50,50,true},
                    account_state{100,200,100,false}));

int main(int argc, char**argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}