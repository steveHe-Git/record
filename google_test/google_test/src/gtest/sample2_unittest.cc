#include "sample2.h"
#include "gtest/gtest.h"

const char HelloString[] = "hello world!";

TEST(MyStringTest, DefauleConstructor) {
    const MyString s;
    EXPECT_EQ(nullptr, s.getCString());
    EXPECT_EQ(0, s.Length());
}

TEST(MyString, ConstructorFromCString) {
    const MyString s(HelloString);
    EXPECT_EQ(0, strcmp(s.getCString(), HelloString));
    EXPECT_EQ(sizeof(HelloString)/sizeof(HelloString[0]) - 1, s.Length());
}

TEST(MyStringTest, CopyConstructor) {
    MyString s1(HelloString);
    EXPECT_EQ(12, s1.Length());
    s1.print();
    MyString s2 = s1;
    s2.print();
    
    EXPECT_EQ(0, strcmp(s2.getCString(), HelloString));
}

TEST(MyStringTest, SetTest) {
    MyString s;
    s.set(HelloString);
    EXPECT_EQ(0, strcmp(s.getCString(), HelloString));

    s.set(s.getCString());
    EXPECT_EQ(0, strcmp(s.getCString(), HelloString));

    s.set(nullptr);
    EXPECT_EQ(nullptr, s.getCString());

}

