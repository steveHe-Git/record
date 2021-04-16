/*
*  可以使用宏实现函数多个形参的委托
*/

#include <iostream>

#define DECLARE_PARAMS(...) __VA_ARGS__
#define DECLARE_ARGS(...) __VA_ARGS__
 
//0个参数的委托
#define DELEGATE0(retType, name) \
    DECLARE_DELEGATE(retType, name, DECLARE_PARAMS(void), )
 
//1个参数的委托
#define DELEGATE1(retType, name, p1) \
    DECLARE_DELEGATE( \
        retType, \
        name, \
        DECLARE_PARAMS(p1 a), \
        DECLARE_ARGS(a))

//2个参数的委托
#define DELEGATE2(retType, name, p1, p2) \
    DECLARE_DELEGATE( \
        retType, \
        name, \
        DECLARE_PARAMS(p1 a, p2 b), \
        DECLARE_ARGS(a, b))

//3个参数的委托
#define DELEGATE3(retType, name, p1, p2, p3) \
    DECLARE_DELEGATE( \
        retType, \
        name, \
        DECLARE_PARAMS(p1 a, p2 b, p3 c), \
        DECLARE_ARGS(a, b, c))

//4个参数的委托
#define DELEGATE4(retType, name, p1, p2, p3, p4) \
    DECLARE_DELEGATE( \
        retType, \
        name, \
        DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d), \
        DECLARE_ARGS(a, b, c, d))

//5个参数的委托
#define DELEGATE5(retType, name, p1, p2, p3, p4, p5) \
    DECLARE_DELEGATE( \
        retType, \
        name, \
        DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d, p5 e), \
        DECLARE_ARGS(a, b, c, d, e))

//6个参数的委托
#define DELEGATE6(retType, name, p1, p2, p3, p4, p5, p6) \
    DECLARE_DELEGATE( \
        retType, \
        name, \
        DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d, p5 e, p6 f), \
        DECLARE_ARGS(a, b, c, d, e, f))

//7个参数的委托
#define DELEGATE7(retType, name, p1, p2, p3, p4, p5, p6, p7) \
    DECLARE_DELEGATE( \
        retType, \
        name, \
        DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d, p5 e, p6 f, p7 g), \
        DECLARE_ARGS(a, b, c, d, e, f, g))

//8个参数的委托
#define DELEGATE8(retType, name, p1, p2, p3, p4, p5, p6, p7, p8) \
    DECLARE_DELEGATE( \
        retType, \
        name, \
        DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d, p5 e, p6 f, p7 g, p8 h), \
        DECLARE_ARGS(a, b, c, d, e, f, g, h))


#define DECLARE_DELEGATE(retType, name, params, args)                         \
class Base##name {                                                            \
public:                                                                       \
    virtual ~Base##name() { }                                                 \
    virtual retType Invoke(params) = 0;                                       \
};                                                                            \
template<typename T>                                                          \
class name : public Base##name {                                              \
public:                                                                       \
    name(T* pType, retType (T::*pFunc)(params))                               \
        : m_pType(pType), m_pFunc(pFunc) { }                                  \
    retType Invoke(params) {                                                  \
        return (m_pType->*m_pFunc)(args);                                     \
    }                                                                         \
private:                                                                      \
    T* m_pType; retType (T::*m_pFunc)(params);                                \
};                                                                            \
template<>                                                                    \
class name<void> : public Base##name {                                        \
public:                                                                       \
    name(retType (*pFunc)(params))                                            \
        : m_pFunc(pFunc) { }                                                  \
    retType Invoke(params) {                                                  \
        return (*m_pFunc)(args);                                              \
    }                                                                         \
private:                                                                      \
    retType (*m_pFunc)(params);                                               \
}

DELEGATE0(void, Test0);
DELEGATE1(void, Test1, int);
DELEGATE2(void, Test2, int, int);

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

void print(){
    std::cout << "Just 0 arg test" << std::endl;
}

int main(int argc, char *argv[])
{
    A af;
    B bf;
    Test0<void> t0(print);
    t0.Invoke();

    Test1<A> test(&af, &A::Afun);
    test.Invoke(5);

    Test1<void> tet(method);
    tet.Invoke(10);

    return 0;
}