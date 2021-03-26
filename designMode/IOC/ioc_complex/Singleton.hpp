#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <iostream>
#include <memory>

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
    const noncopyable& operator=(const noncopyable&) = delete;
};

template<typename T>
class Singleton:public noncopyable
{
public:
    static T& getInstance() {
        if (！value_) {
            //在内存不足时，new(std::nothrow)并不抛出异常，而是将指针置NULL, std::nothrow可以实现对非零指针的检查;
            value_ = new(std::nothrow) T();

            if (value_) {
                std::atexit(destory);   //程序结束时调用注册的函数
            }
        }
    }

    static void destory() {

        /*
        * C++标准允许通过一个 delete 表达式删除指向不完全类的指针。如果该类有一个非平凡的析构函数，或者有一个类相关的 delete 操作符，
        * 那么其行为就是无定义的，因此编译器作了这种扩展，以将这种未定义的行为转为编译期错误，帮助程序员们及早发现
        * incomplete type呢，就是那些声明了，但没有定义的类型 class A; extern A a;
        */
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy;
        void(dummy);

        delete value_;
        value_ = nullptr;
    }

private:
    static T* value_;
};

template<typename T>
T* Singleton<T>::value_ = nullptr;

#endif