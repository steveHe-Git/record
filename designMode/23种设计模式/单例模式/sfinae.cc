#include <iostream>
using namespace std;

template<typename T>
struct has_no_destroy {
    template<typename C>
    static char test(decltype(&C::no_destroy));


    template<typename C>
    static int32_t test(...);

    const static bool value = sizeof(test<T>(0)) == 1;
};
// 其作用就是用来判断是否有 no_destroy 函数

struct A {

};

struct B {
    void no_destroy(){}
};
struct C {
    int no_destroy;
};

struct D : B {

};

int main() {
    printf("%d\n",has_no_destroy<A>::value);
    printf("%d\n",has_no_destroy<B>::value);
    printf("%d\n",has_no_destroy<C>::value);
    printf("%d\n",has_no_destroy<D>::value);
}