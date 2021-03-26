# c++ 随笔记录

## 1. C++11 中的std::function和std::bind

>std::function

```cpp
std::function 是一个可调用对象包装器，是一个类模板，可以容纳除了类成员函数指针之外的所有可调用对象，它可以用统一的方式处理函数、函数对象、函数指针，并允许保存和延迟它们的执行
    
// 普通函数
int add(int a, int b){return a+b;} 

// lambda表达式
auto mod = [](int a, int b){ return a % b;}

// 函数对象类
struct divide{
    int operator()(int denominator, int divisor){
        return denominator/divisor;
    }
};
```

```cpp
上述三种可调用对象虽然类型不同，但是共享了一种调用形式：
int(int ,int)
```

```cpp
std::function就可以将上述类型保存起来
std::function<int(int ,int)>  a = add; 
std::function<int(int ,int)>  b = mod ; 
std::function<int(int ,int)>  c = divide(); 
定义格式：std::function<函数类型>
```

>std::bind

```cpp
std::bind绑定普通函数
double my_divide (double x, double y) {return x/y;}
auto fn_half = std::bind (my_divide,_1,2);  
std::cout << fn_half(10) << '\n';                        // 5
std::bind将可调用对象与其参数一起进行绑定，绑定后的结果可以使用std::function保存
std::function<double(double ,double)>half = std::bind (my_divide,_1,2);

bind的第一个参数是函数名，普通函数做实参时，会隐式转换成函数指针。因此std::bind (my_divide,_1,2)等价于std::bind (&my_divide,_1,2)；
_1表示占位符，位于<functional>中，std::placeholders::_1；
```

```cpp
std::bind绑定一个成员函数
struct Foo {
    void print_sum(int n1, int n2)
    {
        std::cout << n1+n2 << '\n';
    }
    int data = 10;
};
int main() 
{
    Foo foo;
    auto f = std::bind(&Foo::print_sum, &foo, 95, std::placeholders::_1);
    f(5); // 100
}

bind绑定类成员函数时，第一个参数表示对象的成员函数的指针，第二个参数表示对象的地址。
必须显示的指定&Foo::print_sum，因为编译器不会将对象的成员函数隐式转换成函数指针，所以必须在Foo::print_sum前添加&；
使用对象成员函数的指针时，必须要知道该指针属于哪个对象，因此第二个参数为对象的地址 &foo；
```

```cpp
std::bind绑定一个引用参数
    默认情况下，bind的那些不是占位符的参数被拷贝到bind返回的可调用对象中。但是，与lambda类似，有时对有些绑定的参数希望以引用的方式传递，或是要绑定参数的类型无法拷贝。
    
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std::placeholders;
using namespace std;

ostream & print(ostream &os, const string& s, char c)
{
    os << s << c;
    return os;
}

int main()
{
    vector<string> words{"helo", "world", "this", "is", "C++11"};
    ostringstream os;
    char c = ' ';
    for_each(words.begin(), words.end(), 
                   [&os, c](const string & s){os << s << c;} );
    cout << os.str() << endl;

    ostringstream os1;
    // ostream不能拷贝，若希望传递给bind一个对象，
    // 而不拷贝它，就必须使用标准库提供的ref函数
    for_each(words.begin(), words.end(),
                   bind(print, ref(os1), _1, c));
    cout << os1.str() << endl;
}
```

> 指向成员函数的指针

```cpp
#include <iostream>
struct Foo {
    int value;
    void f() { std::cout << "f(" << this->value << ")\n"; }
    void g() { std::cout << "g(" << this->value << ")\n"; }
};
void apply(Foo* foo1, Foo* foo2, void (Foo::*fun)()) {
    (foo1->*fun)();  // call fun on the object foo1
    (foo2->*fun)();  // call fun on the object foo2
}
int main() {
    Foo foo1{1};
    Foo foo2{2};
    apply(&foo1, &foo2, &Foo::f);
    apply(&foo1, &foo2, &Foo::g);
}

成员函数指针的定义：void (Foo::*fun)()，调用是传递的实参: &Foo::f；
fun为类成员函数指针，所以调用是要通过解引用的方式获取成员函数*fun,即(foo1->*fun)();
```

## 2. c++中的std::enable_if

   ```cpp
//cpp reference中的实例代码
// enable_if example: two ways of using enable_if
#include <iostream>
#include <type_traits>
// 1. the return type (bool) is only valid if T is an integral type:
template <class T>
    typename std::enable_if<std::is_integral<T>::value,bool>::type
        is_odd (T i) {return bool(i%2);}
// 2. the second template argument is only valid if T is an integral type:
template < class T,
class = typename std::enable_if<std::is_integral<T>::value>::type>
    bool is_even (T i) {return !bool(i%2);}
int main() {
    short int i = 1;    // code does not compile if type of i is not integral
    std::cout << std::boolalpha;
    std::cout << "i is odd: " << is_odd(i) << std::endl;
    std::cout << "i is even: " << is_even(i) << std::endl;
    return 0;
}
   
//if cond is true ,则typedef int type; 如果没有指定int类型默认为type为void类型，
//if cond is true ,则编译出错；
typename std::enable_if<true, int>::type t; //正确
typename std::enable_if<true>::type; //可以通过编译，没有实际用处，推导的模板是偏特化版本，第一模板参数是true，第二模板参数是通常版本中定义的默认类型即void
typename std::enable_if<false>::type; //无法通过编译，type类型没有定义
typename std::enable_if<false, int>::type t2; //同上
   
   
这两个函数如果是普通函数的话，根据重载的规则是不会通过编译的。即便是模板函数，如果这两个函数都能推导出正确的结果，也会产生重载二义性问题，但是正因为std::enable_if的运用使这两个函数的返回值在同一个函数调用的推导过程中只有一个合法，遵循SFINAE原则，则可以顺利通过编译。
    
template <typename T>
typename std::enable_if<std::is_trivial<T>::value>::type SFINAE_test(T value)
{
    std::cout<<"T is trival"<<std::endl;
}

template <typename T>
typename std::enable_if<!std::is_trivial<T>::value>::type SFINAE_test(T value)
{
    std::cout<<"T is none trival"<<std::endl;
}
   
   ```
## 3. C++中构造函数，拷贝构造函数和赋值函数的区别和实现

> 构造函数

```c++
构造函数可以被重载，可以多个，可以带参数；
析构函数只有一个，不能被重载，不带参数
```

> 拷贝构造函数

```c++
拷贝构造函数是C++独有的，它是一种特殊的构造函数，用基于同一类的一个对象构造和初始化另一个对象
在C++中，3种对象需要复制，此时拷贝构造函数会被调用
1）一个对象以值传递的方式传入函数体
2）一个对象以值传递的方式从函数返回
3）一个对象需要通过另一个对象进行初始化 a(b)

什么时候编译器会生成默认的拷贝构造函数：
1）如果用户没有自定义拷贝构造函数，并且在代码中使用到了拷贝构造函数，编译器就会生成默认的拷贝构造函数。但如果用户定义了拷贝构造函数，编译器就不在生成。
2）如果用户定义了一个构造函数，但不是拷贝构造函数，而此时代码中又用到了拷贝构造函数，那编译器也会生成默认的拷贝构造函数。
```

> 深拷贝和浅拷贝

```c++
因为系统提供的默认拷贝构造函数工作方式是内存拷贝，也就是浅拷贝。如果对象中用到了需要手动释放的对象，则会出现问题，这时就要手动重载拷贝构造函数，实现深拷贝。

下面说说深拷贝与浅拷贝：
浅拷贝：如果复制的对象中引用了一个外部内容（例如分配在堆上的数据），那么在复制这个对象的时候，让新旧两个对象指向同一个外部内容，就是浅拷贝。（指针虽然复制了，但所指向的空间内容并没有复制，而是由两个对象共用，两个对象不独立，删除空间存在）
深拷贝：如果在复制这个对象的时候为新对象制作了外部对象的独立复制，就是深拷贝。

拷贝构造函数重载声明如下：
A (const A&other)
```

> 赋值函数

```c++
当一个类的对象向该类的另一个对象赋值时，就会用到该类的赋值函数。
当没有重载赋值函数（赋值运算符）时，通过默认赋值函数来进行赋值操作
赋值运算的重载声明如下：
A& operator = (const A& other)

1. 拷贝构造函数是一个对象初始化一块内存区域，这块内存就是新对象的内存区，而赋值函数是对于一个已经被初始化的对象来进行赋值操作。
A a;
A b=a;   //调用拷贝构造函数（b不存在）
A c(a) ;   //调用拷贝构造函数
 
/****/
class  A;
A a;
A b;   
b = a ;   //调用赋值函数(b存在)

2. 一般来说在数据成员包含指针对象的时候，需要考虑两种不同的处理需求：一种是复制指针对象，另一种是引用指针对象。拷贝构造函数大多数情况下是复制，而赋值函数是引用对象
    
3.实现不一样。拷贝构造函数首先是一个构造函数，它调用时候是通过参数的对象初始化产生一个对象。赋值函数则是把一个新的对象赋值给一个原有的对象，所以如果原来的对象中有内存分配要先把内存释放掉，而且还要检察一下两个对象是不是同一个对象，如果是，不做任何操作，直接返回。
    
4.如果不想写拷贝构造函数和赋值函数，又不允许别人使用编译器生成的缺省函数，最简单的办法是将拷贝构造函数和赋值函数声明为私有函数，不用编写代码
 private:
 A(const A& a); //私有拷贝构造函数
 A& operate=(const A& a); //私有赋值函数

所以如果类定义中有指针或引用变量或对象，为了避免潜在错误，最好重载拷贝构造函数和赋值函数。
一句话记住三者：对象不存在，且没用别的对象来初始化，就是调用了构造函数；
              对象不存在，且用别的对象来初始化，就是拷贝构造函数（上面说了三种用它的情况！）
              对象存在，用别的对象来给它赋值，就是赋值函数。
```

## 4. c++中的右值引用、移动语义和完美转发

>

## 5. C++中继承详解

>继承的权限有三种
1. 私有继承：基类中的非私有成员都为派生类的私有成员
2. 保护继承：基类中的非私有成员在派生类中的访问属性都降一级(公有变私有，保护变私有)
3. 公有继承：基类中的非私有成员在派生类中的访问属性保持不变
注意：基类中的私有成员，子类能继承，但是子类不能直接访问，需要使用父类提供的方法才能访问该变量。改方法是从父类继承中得到的protected、public方法来访问
注意：派生类从基类公有继承时，派生类的成员函数可以直接访问基类的公有成员，但不能访问基类的私有成员
    为了便于派生类的访问，可以将基类的私有成员中需要提供给派生类访问的成员定义为保护成员
    派生类可以访问protected权限的成员但是派生类的对象不能访问基类的成员
4. 访问权限
public：类内、类的对象；派生类内、派生类对象--->>均可访问。
protected：类内、派生类内--->>可以访问；类的对象、派生类的对象-->>不可访问。
private：只有类内部-->>可以访问；类的对象、派生类、派生类的对象，统统的-->>不可访问(可以使用父类提供的方法才能访问该变量)。

>继承的特性
```cpp
class的缺省的继承方式是私有继承
struct的缺省继承方式是公有继承
子类拥有父类的所有属性和行为
子类就是一种特殊的父类
子类对象可以当作父类对象使用
1.子类对象可以给父类对象赋值
2.父类对象不能赋值给子类对象
3.父类对象的指针/引用可以指向子类对象
4.子类的指针/引用不能指向父类对象(强制类型转换)
5.友元关系：不能被继承，因为友元函数不是类的成员；
6.静态成员变量： 可以被继承，且是同一个变量；
```

>同名隐藏
```cpp
在基类和派生类中，具有相同名称的成员（成员函数||成员变量），如果用派生类对象去访问继承体系中的同名成员，只能访问到自己的，基类的成员无法访问；
只能通过加基类作用域的方式去访问相同成名的基类成员；
且优先给派生类中自己的成员变量和成员函数赋值；
```

>子类构造函数和父类构造之间的关系
```cpp
在子类对象构造时，需要调用父类构造函数对其继承得来的成员进行初始化(父类先构造)
在子类对象析构时，需要调用父类析构函数对其继承得来的成员进行清理(子类先析构)
```

>c++创建一个子类对象时会调用父类的构造函数，那么会创建父类对象吗？

```cpp
不会创建另外一个父类对象，只是初始化子类中属于父类的成员。创建一个对象的时候，发生了两件事情，一是分配对象所需的内存，二是调用构造函数进行初始化。子类对象包含从父类对象继承过来的成员，实现上来说，一般也是子类的内存区域中有一部分就是父类的内存区域。调用父类构造函数的时候，这块父类对象的内存区域就被初始化了。为了避免未初始化的问题，语法强制子类调用父类构造函数。
    
构造方法用来初始化类的对象，与父类的其它成员不同，它不能被子类继承（子类可以继承父类所有的成员变量和成员方法，但不继承父类的构造方法）。因此，在创建子类对象时，为了初始化从父类继承来的数据成员，系统需要调用其父类的构造方法。
如果没有显式的构造函数，编译器会给一个默认的构造函数，并且该默认的构造函数仅仅在没有显式地声明构造函数情况下创建。
    1. 如果子类没有定义构造方法，则调用父类的无参数的构造方法。
    2. 如果子类定义了构造方法，不论是无参数还是带参数，在创建子类的对象的时候,首先执行父类无参数的构造方法，然后执行自己的构造方法。
    3. 在创建子类对象时候，如果子类的构造函数没有显示调用父类的构造函数，则会调用父类的默认无参构造函数。
    4. 在创建子类对象时候，如果子类的构造函数没有显示调用父类的构造函数且父类自己提供了无参构造函数，则会调用父类自己的无参构造函数。
    5. 在创建子类对象时候，如果子类的构造函数没有显示调用父类的构造函数且父类只定义了自己的有参构造函数，则会出错（如果父类只有有参数的构造方法，则子类必须显示调用此带参构造方法）。
    6. 如果子类调用父类带参数的构造方法，需要用初始化父类成员对象的方式，比如：
    class animal
    animal(int height, int weight)
    class fish:public animal
    fish():animal(400,300)
```

> 虚继承

```cpp
对于菱形继承
D继承于BC,BC继承于A;
    类D中的成员变量 int a，如果访问a，就会出现二义性问题，到底是B中的a，还是C中的A，并且会造成数据冗余问题。
其中，对于二义性问题，我们加访问限定即可。例如 B::a 或者C::a；但对于数据冗余问题却没有办法解决。
于是C++中就引入了虚拟继承
        
#include<iostream>  
using namespace std;  
class A  //大小为4  
{  
    public:  
    int a;  
};  
class B :virtual public A  //大小为12，变量a,b共8字节，虚基类表指针4  
{  
    public:  
    int b;  
};  

class C :virtual public A //与B一样12 
{  
    public:  
    int c;  
};  

class D :public B, public C //24,变量a,b,c,d共16，B的虚基类指针4，C的虚基类指针  
{  
    public:  
    int d;  
};  

int main()  
{  
    A a;  
    B b;  
    C c;  
    D d;  
    cout << sizeof(a) << endl;  
    cout << sizeof(b) << endl;  
    cout << sizeof(c) << endl;  
    cout << sizeof(d) << endl;  
    return 0;  
}

虚拟继承和普通继承的区别
    如果一个类中有虚函数，那么就会有一个虚表，有一个指针指向这个虚表;
    对于普通继承，继承的虚函数和本有的虚函数共用同一个虚表;
    但对于虚拟继承来说，不管是基类还是派生类都需要有一个指针来维护自己的虚表，并且还要有一个指针指向虚基表，其中存放偏移量;
```