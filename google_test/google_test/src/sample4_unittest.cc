#include "sample4.h"
#include "gtest/gtest.h"

namespace
{

TEST(CounterTest, Increment) {
    Counter c;
    EXPECT_EQ(0, c.Decrement());
    EXPECT_EQ(0, c.Increment());
    EXPECT_EQ(1, c.Increment());
    EXPECT_EQ(2, c.Increment());
}

    
} // namespace name}


