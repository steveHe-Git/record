#include "gtest/gtest.h"
#include "sample1.h"

TEST(FactorialTest, Negative) {
    EXPECT_EQ(1, Factorial(-1));
    EXPECT_EQ(1, Factorial(-5));
    EXPECT_GT(Factorial(-10), 0);
}

TEST(FactorialTest, ZERO) {
    EXPECT_EQ(1, Factorial(0));
}

TEST(FactorialTest, Positive) {
    EXPECT_EQ(1, Factorial(1));
    EXPECT_EQ(2, Factorial(2));
    EXPECT_EQ(40320, Factorial(8));
}


TEST(IsPrimeTest, Negative) {
    EXPECT_FALSE(IsPrime(-1));
    EXPECT_FALSE(IsPrime(-2));
}

TEST(IsPrimeTest, Trivial) {
    EXPECT_FALSE(IsPrime(0));
    EXPECT_FALSE(IsPrime(1));
    EXPECT_TRUE(IsPrime(2));
    EXPECT_TRUE(IsPrime(3));
}

TEST(IsPrimeTest, Positive) {
    EXPECT_FALSE(IsPrime(4));
    EXPECT_TRUE(IsPrime(5));
    EXPECT_FALSE(IsPrime(6));
    EXPECT_TRUE(IsPrime(23));
}