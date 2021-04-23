#include "Singleton.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <memory>

struct A
{
    virtual void fun_print() {std::cout << "A is print" << std::endl;}
    virtual ~A(){}
};

struct B:public A
{
    virtual void fun_print() {std::cout << "B is print"<< std::endl;}
    virtual ~B(){}
};

struct C:public A
{
    virtual void fun_print() {std::cout << "C is print"<< std::endl;}
    virtual ~C(){}
};

template<typename T>
class IocSimple
{
private:
    /* data */
    void RegisterType(std::string key, std::function<T*()> CallBack) {
        if (KeyValueMap.find(key) != KeyValueMap.end()) {
            throw std::invalid_argument("this key is already exist");
        }
        KeyValueMap.emplace(key, CallBack);
    }
public:
    template<class ClassType>
    void RegisterType(std::string key) {
        //lamba表达式 BackFun实际是一个函数名，函数的返回值类型为T* 
        std::function<T*()> BackFun = [] {return new ClassType();};
        RegisterType(key, BackFun);
    }

    T* Resolve(std::string key) {
        if (KeyValueMap.find(key) == KeyValueMap.end()) {
            return nullptr;
        }
        //KeyValueMap[key]()实际就是BackFun()==>BackFun()就相当于执行了lamba的表达式
        return KeyValueMap[key]();
    }

    std::shared_ptr<T> ResolveShared(std::string key) {
        T* ptr = Resolve(key);
        return std::shared_ptr<T>(ptr);
    }

    std::map<std::string, std::function<T*()>> KeyValueMap;

    IocSimple(/* args */){}
    ~IocSimple(){}
};

int main () {
    auto Ioc = Singleton<IocSimple<A>>::Instance();
    Ioc.RegisterType<B>("B");
    Ioc.RegisterType<C>("C");
    
    std::shared_ptr<A> DeviceB = Ioc.ResolveShared("B");
    DeviceB->fun_print();

    std::shared_ptr<A> DeviceC = Ioc.ResolveShared("C");
    DeviceC->fun_print();

    return 0;
}