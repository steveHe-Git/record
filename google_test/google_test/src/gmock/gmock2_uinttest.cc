/*
 * 使用gmock  mock 普通函数的 example
*/

#include <iostream>
#include <string>
#include "gmock/gmock.h"

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::InitGoogleMock;

template<typename T>
class A
{
public:
    int FunctionA(T& obj) {
        //doing something
        std::string str = "mock non virtual";
        auto res = obj.FunctionB(str);
        return res;
    }
};

class B
{
public:
    int FunctionB(std::string str){
        return puts(str.c_str());
    }
};

class MockB
{
public:
    MOCK_METHOD1(FunctionB, int(std::string str));
};

class A_UT : public testing::Test
{
protected:
    virtual void SetUp(){}
    virtual void TearDOwn() {}

    //重点：将类A改写为类模板之后，在生产代码中，
    //需要使用真正的类B对象来进行模板类的实例化，而在测试代码中，
    //则需要使用mock出来的类B对象进行模板类的实例化。
    //它们之间的无关的，这与mock接口类的虚函数有着本质的区别。
    A<MockB> MockObjA;
};

TEST_F(A_UT, FunctionTest) {
    MockB mockObjB;
    EXPECT_CALL(mockObjB, FunctionB(_))
    .Times(AtLeast(1))
    .WillOnce(Return(100));

    auto res = MockObjA.FunctionA(mockObjB);
    EXPECT_EQ(100, res);
}





