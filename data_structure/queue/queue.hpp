/*
* c++中典型的单链表是queue; STL中queue的用法如下；
* 
*/

#ifndef QUEUE_HPP
#define QUEUE_HPP

// #include <queue>
// #include <iostream>
// using namespace std;
// int main() {
//     int tmp;
//     queue<int> iqueue;

//     //加入元素
//     iqueue.push(10);
//     iqueue.push(20);
//     iqueue.push(30);
//     iqueue.emplace(40);

//     // 删除队列开头的元素
//     iqueue.pop();

//     //获取开头元素但不删除
//     tmp = iqueue.front();

//     cout << "empty:" << iqueue.empty();
//     cout << "size:" << iqueue.size();

//     while (!iqueue.empty())
//     {
//         tmp = iqueue.front();
//         iqueue.pop();
//     }
    
//     //交换两个队列
//     queue<int> aqueue;
//     aqueue.push(10);
//     aqueue.push(20);

//     queue<int> bqueue;
//     bqueue.push(10);
//     bqueue.push(20);
//     bqueue.push(30);

//     swap(aqueue, bqueue);
// }


#endif