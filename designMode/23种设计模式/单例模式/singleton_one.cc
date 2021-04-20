/* 
 *
 * Scott Meyers 在 Effective C++ 的 Item 4: Make sure that objects are initialized before they're used 
 * 里面提出了一种利用 C++ 的 static 关键字来实现的单例模式，这种实现非常简洁高效，它的特点是：
 * 1. 仅当程序第一次执行到 GetInstance 函数时，执行 instance 对象的初始化；
 * 2. 在 C++ 11 之后，被 static 修饰的变量可以保证是线程安全的；
 *  函数内的静态局部变量在第一次调用时初始化(只初始化一次)
*/

#include <iostream>
#include <memory>
#include <mutex>

template<typename T>
class Singleton
{
private:
    /* data */
    //拷贝构造
    Singleton(const T&) = delete;
    //移动构造
    Singleton(T&&) = delete;
    //赋值
    T& operator=(const T&) = delete;
private:
    Singleton(/* args */) = default;
    virtual ~Singleton() = default;
public:
    static T& Instance() {
        //调用构造函数
        static T m_instance;
        std::cout << "Instance" <<std::endl;
        return m_instance;
    }
};

class A
{
private:
    /* date */
public:
    int data;
    //构造函数
    A(/* args */){
        data = -1;
        std::cout << "Afun(): " << this <<std::endl;
    }

    //拷贝构造函数
    A(const A&){
        std::cout << "Afun()&: " << this <<std::endl;
    }

    ~A(){
        std::cout << "~Afun(): " << this <<std::endl;
    }

    void Afun() {
        std::cout << "Afun: " << data <<std::endl;
    }
};

class B
{
private:
    /* data */
public:
    B(/* args */){}
    ~B(){}
    void Bfun() {
        std::cout << "Bfun" << std::endl;
    }
};


int main () {
    //下面的这4行会分别调用A的拷贝构造函数，会产生两个不同的对象
    // auto A1_single = Singleton<A>::Instance();
    // std::cout << "A1_single" <<std::endl;
    // auto A2_single = Singleton<A>::Instance();
    // std::cout << "A2_single" <<std::endl;

    //左值引用，不会调用A的拷贝构造函数
    A& A1_single = Singleton<A>::Instance();
    std::cout << "A1_single" <<std::endl;
    A& A2_single = Singleton<A>::Instance();
    std::cout << "A2_single" <<std::endl;


    std::cout << "a1: " << &A1_single <<"   a2:" << &A2_single <<std::endl;
    return 0;
}