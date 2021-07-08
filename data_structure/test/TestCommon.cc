#include "common.h"
#include <vector>
#include "gtest/gtest.h"

using namespace testing;
using namespace std;

TEST(CommonTest, SplitTest) {
    string s = "192.168.0.1";
    string d = ".";
    auto res = split(s, d);
    EXPECT_EQ(res[0], "192");
    EXPECT_EQ(res[1], "168");
    EXPECT_EQ(res[2], "0");
    EXPECT_EQ(res[3], "1");
}

int main (int argc, char** argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}