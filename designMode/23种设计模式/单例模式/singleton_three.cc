/*
 * 给出一个在linux系统上可用的通过pthread_once来实现的线程安全的单例模式,通过pthread库来保证初始化只被执行一次
 * 从而实现c++11 之前的单例也是线程安全的
 * 在多线程编程环境下，尽管pthread_once()调用会出现在多个线程中，init_routine()函数仅执行一次，pthread_once是很适合用来实现线程安全单例。
 */

#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

template<typename T>
class Singleton
{
private:
    /* data */
    Singleton(const T&) = delete;
    Singleton(T&&) = delete;
    void operator=(const T&) = delete;

    static pthread_once_t one;
    static T* m_instance;
    static void init() {
        m_instance = new T();
    }
protected:
    Singleton(/* args */) = default;
    ~Singleton() = default;
public:

    static T& Instance() {
        //pthread_once保证init只初始化一次;
        pthread_once(&one, &Singleton<T>::init);
        return *m_instance;
    }
};

template<typename T>
pthread_once_t Singleton<T>::one = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::m_instance = nullptr;

class A
{
private:
    /* data */
public:
    A(/* args */){}
    ~A(){}
};


int main() {
    //左值引用
    A& A1 = Singleton<A>::Instance();
    A& A2 = Singleton<A>::Instance();

    std::cout << "a1: " << &A1 <<"   a2:" << &A2 <<std::endl;
    return 0;
}