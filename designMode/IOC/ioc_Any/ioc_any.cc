/*
 * 使用擦除和Any实现ioc
 * 擦出类型常用的方法
 * 1.多态(基类类型约束)
 * 2.模板(缺点是容器的类型T必须要指定)
 * 3.类型容器(Variant可以承载指定的某些类型)
 * 4.通用类型擦除(Any类型可以满足使用)
 * 5.闭包 (例如使用lambda表达式封装起来)
 * 本次实现的Ioc容器主要使用Any和闭包擦除技巧实现。
 */

#include "Singleton.hpp"
//#include "Any.hpp"
#include "AnyRewrite.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <type_traits>

using namespace std;

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

struct D
{
	D(A* ptr) : m_ptr(ptr) {}

	virtual void fun_print() {
		m_ptr->fun_print();
	}

    ~D() {
        if (m_ptr != nullptr) {
            delete m_ptr;
            m_ptr = nullptr;
        }
    }

private:
	A* m_ptr;
};

class IocAny
{
private:
    /* data */
    void Register(string key, Any func) {
        if (m_map.find(key) != m_map.end()) {
            throw std::invalid_argument("this key is already exist");
        }

        m_map.emplace(key, func);
    }

    std::map<std::string, Any>m_map;

    template<typename Base, typename Depend>
    typename std::enable_if<!std::is_same<Depend, void>::value>::type RegisterType(string key) {
        std::function<Base*()> func = []{
            cout << "first" <<endl;
            return new Base(new Depend());
        };
        std::cout << "Base is not father of Depend" << std::endl;
        Register(key, func);
    }

    template<typename Base>
    void RegisterType(string key) {
        std::function<Base*()> func = []{
            return new Base();
        };
        std::cout << "Base is father of Depend" << std::endl;
        Register(key, func);
    }

    template<typename T>
    std::shared_ptr<T> ResolveShared(string key) {
        T* t = resolve<T>(key);
        return std::shared_ptr<T>(t);
    }

    template<class T>
	T* resolve(string key) {
        if (m_map.find(key) == m_map.end()) {
            return nullptr;
        }

        Any func = m_map[key];

        std::function<T*()> f = func.AnyCast<std::function<T*()>>();

        return f();
    }

public:
    IocAny(/* args */){}
    ~IocAny(){}

    template<typename U, typename K>
    std::shared_ptr<U> getRegisterShared(string key) {
        if (m_map.find(key) == m_map.end()) {
            RegisterType<U, K>(key);
        }
        return ResolveShared<U>(key);
    }
};

int main () {
    auto Ioc = Singleton<IocAny>::Instance();

    // Ioc.RegisterType<D, B>("B");
    // Ioc.RegisterType<D, C>("C");
    // auto DeviceB = Ioc.ResolveShared<D>("B");
    // auto DeviceC = Ioc.ResolveShared<D>("C");
    // DeviceB->fun_print();
    // DeviceC->fun_print();

    auto DeviceB = Ioc.getRegisterShared<D, B>("B");
    auto DeviceC = Ioc.getRegisterShared<D, C>("C");
    DeviceB->fun_print();
    DeviceC->fun_print();

    // Ioc.RegisterType<B>("B");
    // Ioc.RegisterType<C>("C");
    // auto deviceA = Ioc.ResolveShared<B>("B");
    // auto deviceB = Ioc.ResolveShared<C>("C");
    // deviceA->fun_print();
    // deviceB->fun_print();

    return 0;
}