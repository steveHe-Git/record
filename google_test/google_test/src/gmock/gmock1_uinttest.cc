/*
 * 使用gmock  mock 虚函数的 example
*/

#if 0
#include <iostream>
#include <string>
//#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::InitGoogleMock;

class DataBaseConnect
{
private:
    /* data */
public:
    virtual bool login(std::string username, std::string password)=0;
    virtual bool logout(std::string username)=0;
    virtual int fetchRecord()=0;
};

class MockDB:public DataBaseConnect
{
private:
    /* data */
public:
    MOCK_METHOD0(fetchRecord, int());
    MOCK_METHOD1(logout, bool(std::string username));
    MOCK_METHOD2(login, bool(std::string username, std::string password));
};

class MyDataBase
{
private:
    /* data */
    DataBaseConnect& dbc;
public:
    MyDataBase(DataBaseConnect& _dbc):dbc(_dbc){}
    int Init(std::string username, std::string password) {
        if (dbc.login(username, password) != true) {
            cout << "DB FAILURE" << endl; return -1;
        } else {
            cout << "DB SUCCESS" << endl; return 1;
        }
    }
};

struct tetsABC
{
    /* data */
    bool dummlogin(string a, string b){
        cout << "Dummy login gets call " <<a <<" "<< b<<endl; return true;
    }
};

bool DummyFn() {
    cout << "Global Fn.called ..." << endl;
    return true;
}

TEST(MyDataTest, LoginSuccess) {
    //Arrage
    MockDB mdb;
    MyDataBase db(mdb);
    EXPECT_CALL(mdb, login("Terminator", "I'm back")).Times(AtLeast(1)).WillOnce(Return(true));
    //Act
    int retValue = db.Init("Terminator", "I'm back");
    EXPECT_EQ(retValue, 1);
}

TEST(MyDataTest, LoginFailure) {
    //Arrage
    MockDB mdb;
    MyDataBase db(mdb);
    EXPECT_CALL(mdb, login("Terminator", "I'm back")).Times(AtLeast(1)).WillOnce(Return(false));
    //Act
    int retValue = db.Init("Terminator", "I'm back");
    EXPECT_EQ(retValue, -1);
}

TEST(MyDataTest, LoginInvokeWithArgs) {
    //Arrage
    MockDB mdb;
    MyDataBase db(mdb);
    tetsABC dbTest;
    EXPECT_CALL(mdb, login("Terminator", "I'm back")).Times(AtLeast(1)).WillOnce(testing::Invoke(&dbTest, &tetsABC::dummlogin));
    //Act
    int retValue = db.Init("Terminator", "I'm back");
    EXPECT_EQ(retValue, 1);
}

TEST(MyDataTest, LoginInvokeWithoutArgs) {
    //Arrage
    MockDB mdb;
    MyDataBase db(mdb);
    tetsABC dbTest;
    EXPECT_CALL(mdb, login("Terminator", "I'm back")).Times(AtLeast(1)).WillOnce(testing::InvokeWithoutArgs(DummyFn));
    //Act
    int retValue = db.Init("Terminator", "I'm back");
    EXPECT_EQ(retValue, 1);
}

int main (int argc, char**argv) {
    testing::InitGoogleMock(&argc, argv);
    //testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else 
#include <iostream>
int main () {
    return 0;
}
#endif
