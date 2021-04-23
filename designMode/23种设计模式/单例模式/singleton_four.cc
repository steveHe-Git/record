/*
 * 给出一个在linux系统上可用的通过pthread_once来实现的线程安全的单例模式,通过pthread库来保证初始化只被执行一次
 * 从而实现c++11 之前的单例也是线程安全的
 * 在多线程编程环境下，尽管pthread_once()调用会出现在多个线程中，init_routine()函数仅执行一次，pthread_once是很适合用来实现线程安全单例。
 */

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <cassert>
#include <new>

class noncopyable {
protected:
    // 默认的构造函数和析构函数是 protected，
    // 不允许创建 noncopyable 实例，但允许子类创建实例
    // （即允许派生类构造和析构）。
    noncopyable() = default;
    ~noncopyable() = default;

private:
    // 使用 delete 关键字禁止编译器自动产生复制构造函数和复制赋值操作符。
    noncopyable(const noncopyable&) = delete;
    // 移动构造函数
    noncopyable(noncopyable&&) = delete;
    const noncopyable& operator=(const noncopyable&) = delete;
};

//判断T 是否有Destory函数
template<typename T>
struct isHasDestory {
    template<typename U>
    static constexpr bool Test(decltype(&U::Destory)*) {
       return true; 
    }

    template<typename U>
    static constexpr bool Test(...) {
       return false; 
    }

    static constexpr bool value = Test<T>(nullptr);
};
template <typename T>                                   
constexpr bool isHasDestory<T>::value;

template<typename T>
typename std::enable_if<!isHasDestory<T>::value>::type CallDestory(T* instance) {
    (void)instance;
}

template<typename T>
typename std::enable_if<isHasDestory<T>::value>::type CallDestory(T* instance) {
    instance->Destory();
}

template <typename T>
class Singleton:private noncopyable
{
public:

    static T& Instance() {
        if (!instance) {
            try{
                pthread_once(&once_flag, [&]{
                    //std::nothrow 在new空间不够的时候会返回空，并且不会扔出异常
                    instance = new(std::nothrow) T();

                    if (instance) {
                        std::atexit(destorySelf);
                    }
                });
            } catch(std::system_error e) {
                std::cout << e.what() << std::endl;
            }
        }
        return *instance;
    }

private:
    
    static void destorySelf() {
        //call T's Destory
        CallDestory(instance);


        //如果T是不完整类型(只是声明没有定义的类)，char[-1]编译器将会报错，编译不通过
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy;
        (void)dummy;

        delete instance;
        instance = nullptr;
    }

    static T* instance;
    static pthread_once_t once_flag;
};

template <typename T>
T* Singleton<T>::instance = nullptr;

template <typename T>
pthread_once_t Singleton<T>::once_flag = PTHREAD_ONCE_INIT;

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
