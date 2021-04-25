/*
 * 使用擦除和Any实现ioc(多参数版本)
 * 擦出类型常用的方法
 * 1.多态(基类类型约束)
 * 2.模板(缺点是容器的类型T必须要指定)
 * 3.类型容器(Variant可以承载指定的某些类型)
 * 4.通用类型擦除(Any类型可以满足使用)
 * 5.闭包 (例如使用lambda表达式封装起来)
 * 本次实现的Ioc容器主要使用Any和闭包擦除技巧实现。
 */

#include <iostream>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Singleton.hpp"
#include "Any.hpp"

using namespace std;

class Base
{
private:
    /* data */
public:
    Base(/* args */){}
    virtual ~Base(){}
public:
    virtual void print(){
        cout << "Base print" << endl;
    }
};

class A:public Base
{
private:
    /* data */
public:
    A(/* args */){}
    ~A(){}
    virtual void print(){
        cout << "A print" << endl;
    }
};

class B:public Base
{
private:
    /* data */
public:
    B(/* args */){}
    ~B(){}
    virtual void print(){
        cout << "B print" << endl;
    }
};

class C:public Base
{
private:
    /* data */
    int m_a;
    double m_b;
public:
    C(int a, double b):m_a(a),m_b(b){}
    ~C(){}
    virtual void print(){
        cout << "a+b= " << m_a + m_b << endl;
    }
};

class Bridge
{
private:
    Base* base_ptr;
public:
    Bridge(Base* ptr):base_ptr(ptr){

    }

    void print() {
        base_ptr->print();
    }

    ~Bridge(){
        if (base_ptr) {
            delete base_ptr;
            base_ptr = nullptr;
        }
    }
};

class IocAnyMultiParam
{
private:
    /* data */
    std::unordered_map<string, Any>m_map;

    void Register(string key, Any func) {
        if (m_map.find(key) != m_map.end()) {
            throw invalid_argument("this key is exists");
        }

        m_map.emplace(key, func);
    }

    template<class T, typename... Args>
    T* resolve(string key, Args... args) {
        if (m_map.find(key) == m_map.end()) {
            return nullptr;
        }

        Any func = m_map[key];

        std::function<T*(Args...)> f = func.AnyCast<std::function<T*(Args...)>>();

        return f(args...);
    }

public:
    IocAnyMultiParam(/* args */){}
    ~IocAnyMultiParam(){}

    //不具有继承关系
    template<class Base, class Depend, typename... Args>
    typename std::enable_if<!is_base_of<Base, Depend>::value>::type
    RegisterType(string key) {
        std::function<Base*(Args...)> func = [](Args... args){
            return new Base(new Depend(args...));
        };
        cout << "Base is not father of depend" <<endl;
        Register(key, func);
    }

    //具有继承关系
    template<class Base, class Depend, typename... Args>
    typename std::enable_if<is_base_of<Base, Depend>::value>::type
    RegisterType(string key) {
        std::function<Base*(Args...)> func = [](Args... args){
            return new Base(args...);
        };
        Register(key, func);
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> ResolveShared(string key, Args... args) {
        T* t = resolve<T>(key, args...);
        return std::shared_ptr<T>(t);
    }
};


int main () {

    auto Ioc = Singleton<IocAnyMultiParam>::Instance();

    //第2种
    // Ioc.RegisterType<Bridge, A>("A");
    // Ioc.RegisterType<Bridge, B>("B");
    // auto deviceA = Ioc.ResolveShared<Bridge>("A");
    // auto deviceB = Ioc.ResolveShared<Bridge>("B");
    // deviceA->print();
    // deviceB->print();

    //第2种
    // Ioc.RegisterType<A>("A");
    // Ioc.RegisterType<B>("B");
    // auto deviceA = Ioc.ResolveShared<A>("A");
    // auto deviceB = Ioc.ResolveShared<B>("B");
    // deviceA->print();
    // deviceB->print();

    //第3种
    Ioc.RegisterType<Bridge, C, int, double>("C");
	auto pa = Ioc.ResolveShared<Bridge>("C", 1, 2.0);
    pa->print();

    return 0;
}
