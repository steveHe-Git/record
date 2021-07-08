#include "queue_array.h"
#include <vector>
#include "gtest/gtest.h"

using namespace testing;
using namespace std;

TEST(ArrayQueueTest, SimpleTest) {
    create_queue(10);
    EXPECT_TRUE(empty());
    push(2);
    push(4);
    push(5);
    push(6);
    EXPECT_EQ(size(), 4);
    EXPECT_EQ(front(), 2);
    EXPECT_EQ(size(), 4);
    int value = pop();
    EXPECT_EQ(value, 2);
    EXPECT_EQ(front(), 4);
    EXPECT_EQ(size(), 3);

    destory_queue();
}

int main (int argc, char** argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}