/*
 *
 * 对于c++11以前的标准是非线程安全的(局部静态变量)，标准并未规定多线程中对于静态局部变量的处理
 * 那很自然的办法就是加锁
 * 于乱序执行的影响，即由于编译器的优化等，会导致程序实际执行的顺序发生变化，DCL也不能完全保证线程安全
 * 分析：m_instance = new T()这句话可以分成三个步骤来执行：
 *
 * 1. 分配了一个T类型对象所需要的内存。
 * 2. 在分配的内存处构造T类型的对象。
 * 3. 把分配的内存的地址赋给指针m_instance。
 * 
 * 可能会认为这三个步骤是按顺序执行的，但实际上只能确定步骤1是最先执行的，
 * 步骤2，3却不一定。问题就出现在这。假如某个线程A在调用执行m_instance = new T()的时候是按照1,3,2的顺序的，
 * 那么刚刚执行完步骤3给T类型分配了内存（此时m_instance就不是nullptr了）就切换到了线程B，由于m_instance已经不是nullptr了，
 * 所以线程B会直接执行return m_instance得到一个对象，而这个对象并没有真正的被构造！！严重bug就这么发生了。
 */

#include <iostream>
#include <memory>
#include <mutex>

template<typename T>
class Singleton
{
private:
    /* data */
    Singleton(const T&) = delete;
    Singleton(T&&) = delete;   
    static std::mutex m_mutex;
    static T* m_instance;
protected:
    /* data */
    Singleton(/* args */) = default;
    virtual ~Singleton() = default;
public:
    //方式1
    // static T* Instance() {
    //     if (!m_instance) {
    //         std::lock_guard<std::mutex> lock(m_mutex);
    //         if (!m_instance) {
    //             m_instance = new T();
    //         }
    //     }
    //     return m_instance;
    // }
    
    //方式2
    static T& Instance() {
        if (!m_instance) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_instance) {
                m_instance = new T();
            }
        }
        return *m_instance;
    }
};

template<typename T>
std::mutex Singleton<T>::m_mutex;

template<typename T>
T* Singleton<T>::m_instance = nullptr;

class A
{
private:
    /* data */
public:
    A(/* args */){
        std::cout << "A(): "<< this <<std::endl;
    }

    A(const A&){
        std::cout << "A&: "<< this <<std::endl;
    }

    void operator=(const A&){
        std::cout << "A =: "<< this <<std::endl;
    }

    ~A(){
        std::cout << "~A(): "<< this <<std::endl;
    }
};



int main() {
    //方式1
    //指针的赋值，不会调用A的任何构造函数；
    // A* A1 = Singleton<A>::Instance();
    // A* A2 = Singleton<A>::Instance();

    //方式2
    //左值引用
    A& A1 = Singleton<A>::Instance();
    A& A2 = Singleton<A>::Instance();

    std::cout << "a1: " << &A1 <<"   a2:" << &A2 <<std::endl;

    return 0;
}
