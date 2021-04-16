/*
* 委托模式可以解决观察者模式的弊端，使两个毫不相关的类的成员函数或者普通函数可以通过信号同时触发
* 将不同的函数实现委托，一起触发(限制条件是委托在一起的函数的传入的形参类型和个数必须一致)
*
*  类成员函数指针的操作
*
class  A {
public :
     void  Func( int ) { … }
};
要取得Func函数的指针，必须这么做：
void  (A::*pFunc)( int ) = &A::Func;

调用：
A a;
(a.*pFunc)(10);
A* pa = &a;
(pa->*pFunc)(11);

*/

#include <iostream>
#include <vector>

class A
{
private:
    /* data */
public:
    A(/* args */){}
    ~A(){}
    void Afun(int value) {
        std::cout << "delegate A ==> " << value <<std::endl;
    }
};

class B
{
private:
    /* data */
public:
    B(/* args */){}
    ~B(){}
    void Bfun(int value) {
        std::cout << "delegate B ==>" << value <<std::endl;
    }
};

void method(int value) {
    std::cout << "delegate void ==>" << value <<std::endl;
}

class Delegate
{
private:
    /* data */
public:
    Delegate(/* args */){}
    virtual~Delegate(){}
    virtual void Invoke(int value) {}
};

template<typename T>
class DelegateHandle:public Delegate
{
private:
    /* data */
    void (T::*m_fun)(int);
    T* m_ptr;
public:
    DelegateHandle(T* ptr, void (T::*fun)(int)):m_fun(fun), m_ptr(ptr){}
    ~DelegateHandle(){}
    void Invoke(int value) {
        (m_ptr->*m_fun)(value);
    }
};

template<>
class DelegateHandle<void>:public Delegate
{
private:
    /* data */
    void (*m_fun)(int);
public:
    DelegateHandle(void (*fun)(int)):m_fun(fun){}
    ~DelegateHandle(){}
    void Invoke(int value) {
        (*m_fun)(value);
    }
};

int main () {
    A a;
    B b;
    DelegateHandle<A>da(&a, &A::Afun);
    DelegateHandle<B>db(&b, &B::Bfun);
    DelegateHandle<void>dvoid(method);

    std::vector<Delegate*> Delegate_vectorl;
    Delegate_vectorl.push_back(&da);
    Delegate_vectorl.push_back(&db);
    Delegate_vectorl.push_back(&dvoid);

    for(auto delegate : Delegate_vectorl) {
        delegate->Invoke(6);
    }

    return 0;
}




