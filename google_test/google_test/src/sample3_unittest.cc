#include "sample3_queue.h"
#include "gtest/gtest.h"

namespace {
    
//夹具测试
class QueueTestSample3:public testing::Test
{
private:
    /* data */
public:
    /* data */
protected:
    void SetUp() override {
        q1.Enqueue(1);
        q2.Enqueue(2);
        q2.Enqueue(3);
    }

    static int Double(int n) {
        return 2*n;
    }

    void MapTester(const Queue<int>* q) {
        const Queue<int>* const new_queue = q->Map(Double);

        ASSERT_EQ(new_queue->Size(), q->Size());

        for (const QueueNode<int>* n1 = q->Head(),* n2 = new_queue->Head(); 
            n1 != nullptr; n1 = n1->Next(), n2 = n2->Next()) {
            EXPECT_EQ(2* n1->Element(), n2->Element());
        }

        delete new_queue;
    }

    Queue<int> q0;
    Queue<int> q1;
    Queue<int> q2;
};

TEST_F(QueueTestSample3, DefaultStruct) {
    EXPECT_EQ(q0.Head(), nullptr);
    EXPECT_EQ(q0.Size(), 0);
}

TEST_F(QueueTestSample3, DequeueTest) {
    int *n = q0.Dequeue();
    ASSERT_EQ(n, nullptr);

    n = q1.Dequeue();
    ASSERT_EQ(*n, 1);
    ASSERT_EQ(0, q1.Size());
    delete n;

    n = q2.Dequeue();
    ASSERT_EQ(*n, 2);
    ASSERT_EQ(1, q2.Size());
    delete n;


}

TEST_F(QueueTestSample3, MapTest) {
    MapTester(&q0);
    MapTester(&q1);
    MapTester(&q2);
}



}//namespace