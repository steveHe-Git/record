/*
* 实现简易的线程池
*/

#include <iostream>
#include <thread>
#include <assert.h>
#include <functional>
#include <sstream>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <string>
#include <deque>
using namespace std;

class ThreadPool
{
public:
    typedef std::function<void()> taskCallBack;
    typedef std::vector<std::thread*>threadPtrVector;
    typedef std::deque<taskCallBack> taskDeque;

    bool isStart(){
        return m_isStart;
    }

    void start(){
        std::cout << "#########threadPool start##########" << endl;
        assert(m_threadPtrVector.empty());
        m_threadPtrVector.reserve(m_initCount);
        m_isStart = true;
        for (int i=0 ; i < m_initCount; i++) {
            std::cout << m_initCount;
            m_threadPtrVector.push_back(new std::thread(std::bind(&ThreadPool::threadLoop, this)));
        }
    }

    void stop(){
        std::cout << "#########threadPool free##########" << endl;
        std::unique_lock<std::mutex> lock(m_mutex);
        m_isStart = false;
        m_cond.notify_all();

        //避免成为僵尸线程
        for(threadPtrVector::iterator it = m_threadPtrVector.begin(); it != m_threadPtrVector.end(); it++) {
            (*it)->join();
            delete (*it);
        }
        m_threadPtrVector.clear();
    }

    void addTask(taskCallBack task){
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_isStart) {
            m_taskDeque.push_back(task);
            m_cond.notify_one();
        }
    }

    taskCallBack getTask(){
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_taskDeque.empty() && m_isStart) {
            std::cout << "thread[" << getThreadTid() << "]" << "wait." <<endl;
            m_cond.wait(lock);
        }
        
        std::cout << "thread[" << getThreadTid() << "]" << "wake up." <<endl;
        taskCallBack task;
        if (!m_taskDeque.empty() && m_isStart) {
            task = m_taskDeque.front();
            m_taskDeque.pop_front();
        }
        return task;
    }

    void threadLoop(){
        std::cout << "thread[" << getThreadTid() << "]" << "start." <<endl;
        while (m_isStart) {
            taskCallBack task = getTask();
            if (task) {
                task();
            }
        }
        std::cout << "thread[" << getThreadTid() << "]" << "exit." <<endl;
    }

    static std::string getThreadTid() {
        stringstream tmp;
        tmp << std::this_thread::get_id();
        return tmp.str();
    }

    ThreadPool(int initCount = 3):m_initCount(initCount),
        m_isStart(false),
        m_mutex(),
        m_cond(){
            start();
        }
    ~ThreadPool(){}

private:
    ThreadPool(const ThreadPool&){}
    const ThreadPool& operator=(const ThreadPool&){}

    bool m_isStart;
    int m_initCount;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    threadPtrVector m_threadPtrVector;
    taskDeque m_taskDeque;
};

std::mutex global_mutex;
void testfun() {
    for (int i = 0; i < 4; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(global_mutex);
        std::cout << "testFunc() [" << i << "] at thread [ " << std::this_thread::get_id() << "]" << std::endl;
    }
}

int main () {
    ThreadPool threadPool;
    for (int i = 0 ; i < 10; i++) {
        threadPool.addTask(testfun);
    }
    getchar();
    return 0;
}

