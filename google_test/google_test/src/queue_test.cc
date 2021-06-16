#include <iostream>
#include <queue>
#include "gtest/gtest.h"

using namespace std;

template<typename T>
class Queue
{
private:
    /* data */
    queue<T*> m_queue;
public:
    void Enqueue(T* Element){
        m_queue.push(Element);
    };
    T* Dequeue(){
        if (!m_queue.empty()) {
            auto value = m_queue.front();
            m_queue.pop();
            return value;
        }
        return nullptr;
    };
    decltype(m_queue.size()) size(){
        return m_queue.size();
    }
};

class QueueTest: public testing::Test
{
private:
    /* data */
protected:
    void SetUp()override{
        q1.Enqueue(new int(1));
        q2.Enqueue(new int(2));
        q2.Enqueue(new int(3));
    };
    //void TearDown() override{}

    Queue<int>q0;
    Queue<int>q1;
    Queue<int>q2;
};

TEST_F(QueueTest, IsIntialEmpty) {
    EXPECT_EQ(q0.size(), 0);
}

TEST_F(QueueTest, DequeueWorks){
    int* n = q0.Dequeue();
    EXPECT_EQ(n, nullptr);

    n = q1.Dequeue();
    ASSERT_NE(n, nullptr);
    EXPECT_EQ(*n, 1);
    EXPECT_EQ(q1.size(), 0);
    delete n;

    n = q2.Dequeue();
    ASSERT_NE(n, nullptr);
    EXPECT_EQ(*n, 2);
    EXPECT_EQ(q2.size(), 1);
    delete n;
}



int main (int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}