/*
 *
 * 对于c++11以前的标准是非线程安全的(局部静态变量)，标准并未规定多线程中对于静态局部变量的处理
 * 那很自然的办法就是加锁
 * 于乱序执行的影响，即由于编译器的优化等，会导致程序实际执行的顺序发生变化，DCL也不能完全保证线程安全
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
