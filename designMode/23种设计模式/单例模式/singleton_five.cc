/*
 * 使用原子锁实现多线程模式下的单例模式(线程安全的)
 */
#include <iostream>
#include <memory>
#include <atomic>
#include <mutex>

class noncopyable
{
private:
    /* data */
    noncopyable(const noncopyable&) = delete;
    noncopyable(noncopyable&&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
protected:
    noncopyable(/* args */) = default;
    ~noncopyable() = default;
};

template<typename T>
class Singleton:private noncopyable
{
private:
    /* data */
    static T* m_instance;
    static std::mutex m_mutex;
public:
    Singleton(/* args */){}
    ~Singleton(){}

    static T& Instance() {
        std::atomic_thread_fence(std::memory_order_acquire);
        if (!m_instance) {
            std::lock_guard<std::mutex>lock(m_mutex);
            if (!m_instance) {
                m_instance = new(std::nothrow) T();
                std::atomic_thread_fence(std::memory_order_release);
            }
        }
        return *m_instance;
    }
};

template<typename T>
T* Singleton<T>::m_instance = nullptr;

template<typename T>
std::mutex Singleton<T>::m_mutex;

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
