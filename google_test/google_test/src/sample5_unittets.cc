#include <time.h>
#include "sample1.h"
#include "sample3_queue.h"
#include "gtest/gtest.h"

namespace name
{


// In this sample, we want to ensure that every test finishes within
// ~5 seconds.  If a test takes longer to run, we consider it a
// failure.

class QuickTest:public testing::Test
{
private:
    /* data */
    time_t start_time;
public:
    /* data */

protected:
    void SetUp() override {
        start_time = time(nullptr);
    }

    void TearDown() override {
        const time_t end_time = time(nullptr);

        EXPECT_TRUE(end_time - start_time <= 5) << "The test task takes too long times";
    }
};

class IntegerFunctionTest:public QuickTest {};
TEST_F(IntegerFunctionTest, FactorialTest) {
    // Tests factorial of negative numbers.
    EXPECT_EQ(1, Factorial(-5));
    EXPECT_EQ(1, Factorial(-1));
    EXPECT_GT(Factorial(-10), 0);

    // Tests factorial of 0.
    EXPECT_EQ(1, Factorial(0));

    // Tests factorial of positive numbers.
    EXPECT_EQ(1, Factorial(1));
    EXPECT_EQ(2, Factorial(2));
    EXPECT_EQ(6, Factorial(3));
    EXPECT_EQ(40320, Factorial(8));
}


class QueueTest:public QuickTest
{
private:
    /* data */
public:
    /* data */
protected:
    void SetUp() override {
        QuickTest::SetUp();

        q1.Enqueue(1);
        q2.Enqueue(2);
        q2.Enqueue(3);
    }

    Queue<int>q0;
    Queue<int>q1;
    Queue<int>q2;
};

TEST_F(QueueTest, DefaultConstructor) {
    EXPECT_EQ(q0.Size(), 0);
}

TEST_F(QueueTest, DequeueTest) {
    int* n = q0.Dequeue();
    EXPECT_TRUE(n == nullptr);

    n = q1.Dequeue();
    EXPECT_EQ(*n, 1);
    EXPECT_EQ(0, q1.Size());
    delete n;

    n = q2.Dequeue();
    EXPECT_EQ(*n, 2);
    EXPECT_EQ(1, q2.Size());
    delete n;
}







} // namespace name
