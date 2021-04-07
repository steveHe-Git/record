#include <iostream>
#include <vector>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <string>
#include <sstream>
#include <assert.h>
using namespace std;

class ThreadPool
{
public:
    ThreadPool(int initCount = 3):m_initCount(initCount),
        m_mutex(),
        m_cond(),
        m_isStart(false){
        Start();
    }
    ~ThreadPool(){}

    typedef std::function<void()> task_callBack_;
    void stop(){
        std::unique_lock<std::mutex> lock(m_mutex);
        m_isStart = false;
        m_cond.notify_all();

        for (ThreadPtrVector::iterator it = m_threadPtrVector.begin(); it != m_threadPtrVector.end(); it++ ) {
            //防止变成僵尸线程
            (*it)->join();
            delete *it;
        }
        m_threadPtrVector.clear();
    }
    void add_task(const task_callBack_& task){
        std::unique_lock<std::mutex> lock(m_mutex);
        //when m_taskDeque have maxsize
        // while (m_taskDeque.isFull())
        // {
        //     m_cond.notify_one();
        // }
        m_taskDeque.push_back(task);
        m_cond.notify_one();
    }

private:
    /* data */
    ThreadPool(const ThreadPool&){}
    const ThreadPool& operator=(const ThreadPool&){}

    bool IsStart() {}
    void Start(){
        assert(m_threadPtrVector.empty());
        m_isStart = true;
        m_threadPtrVector.reserve(m_initCount);
        for (int i=0; i < m_initCount; i++) {
            m_threadPtrVector.push_back(new thread(std::bind(&ThreadPool::ThreadLoop, this)));
        }
    }

    void ThreadLoop() {
        std::cout << "thread_pool::threadLoop() tid : " + GetThreadTid() + " start."<< endl;
        while (m_isStart)
        {
            task_callBack_ task = GetTask();
            if (task) {
                task();
            }
        }
        std::cout << "thread_pool::threadLoop() tid : " + GetThreadTid() + " exit."<< endl;
    }

    task_callBack_ GetTask(){
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_taskDeque.empty() && m_isStart)
        {
            std::cout << "thread_pool::threadLoop() tid : " + GetThreadTid() + " wait."<< endl;
            m_cond.wait(lock);
        }

        std::cout << "thread_pool::threadLoop() tid : " + GetThreadTid() + " wakeup."<< endl;
        task_callBack_ task;
        TaskDeque::size_type size = m_taskDeque.size();
        if (!m_taskDeque.empty() && m_isStart) {
            task = m_taskDeque.front();
            m_taskDeque.pop_front();
        }

        return task;
    }

    typedef std::vector<std::thread*> ThreadPtrVector;
    typedef std::deque<task_callBack_> TaskDeque;

    ThreadPtrVector m_threadPtrVector;
    TaskDeque m_taskDeque;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    bool m_isStart;
    int m_initCount;

    static std::string GetThreadTid() {
        std::stringstream tmp;
        tmp << std::this_thread::get_id();
        return tmp.str();
    }
};


std::mutex g_mutex;
void priorityFunc(){
    for (int i = 1; i < 4; ++i){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(g_mutex);
        std::cout << "priorityFunc() [" << i << "] at thread [ " << std::this_thread::get_id() << "] output" << std::endl;
    }
}

void testFunc(){
  // loop to print character after a random period of time
  for (int i = 1; i < 4; ++i){
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::lock_guard<std::mutex> lock(g_mutex);
      std::cout << "testFunc() [" << i << "] at thread [ " << std::this_thread::get_id() << "] output" << std::endl;
    }
}

// int main() {
//     ThreadPool thread_pool;

//     for(int i = 0; i < 5 ; i++) {
//         thread_pool.add_task(testFunc);
//         thread_pool.add_task(priorityFunc);
//     }

//     getchar();
//     return 0;
// }
