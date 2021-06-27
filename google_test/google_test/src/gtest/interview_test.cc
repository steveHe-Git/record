#include <iostream>
#include "gtest/gtest.h"
#include "interview.hpp"

// class MockTest
// {
// private:
//     /* data */
// public:
//     MockTest(/* args */);
//     ~MockTest();
// };

class InterviewTest: public testing::Test
{
private:
    /* data */
protected:
    void SetUp() {}
    void TearDown() {}

    RouteManger route;
};

TEST_F(InterviewTest, TestUpdate) {
    EXPECT_EQ(0, route.getSize());
    int ret = route.update(0, "路线1", "路线2", 100.0);
    EXPECT_EQ(ret, 1);
    EXPECT_EQ(1, route.getSize());
    ret = route.update(0, "路线2", "路线3", 150.0);
    EXPECT_EQ(ret, 2);
    EXPECT_EQ(2, route.getSize());
    ret = route.update(1, "路线1", "路线2", 120.0);
    EXPECT_EQ(ret, 1);
    EXPECT_EQ(2, route.getSize());
}

TEST_F(InterviewTest, TestFindId) {
    route.update(0, "路线1", "路线2", 100.0);
    route.update(0, "路线2", "路线3", 150.0);
    route.update(0, "路线3", "路线4", 120.0);
    route.update(0, "路线3", "路线1", 100.0);
    //route.printMapData();

    RouteData data_actual = route.find_by_id(1);
    RouteData data_expect = RouteData{1, "路线1", "路线2", 100.0};
    EXPECT_EQ(data_expect, data_actual);

    route.update(1, "路线1", "路线2", 120.0);
    //route.printMapData();
    RouteData data_actual1 = route.find_by_id(1);
    RouteData data_expect1 = RouteData{1, "路线1", "路线2", 120.0};
    EXPECT_EQ(data_expect1, data_actual1);
}

TEST_F(InterviewTest, TestFindStart) {
    EXPECT_EQ(0, route.getSize());
    route.update(0, "路线1", "路线2", 100.0);
    route.update(0, "路线2", "路线3", 150.0);
    route.update(0, "路线3", "路线4", 120.0);
    route.update(0, "路线3", "路线1", 100.0);
    EXPECT_EQ(4, route.getSize());
    //route.printMapData();
    vector<int> tmp_actual, tmp_except;
    tmp_except.push_back(4);
    tmp_except.push_back(3);
    tmp_actual = route.find_by_start_point("路线3");

    EXPECT_EQ(tmp_actual, tmp_except);
}

int main (int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}