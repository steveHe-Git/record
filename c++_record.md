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

## 4. C++中继承详解

>继承的权限有三种
```cpp
1. 私有继承：基类中的非私有成员都为派生类的私有成员
2. 保护继承：基类中的非私有成员在派生类中的访问属性都降一级(公有变保护，保护变保护)
3. 公有继承：基类中的非私有成员在派生类中的访问属性保持不变
注意：基类中的私有成员，子类能继承，但是子类不能直接访问，需要使用父类提供的方法才能访问该变量。改方法是从父类继承中得到的protected、public方法来访问
注意：派生类从基类公有继承时，派生类的成员函数可以直接访问基类的公有成员，但不能访问基类的私有成员
    为了便于派生类的访问，可以将基类的私有成员中需要提供给派生类访问的成员定义为保护成员
    派生类可以访问protected权限的成员但是派生类的对象不能访问基类的成员

4. 访问权限
public：类内、类的对象；派生类内、派生类对象--->>均可访问。
protected：类内、派生类内--->>可以访问；类的对象、派生类的对象-->>不可访问。
private：只有类内部-->>可以访问；类的对象、派生类、派生类的对象，统统的-->>不可访问(可以使用父类提供的方法才能访问该变量)。
```
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

## 5. c++中的友元函数
```cpp
类的友元函数是定义在类外部，但有权访问类的所有私有（private）成员和保护（protected）成员。尽管友元函数的原型有在类的定义中出现过，但是友元函数并不是成员函数。
友元可以是一个函数，该函数被称为友元函数；友元也可以是一个类，该类被称为友元类，在这种情况下，整个类及其所有成员都是友元。
如果要声明函数为一个类的友元，需要在类定义中该函数原型前使用关键字 friend，如下所示：

//友元类和友元函数的使用
#include <iostream>
using namespace std;
class Box
{
    double width;
public:
    friend void printWidth(Box box);
    friend class BigBox;
    void setWidth(double wid);
};
class BigBox
{
public :
    void Print(int width, Box &box){
        // BigBox是Box的友元类，它可以直接访问Box类的任何成员
        box.setWidth(width);
        cout << "Width of box : " << box.width << endl;
    }
};
// 成员函数定义
void Box::setWidth(double wid){
    width = wid;
}
// 请注意：printWidth() 不是任何类的成员函数
void printWidth(Box box) {
    /* 因为 printWidth() 是 Box 的友元，它可以直接访问该类的任何成员 */
    cout << "Width of box : " << box.width << endl;
}

// 程序的主函数
int main()
{
    Box box;
    BigBox big;
    // 使用成员函数设置宽度
    box.setWidth(10.0);
    // 使用友元函数输出宽度
    printWidth(box);
    // 使用友元类中的方法设置宽度
    big.Print(20, box);
    getchar();
    return 0;
}

注意：
1. 因为友元函数没有this指针，则参数要有三种情况： 
2. 要访问非static成员时，需要对象做参数；
3. 要访问static成员或全局变量时，则不需要对象做参数；
4. 如果做参数的对象是全局对象，则不需要对象做参数.
5. 可以直接调用友元函数，不需要通过对象或指针
```

## 6. c++中的强制转换

> dynamic_cast和static_cast，reinterpret_cast，const_cast异同

```cpp
1. const_cast
const_cast顾名思义，用来将对象的常量属性转除，使常量可以被修改。const_cast<type>(varible)中的type必须是指针，引用，或者指向对象类型成员的指针。比如以下用法是错误的
const int a = 3;
const_cast<int>(a) = 4; //错误的
const_cast<int&>(a) = 4; //正确的

2.dynamic_cast
多态类之间的类型转换用daynamic_cast。
    1. 子类转成父类dynamic_cast和static_cast都没有问题.
    2. 父类转成子类，dynamic_cast要求父类中有虚函数，否则编译不通过。static_cast不作此要求，编译通过。
       在有虚函数的前提下，如果父类指针的确实指向的是子类实例，dynamic_cast转换成功，否则返回NULL；
       static_cast对于转换前的指针是否指向实际子类实例，不作要求，都能转换成功;
    class B
    {
         virtual void f(){};
    };
    class D : public B
    {
         virtual void f(){};
    };
    void main()
    {
         B* pb = new D;   // unclear but ok
         B* pb2 = new B;
         D* pd = dynamic_cast<D*>(pb);   // ok: pb actually points to a D
         D* pd2 = dynamic_cast<D*>(pb2);   // pb2 points to a B not a D, now pd2 is NULL
    }

3.static_cast
无条件转换，静态类型转换；
　  1. 基类和子类之间转换：其中子类指针转换成父类指针是安全的;但父类指针转换成子类指针是不安全的。(基类和子类之间的动态类型转换建议用dynamic_cast)
　　2. 基本数据类型转换。enum, struct, int, char, float等。static_cast不能进行无关类型(如非基类和子类)指针之间的转换			int*a;（double *d = static_cast<double *>(a) //无关类型指针转换，编译错误）。
　　3. 把空指针转换成目标类型的空指针。
　　4. 把任何类型的表达式转换成void类型。
　　5. static_cast不能去掉类型的const、volitale属性(用const_cast)，但是可以加常属性(const int a = static_cast<const int>(a))。
    
4.reinterpret_cast
    允许将任何指针类型转换为其它的指针类型；听起来很强大，但是也很不靠谱。它主要用于将一种数据类型从一种类型转换为另一种类型。它可以将一个指针转换成一个整数，也可以将一个整数转换成一个指针，在实际开发中，先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原来的指针值；特别是开辟了系统全局的内存空间，需要在多个应用程序之间使用时，需要彼此共享，传递这个内存空间的指针时，就可以将指针转换成整数值，得到以后，再将整数值转换成指针，进行对应的操作。
```
## 7. C++ 用operator实现隐式类型转换

```cpp
#include <iostream>
template<typename _T>
class A {
public:
	A(_T a) : data(a) {}
	operator _T () { return data; }
private:
	_T data;
};
 
int main() {
	A<int> obj(2);
	obj = obj + 1; //obj会隐式转换成int类型
	std::cout << obj << std::endl;
	return 0;
}

注意： 运算符重载中(以==为例说明隐式类型转换)
1. 首先看是否是同种支持(==)的普通基本运算符，如果是，直接进行操作；
2. 如果满足上述条件，看是否有重载==运算符；
3. 上述两条不满足，则看是否具有隐式类型转换；

示例：
class Integer{
public:
	Integer(int const& i):m_i(i){}
    operator int&(void){
        return m_i;
    }
    operator int const&(void) const{
        return static_cast<int&>(const_cast<Integer&>(*this))
    }
private:
    int m_i;
}
```

## 8. c++ 常量引用, 常对象，和对象的常成员用法详解
> 常引用
```cpp
1. 指向常量对象时，一定要使用“常量引用”，而不能是一般的引用。
const int ci = 1024;
const int &r1 = ci;         // 正确：引用及其对应的对象都是常量
r1 = 42;                    // 错误：r1是对常量的引用，不能被用作修改它所绑定的对象
int &r2 = ci;               // 错误：试图让一个非常量引用指向一个常量对象

2. “常量引用”可以指向一个非常量对象，但不允许用过该引用修改非常量对象的值。
int i = 42;
int &r1 = i;                // 普通引用指向非常量对象 i
const int &r2 = i;          // 常量引用也绑定非常量对象 i
r1 = 0;                     // 正确，r1并非常量引用
r2 = 0;                     // 错误：r2是一个常量引用
r2 绑定非常量整数 i 是合法的行为。然而不允许通过 r2 修改 i 的值。尽管如此，i 的值仍然允许通过其他途径修改，既可以直接给 i 赋值，也可以通过像 r1 一样绑定到 i 的其他引用来修改
    
3. 引用的类型必须和所引用的类型严格匹配，且不能与字面值或者某个表达式的计算结果绑定在一起，但是 “常量引用” 是例外（只要被引用的类型能够转换为常量引用的类型）
//类型严格匹配  
const int i = 42;
const int &r1 = i;          // i和r1指向的是同一块地址
    
//类型不严格匹配    
int i = 42;
const int &r1 = i;          // 正确：指向非常量对象 
const int &r2 = 42;         // 正确：r2 是一个常量引用
const int &r3 = r1 * 2;     // 正确：r3 是一个常量引用
int &r4 = r1 * 2;           // 错误：r4 是一个普通的非常量引用

double dval = 3.14;
const int &r1 = dval;
此处 const int &r1 = dval 编译器实际上相当于执行了下列语句：引用和原 dval 已经不是同一个地址了:
const int temp = dval;      // 生成一个临时的整型常量
const int &r1 = temp;       // 让 r1 绑定这个临时量
也就是说，不允许一个普通引用与字面值或者某个表达式的计算结果，或类型不匹配的对象绑定在一起，其实就是不允许一个普通引用指向一个临时变量，只允许将“常量引用”指向临时对象。
    
4. 在函数参数中，使用常量引用非常重要。因为函数有可能接受临时对象，而且同时需要禁止对所引用对象的一切修改
int test() {
	return 1;
}

void fun(int &x) {
    cout << x << endl;
}
 
int main()
{
	int m = 1;
	fun(m);         // ok
	fun(1);         // error
    fun(test());    // error
 
	return 0;
}
按下面修改后，fun()函数无论是接受字面值常量作为参数，还是将函数的返回值作为参数均可
 int test() {
	return 1;
}
 
void fun(const int &x) {
    cout << x << endl;
}
 
int main()
{
    fun(1);         // ok
	fun(test());    // ok
 
	return 1;
}
```

> 常对象和对象的常成员

```cpp
1. 所谓常对象是指其数据成员在他的生存周期内不会被改变，定义常对象时必须对其进行初始化，并且不能改变数据成员的数值
2. 常对象不能调用普通的成员函数，没有对外的接口，需要专门为常对象定义常成员函数
    a. 常成员函数在声明和实现时都要带const关键字
    b. 常成员函数不能修改对象的数据成员，也不能访问类中没有const声明的非常成员函数
    c. 常对象只能调用它的常成员函数，不能调用其他的普通成员函数
   	d. const关键字可以被用于参与对重载函数的区分
   	
3. 类的常数据成员：在任何函数中都不能对常数据成员赋值，构造函数对常数据成员初始化，只能通过初始化列表
```

> 创建一个二维数组

```cpp
#include <iostream>
using namespace std;
void main()
{
//用new创建一个二维数组,有两种方法,是等价的
//一:
int (*p)[10] = new int[5][10];
//二:
int **p = new int* [5];
for(int i=0;i <5;i++)
p[i] = new int[10];
}
```

## 9 三种智能指针(shared_ptr、weak_ptr 、 unique_ptr)

> shared_ptr

```cpp
 - shared_ptr使用了引用计数，每一个shared_ptr的拷贝都指向相同的内存，每次拷贝都会触发引用计数+1，每次生命周期结束析构的时候引用计数-1，在最后一个shared_ptr析构的时候，内存才会释放。
 struct ClassWrapper {

    ClassWrapper() {
        cout << "construct" << endl;
        data = new int[10];
    }

    ~ClassWrapper() {
        cout << "deconstruct" << endl;
        if (data != nullptr) {
            delete[] data;
        }
    }

    void Print() {
        cout << "print" << endl;
    }

    int* data;
};

void Func(std::shared_ptr<ClassWrapper> ptr) {
    ptr->Print();
}

int main() {
    auto smart_ptr = std::make_shared<ClassWrapper>();
    auto ptr2 = smart_ptr; // 引用计数+1
    ptr2->Print();
    Func(smart_ptr); // 引用计数+1
    smart_ptr->Print();
    ClassWrapper *p = smart_ptr.get(); // 可以通过get获取裸指针
    p->Print();
    return 0;
}

智能指针还可以自定义删除器，在引用计数为0的时候自动调用删除器来释放对象的内存，代码如下：
std::shared_ptr<int> ptr(new int, [](int *p){ delete p; });

注意：
    1. 不要用一个裸指针初始化多个shared_ptr，会出现double_free导致程序崩溃；
    2. 通过shared_from_this()返回this指针，不要把this指针作为shared_ptr返回出来，因为this指针本质就是裸指针，通过this返回可能 会导致重复析构，不能把this指针交给智能指针管理。
   class A {
      shared_ptr<A> GetSelf() {
        return shared_from_this();
        // return shared_ptr<A>(this); 错误，会导致double free
    	}  
	};
	3. 尽量使用make_shared，少用new。
    4. 不要delete get()返回来的裸指针。
    5. 是new出来的空间要自定义删除器。
	6. 要避免循环引用，循环引用导致内存永远不会被释放，造成内存泄漏；
    using namespace std;
    struct A;
    struct B;

    struct A {
        std::shared_ptr<B> bptr;
        ~A() {
            cout << "A delete" << endl;
        }
    };

    struct B {
        std::shared_ptr<A> aptr;
        ~B() {
            cout << "B delete" << endl;
        }
    };

    int main() {
        auto aaptr = std::make_shared<A>();
        auto bbptr = std::make_shared<B>();
        aaptr->bptr = bbptr;
        bbptr->aptr = aaptr;
        return 0;
    }
    上面代码，产生了循环引用，导致aptr和bptr的引用计数为2，离开作用域后aptr和bptr的引用计数-1，但是永远不会为0，导致指针永远不会析构，产生了内存泄漏，如何解决这种问题呢，答案是使用weak_ptr
```

> weak_ptr

```cpp
weak_ptr是用来监视shared_ptr的生命周期，它不管理shared_ptr内部的指针，它的拷贝的析构都不会影响引用计数，纯粹是作为一个旁观者监视shared_ptr中管理的资源是否存在，可以用来返回this指针和解决循环引用问题。
    - 作用1：返回this指针，上面介绍的shared_from_this()其实就是通过weak_ptr返回的this指针，这里参考我之前写的源码分析shared_ptr实现的文章，最后附上链接
    - 作用2：解决循环引用问题。
    struct A;
    struct B;

    struct A {
        std::shared_ptr<B> bptr;
        ~A() {
            cout << "A delete" << endl;
        }
        void Print() {
            cout << "A" << endl;
        }
    };

    struct B {
        std::weak_ptr<A> aptr; // 这里改成weak_ptr
        ~B() {
            cout << "B delete" << endl;
        }
        void PrintA() {
            if (!aptr.expired()) { // 监视shared_ptr的生命周期
                auto ptr = aptr.lock();
                ptr->Print();
            }
        }
    };

    int main() {
        auto aaptr = std::make_shared<A>();
        auto bbptr = std::make_shared<B>();
        aaptr->bptr = bbptr;
        bbptr->aptr = aaptr;
        bbptr->PrintA();
        return 0;
    }
    输出：
    A
    A delete
    B delete
```

> unique_ptr

```cpp
std::unique_ptr是一个独占型的智能指针，它不允许其它智能指针共享其内部指针，也不允许unique_ptr的拷贝和赋值。使用方法和shared_ptr类似，区别是不可以拷贝：
    using namespace std;

    struct A {
        ~A() {
            cout << "A delete" << endl;
        }
        void Print() {
            cout << "A" << endl;
        }
    };


    int main() {
        auto ptr = std::unique_ptr<A>(new A);
        auto tptr = std::make_unique<A>(); // error, c++11还不行，需要c++14
        std::unique_ptr<A> tem = ptr; // error, unique_ptr不允许移动
        ptr->Print();
        return 0;
    }
unique_ptr也可以像shared_ptr一样自定义删除器，使用方法和shared_ptr相同。
```

## 10.  c++中的右值引用、移动语义和完美转发

> 左值、右值

```cpp
c++中引入了右值引用和移动语义，可以避免无谓的复制，提高程序性能
`C++`中所有的值都必然属于左值、右值二者之一。左值是指表达式结束后依然存在的*持久化对象*，右值是指表达式结束时就不再存在的*临时对象*。所有的具名变量或者对象都是左值，而右值不具名。很难得到左值和右值的真正定义，但是有一个可以区分左值和右值的便捷方法：**看能不能对表达式取地址，如果能，则为左值，否则为右值**

右值分为将亡值和纯右值，纯右值就是c++98标准中右值的概念，如非引用返回的函数返回的临时变量值；一些运算表达式，如1+2产生的临时变量；不跟对象关联的字面量值，如2，'c'，true，"hello"；这些值都不能够被取地址；
而将亡值则是c++11新增的和右值引用相关的表达式，这样的表达式通常时将要移动的对象、T&&函数返回值、std::move()函数的返回值等；
不懂将亡值和纯右值的区别其实没关系，统一看作右值即可，不影响使用。
    int i=0;// i是左值， 0是右值
    class A {
      public:
        int a;
    };
    A getTemp()
    {
        return A();
    }
    A a = getTemp();   // a是左值  getTemp()的返回值是右值（临时变量）
```

>左值引用、右值引用

```cpp
c++98中的引用很常见了，就是给变量取了个别名，在c++11中，因为增加了右值引用(rvalue reference)的概念，所以c++98中的引用都称为了左值引用(lvalue reference)。
    int a = 10; 
    int& refA = a; // refA是a的别名， 修改refA就是修改a, a是左值，左移是左值引用
    int& b = 1; //编译错误! 1是右值，不能够使用左值引用

c++11中的右值引用使用的符号是&&，如
    int&& a = 1; //实质上就是将不具名(匿名)变量取了个别名
    int b = 1;
    int && c = b; //编译错误！ 不能将一个左值复制给一个右值引用
    class A {
      public:
        int a;
    };
    A getTemp()
    {
        return A();
    }
    A && a = getTemp();   //getTemp()的返回值是右值（临时变量）

getTemp()返回的右值本来在表达式语句结束后，其生命也就该终结了（因为是临时变量），而通过右值引用，该右值又重获新生，其生命期将与右值引用类型变量a的生命期一样，只要a还活着，该右值临时变量将会一直存活下去。实际上就是给那个临时变量取了个名字。
注意：
    这里a的类型是右值引用类型(int &&)，但是如果从左值和右值的角度区分它，它实际上是个左值。因为可以对它取地址，而且它还有名字，是一个已经命名的右值。
    所以，左值引用只能绑定左值，右值引用只能绑定右值，如果绑定的不对，编译就会失败。但是，常量左值引用却是个奇葩，它可以算是一个“万能”的引用类型，它可以绑定非常量左值、常量左值、右值，而且在绑定右值的时候，常量左值引用还可以像右值引用一样将右值的生命期延长，缺点是，只能读不能改。
    const int & a = 1; //常量左值引用绑定 右值， 不会报错

    class A {
      public:
        int a;
    };
    A getTemp()
    {
        return A();
    }
    const A & a = getTemp();   //不会报错 而 A& a 会报错

事实上，很多情况下我们用来常量左值引用的这个功能却没有意识到，如下面的例子：
    #include <iostream>
    using namespace std;

    class Copyable {
    public:
        Copyable(){}
        Copyable(const Copyable &o) {
            cout << "Copied" << endl;
        }
    };
    Copyable ReturnRvalue() {
        return Copyable(); //返回一个临时对象
    }
    void AcceptVal(Copyable a) {

    }
    void AcceptRef(const Copyable& a) {

    }

    int main() {
        cout << "pass by value: " << endl;
        AcceptVal(ReturnRvalue()); // 应该调用两次拷贝构造函数
        cout << "pass by reference: " << endl;
        AcceptRef(ReturnRvalue()); //应该只调用一次拷贝构造函数
    }

当敲完上面的例子并运行后，发现结果和我想象的完全不一样！期望中AcceptVal(ReturnRvalue())需要调用两次拷贝构造函数，一次在ReturnRvalue()函数中，构造好了Copyable对象，返回的时候会调用拷贝构造函数生成一个临时对象，在调用AcceptVal()时，又会将这个对象拷贝给函数的局部变量a，一共调用了两次拷贝构造函数。而AcceptRef()的不同在于形参是常量左值引用，它能够接收一个右值，而且不需要拷贝。
注意：
    而实际的结果是，不管哪种方式，一次拷贝构造函数都没有调用！
    这是由于编译器默认开启了返回值优化(RVO/NRVO, RVO, Return Value Optimization 返回值优化，或者NRVO， Named Return Value Optimization)。编译器很聪明，发现在ReturnRvalue内部生成了一个对象，返回之后还需要生成一个临时对象调用拷贝构造函数，很麻烦，所以直接优化成了1个对象对象，避免拷贝，而这个临时变量又被赋值给了函数的形参，还是没必要，所以最后这三个变量都用一个变量替代了，不需要调用拷贝构造函数。
	虽然各大厂家的编译器都已经都有了这个优化，但是这并不是c++标准规定的，而且不是所有的返回值都能够被优化，而这篇文章的主要讲的右值引用，移动语义可以解决编译器无法解决的问题。
    为了更好的观察结果，可以在编译的时候加上-fno-elide-constructors选项(关闭返回值优化)。
    // g++ test.cpp -o test -fno-elide-constructors
    pass by value: 
    Copied
    Copied //可以看到确实调用了两次拷贝构造函数
    pass by reference: 
    Copied	
        
    上面这个例子本意是想说明常量左值引用能够绑定一个右值，可以减少一次拷贝（使用非常量的左值引用会编译失败），但是顺便讲到了编译器的返回值优化。。编译器还是干了很多事情的，很有用，但不能过于依赖，因为你也不确定它什么时候优化了什么时候没优化。
	总结一下，其中T是一个具体类型：
    1. 左值引用， 使用 T&, 只能绑定左值
    2. 右值引用， 使用 T&&， 只能绑定右值
    3. 常量左值， 使用 const T&, 既可以绑定左值又可以绑定右值
    4. 已命名的右值引用，编译器会认为是个左值
    5. 编译器有返回值优化，但不要过于依赖
```

>移动构造和移动赋值

```cpp
回顾一下如何用c++实现一个字符串类MyString，MyString内部管理一个C语言的char *数组，这个时候一般都需要实现拷贝构造函数和拷贝赋值函数，因为默认的拷贝是浅拷贝，而指针这种资源不能共享，不然一个析构了，另一个也就完蛋了。
    #include <iostream>
    #include <cstring>
    #include <vector>
    using namespace std;

    class MyString
    {
    public:
        static size_t CCtor; //统计调用拷贝构造函数的次数
    public:
        // 构造函数
       MyString(const char* cstr=0){
           if (cstr) {
              m_data = new char[strlen(cstr)+1];
              strcpy(m_data, cstr);
           }
           else {
              m_data = new char[1];
              *m_data = '\0';
           }
       }

       // 拷贝构造函数
       MyString(const MyString& str) {
           CCtor ++;
           m_data = new char[ strlen(str.m_data) + 1 ];
           strcpy(m_data, str.m_data);
       }
       // 拷贝赋值函数 =号重载
       MyString& operator=(const MyString& str){
           if (this == &str) // 避免自我赋值!!
              return *this;

           delete[] m_data;
           m_data = new char[ strlen(str.m_data) + 1 ];
           strcpy(m_data, str.m_data);
           return *this;
       }

       ~MyString() {
           delete[] m_data;
       }

       char* get_c_str() const { return m_data; }
    private:
       char* m_data;
    };
    size_t MyString::CCtor = 0;

    int main()
    {
        vector<MyString> vecStr;
        vecStr.reserve(1000); //先分配好1000个空间，不这么做，调用的次数可能远大于1000
        for(int i=0;i<1000;i++){
            vecStr.push_back(MyString("hello"));
        }
        cout << MyString::CCtor << endl;
    }
代码看起来挺不错，却发现执行了1000次拷贝构造函数，如果MyString("hello")构造出来的字符串本来就很长，构造一遍就很耗时了，最后却还要拷贝一遍，而MyString("hello")只是临时对象，拷贝完就没什么用了，这就造成了没有意义的资源申请和释放操作，如果能够直接使用临时对象已经申请的资源，既能节省资源，又能节省资源申请和释放的时间。而C++11新增加的移动语义就能够做到这一点。
    要实现移动语义就必须增加两个函数：移动构造函数和移动赋值构造函数。
    #include <iostream>
    #include <cstring>
    #include <vector>
    using namespace std;

    class MyString
    {
    public:
        static size_t CCtor; //统计调用拷贝构造函数的次数
        static size_t MCtor; //统计调用移动构造函数的次数
        static size_t CAsgn; //统计调用拷贝赋值函数的次数
        static size_t MAsgn; //统计调用移动赋值函数的次数

    public:
        // 构造函数
       MyString(const char* cstr=0){
           if (cstr) {
              m_data = new char[strlen(cstr)+1];
              strcpy(m_data, cstr);
           }
           else {
              m_data = new char[1];
              *m_data = '\0';
           }
       }

       // 拷贝构造函数
       MyString(const MyString& str) {
           CCtor ++;
           m_data = new char[ strlen(str.m_data) + 1 ];
           strcpy(m_data, str.m_data);
       }
       // 移动构造函数
       MyString(MyString&& str) noexcept
           :m_data(str.m_data) {
           MCtor ++;
           str.m_data = nullptr; //不再指向之前的资源了
       }

       // 拷贝赋值函数 =号重载
       MyString& operator=(const MyString& str){
           CAsgn ++;
           if (this == &str) // 避免自我赋值!!
              return *this;

           delete[] m_data;
           m_data = new char[ strlen(str.m_data) + 1 ];
           strcpy(m_data, str.m_data);
           return *this;
       }

       // 移动赋值函数 =号重载
       MyString& operator=(MyString&& str) noexcept{
           MAsgn ++;
           if (this == &str) // 避免自我赋值!!
              return *this;

           delete[] m_data;
           m_data = str.m_data;
           str.m_data = nullptr; //不再指向之前的资源了
           return *this;
       }

       ~MyString() {
           delete[] m_data;
       }

       char* get_c_str() const { return m_data; }
    private:
       char* m_data;
    };
    size_t MyString::CCtor = 0;
    size_t MyString::MCtor = 0;
    size_t MyString::CAsgn = 0;
    size_t MyString::MAsgn = 0;
    int main()
    {
        vector<MyString> vecStr;
        vecStr.reserve(1000); //先分配好1000个空间
        for(int i=0;i<1000;i++){
            vecStr.push_back(MyString("hello"));
        }
        cout << "CCtor = " << MyString::CCtor << endl;
        cout << "MCtor = " << MyString::MCtor << endl;
        cout << "CAsgn = " << MyString::CAsgn << endl;
        cout << "MAsgn = " << MyString::MAsgn << endl;
    }

    /* 结果
    CCtor = 0
    MCtor = 1000
    CAsgn = 0
    MAsgn = 0
    */

可以看到，移动构造函数与拷贝构造函数的区别是，拷贝构造的参数是const MyString& str，是常量左值引用，而移动构造的参数是MyString&& str，是右值引用，而MyString("hello")是个临时对象，是个右值，优先进入移动构造函数而不是拷贝构造函数。而移动构造函数与拷贝构造不同，它并不是重新分配一块新的空间，将要拷贝的对象复制过来，而是"偷"了过来，将自己的指针指向别人的资源，然后将别人的指针修改为nullptr，这一步很重要，如果不将别人的指针修改为空，那么临时对象析构的时候就会释放掉这个资源，"偷"也白偷了。下面这张图可以解释copy和move的区别。
不用奇怪为什么可以抢别人的资源，临时对象的资源不好好利用也是浪费，因为生命周期本来就是很短，在你执行完这个表达式之后，它就毁灭了，充分利用资源，才能很高效。

对于一个左值，肯定是调用拷贝构造函数了，但是有些左值是局部变量，生命周期也很短，能不能也移动而不是拷贝呢？C++11为了解决这个问题，"提供了std::move()方法来将左值转换为右值，从而方便应用移动语义"。我觉得它其实就是告诉编译器，虽然我是一个左值，但是不要对我用拷贝构造函数，而是用移动构造函数吧。。。
int main()
{
    vector<MyString> vecStr;
    vecStr.reserve(1000); //先分配好1000个空间
    for(int i=0;i<1000;i++){
        MyString tmp("hello");
        vecStr.push_back(tmp); //调用的是拷贝构造函数
    }
    cout << "CCtor = " << MyString::CCtor << endl;
    cout << "MCtor = " << MyString::MCtor << endl;
    cout << "CAsgn = " << MyString::CAsgn << endl;
    cout << "MAsgn = " << MyString::MAsgn << endl;

    cout << endl;
    MyString::CCtor = 0;
    MyString::MCtor = 0;
    MyString::CAsgn = 0;
    MyString::MAsgn = 0;
    vector<MyString> vecStr2;
    vecStr2.reserve(1000); //先分配好1000个空间
    for(int i=0;i<1000;i++){
        MyString tmp("hello");
        vecStr2.push_back(std::move(tmp)); //调用的是移动构造函数
    }
    cout << "CCtor = " << MyString::CCtor << endl;
    cout << "MCtor = " << MyString::MCtor << endl;
    cout << "CAsgn = " << MyString::CAsgn << endl;
    cout << "MAsgn = " << MyString::MAsgn << endl;
}

/* 运行结果
CCtor = 1000
MCtor = 0
CAsgn = 0
MAsgn = 0

CCtor = 0
MCtor = 1000
CAsgn = 0
MAsgn = 0
*/

下面再举几个例子：
    MyString str1("hello"); //调用构造函数
    MyString str2("world"); //调用构造函数
    MyString str3(str1); //调用拷贝构造函数
    MyString str4(std::move(str1)); // 调用移动构造函数、
    //    cout << str1.get_c_str() << endl; // 此时str1的内部指针已经失效了！不要使用
    //注意：虽然str1中的m_dat已经称为了空，但是str1这个对象还活着，知道出了它的作用域才会析构！而不是move完了立刻析构
    MyString str5;
    str5 = str2; //调用拷贝赋值函数
    MyString str6;
    str6 = std::move(str2); // str2的内容也失效了，不要再使用
				// str6 = std::move(str2) 调用移动构造函数，m_date会被置成空;但是str2这个对象还存在，所以不要用str2中的m_data资源


需要注意一下几点：
    str6 = std::move(str2)，虽然将str2的资源给了str6，但是str2并没有立刻析构，只有在str2离开了自己的作用域的时候才会析构，所以，如果继续使用str2的m_data变量，可能会发生意想不到的错误。
    如果我们没有提供移动构造函数，只提供了拷贝构造函数，std::move()会失效但是不会发生错误，因为编译器找不到移动构造函数就去寻找拷贝构造函数，也这是拷贝构造函数的参数是const T&常量左值引用的原因！
    c++11中的所有容器都实现了move语义，move只是转移了资源的控制权，本质上是将左值强制转化为右值使用，以用于移动拷贝或赋值，避免对含有资源的对象发生无谓的拷贝。move对于拥有如内存、文件句柄等资源的成员的对象有效，如果是一些基本类型，如int和char[10]数组等，如果使用move，仍会发生拷贝（因为没有对应的移动构造函数），所以说move对含有资源的对象说更有意义。
```

>universal references(通用引用)

```cpp
当右值引用和模板结合的时候，就复杂了。T&&并不一定表示右值引用，它可能是个左值引用又可能是个右值引用。例如：
    template<typename T>
    void f( T&& param){

    }
    f(10);  //10是右值
    int x = 10; //
    f(x); //x是左值
    如果上面的函数模板表示的是右值引用的话，肯定是不能传递左值的，但是事实却是可以。这里的&&是一个未定义的引用类型，称为universal references，它必须被初始化，它是左值引用还是右值引用却决于它的初始化，如果它被一个左值初始化，它就是一个左值引用；如果被一个右值初始化，它就是一个右值引用。
        
注意：
    只有当发生自动类型推断时（如函数模板的类型自动推导，或auto关键字），&&才是一个universal references。
    template<typename T>
    void f( T&& param); //这里T的类型需要推导，所以&&是一个 universal references

    template<typename T>
    class Test {
      Test(Test&& rhs); //Test是一个特定的类型，不需要类型推导，所以&&表示右值引用  
    };

    void f(Test&& param); //右值引用

    //复杂一点
    template<typename T>
    void f(std::vector<T>&& param); //在调用这个函数之前，这个vector<T>中的推断类型
    //已经确定了，所以调用f函数的时候没有类型推断了，所以是 右值引用

    template<typename T>
    void f(const T&& param); //右值引用
    // universal references仅仅发生在 T&& 下面，任何一点附加条件都会使之失效

  	所以最终还是要看T被推导成什么类型，如果T被推导成了string，那么T&&就是string&&，是个右值引用，如果T被推导为string&，就会发生类似string& &&的情况，对于这种情况，c++11增加了引用折叠的规则，总结如下：
    1. 所有的右值引用叠加到右值引用上仍然使一个右值引用。
    2. 所有的其他引用类型之间的叠加都将变成左值引用。
如上面的T& &&其实就被折叠成了个string&，是一个左值引用。
    #include <iostream>
    #include <type_traits>
    #include <string>
    using namespace std;

    template<typename T>
    void f(T&& param){
        if (std::is_same<string, T>::value)
            std::cout << "string" << std::endl;
        else if (std::is_same<string&, T>::value)
            std::cout << "string&" << std::endl;
        else if (std::is_same<string&&, T>::value)
            std::cout << "string&&" << std::endl;
        else if (std::is_same<int, T>::value)
            std::cout << "int" << std::endl;
        else if (std::is_same<int&, T>::value)
            std::cout << "int&" << std::endl;
        else if (std::is_same<int&&, T>::value)
            std::cout << "int&&" << std::endl;
        else
            std::cout << "unkown" << std::endl;
    }

    int main()
    {
        int x = 1;
        f(1); // 参数是右值 T推导成了int, 所以是int&& param, 右值引用
        f(x); // 参数是左值 T推导成了int&, 所以是int&&& param, 折叠成 int&,左值引用
        int && a = 2;
        f(a); //虽然a是右值引用，但它还是一个左值， T推导成了int&
        string str = "hello";
        f(str); //参数是左值 T推导成了string&
        f(string("hello")); //参数是右值， T推导成了string
        f(std::move(str));//参数是右值， T推导成了string
    }
所以，归纳一下， 传递左值进去，就是左值引用，传递右值进去，就是右值引用。如它的名字，这种类型确实很"通用"，下面要讲的完美转发，就利用了这个特性。
```

> 完美转发

```cpp
所谓转发，就是通过一个函数将参数继续转交给另一个函数进行处理，原参数可能是右值，可能是左值，如果还能继续保持参数的原有特征，那么它就是完美的
    void process(int& i){
        cout << "process(int&):" << i << endl;
    }
    void process(int&& i){
        cout << "process(int&&):" << i << endl;
    }

    void myforward(int&& i){
        cout << "myforward(int&&):" << i << endl;
        process(i);
    }

    int main()
    {
        int a = 0;
        process(a); //a被视为左值 process(int&):0
        process(1); //1被视为右值 process(int&&):1
        process(move(a)); //强制将a由左值改为右值 process(int&&):0
        myforward(2);  //右值经过forward函数转交给process函数，却称为了一个左值，
        //原因是该右值有了名字  所以是 process(int&):2
        myforward(move(a));  // 同上，在转发的时候右值变成了左值  process(int&):0
        // forward(a) // 错误用法，右值引用不接受左值
    }
上面修改过后还是不完美转发，myforward()函数能够将右值转发过去，但是并不能够转发左值，解决办法就是借助universal references通用引用类型和std::forward()模板函数共同实现完美转发。例子如下：
    #include <iostream>
    #include <cstring>
    #include <vector>
    using namespace std;

    void RunCode(int &&m) {
        cout << "rvalue ref" << endl;
    }
    void RunCode(int &m) {
        cout << "lvalue ref" << endl;
    }
    void RunCode(const int &&m) {
        cout << "const rvalue ref" << endl;
    }
    void RunCode(const int &m) {
        cout << "const lvalue ref" << endl;
    }

    // 这里利用了universal references，如果写T&,就不支持传入右值，而写T&&，既能支持左值，又能支持右值
    template<typename T>
    void perfectForward(T && t) {
        RunCode(forward<T> (t));
    }

    template<typename T>
    void notPerfectForward(T && t) {
        RunCode(t);
    }

    int main()
    {
        int a = 0;
        int b = 0;
        const int c = 0;
        const int d = 0;

        notPerfectForward(a); // lvalue ref
        notPerfectForward(move(b)); // lvalue ref
        notPerfectForward(c); // const lvalue ref
        notPerfectForward(move(d)); // const lvalue ref

        cout << endl;
        perfectForward(a); // lvalue ref
        perfectForward(move(b)); // rvalue ref
        perfectForward(c); // const lvalue ref
        perfectForward(move(d)); // const rvalue ref
    }
上面的代码测试结果表明，在universal references和std::forward的合作下，能够完美的转发这4种类型。
```

>emplace_back减少内存拷贝和移动

```cpp
我们之前使用vector一般都喜欢用push_back()，由上文可知容易发生无谓的拷贝，解决办法是为自己的类增加移动拷贝和赋值函数，但其实还有更简单的办法！就是使用emplace_back()替换push_back()，如下面的例子：
    #include <iostream>
    #include <cstring>
    #include <vector>
    using namespace std;

    class A {
    public:
        A(int i){
    //        cout << "A()" << endl;
            str = to_string(i);
        }
        ~A(){}
        A(const A& other): str(other.str){
            cout << "A&" << endl;
        }

    public:
        string str;
    };

    int main()
    {
        vector<A> vec;
        vec.reserve(10);
        for(int i=0;i<10;i++){
            vec.push_back(A(i)); //调用了10次拷贝构造函数
    //        vec.emplace_back(i);  //一次拷贝构造函数都没有调用过
        }
        for(int i=0;i<10;i++)
            cout << vec[i].str << endl;
    }

可以看到效果是明显的，虽然没有测试时间，但是确实可以减少拷贝。emplace_back()可以直接通过构造函数的参数构造对象，但前提是要有对应的构造函数。

对于map和set，可以使用emplace()。基本上emplace_back()对应push_bakc(), emplce()对应insert()。

移动语义对swap()函数的影响也很大，之前实现swap可能需要三次内存拷贝，而有了移动语义后，就可以实现高性能的交换函数了。
	template <typename T>
    void swap(T& a, T& b)
    {
        T tmp(std::move(a));
        a = std::move(b);
        b = std::move(tmp);
    }
如果T是可移动的，那么整个操作会很高效，如果不可移动，那么就和普通的交换函数是一样的，不会发生什么错误，很安全。
```
>浅谈std::forward
```cpp
td::forward通常是用于完美转发的，它会将输入的参数原封不动地传递到下一个函数中，这个“原封不动”指的是，如果输入的参数是左值，那么传递给下一个函数的参数的也是左值；如果输入的参数是右值，那么传递给下一个函数的参数的也是右值。一个经典的完美转发的场景是：

    template <class... Args>
    void forward(Args&&... args) {
        f(std::forward<Args>(args)...);
    }
需要注意的有2点：1、输入参数的类型是Args&&... ， &&的作用是引用折叠，其规则是：
    && && -> &&
    & && -> &
    & & -> &
    && & -> &
    由于我们需要保留输入参数的右值属性，因此Args后面需要跟上&&；2、std::forward的模板参数必须是<Args>，而不能是<Args...>，这是由于我们不能对Args进行解包之后传递给std::forward，而解包的过程必须在调用std::forward之后.
    那么std::forward为什么可以达到这一一个作用呢，我们看一下其实现代码：

    template<class T>
    constexpr T&& forward(std::remove_reference_t<T>& arg) noexcept{
        // forward an lvalue as either an lvalue or an rvalue
        return (static_cast<T&&>(arg));
    }

    template<class T>
    constexpr T&& forward(std::remove_reference_t<T>&& arg) noexcept{
        // forward an rvalue as an rvalue
        return (static_cast<T&&>(arg));
    }

    std::remove_reference_t是一个模板类的类型别名，用于去掉T的引用属性（但不会去掉const属性，const属性可以用std::remove_const_t来去掉）；如果forward接受的参数为左值的话，它将其转化成右值返回，这样既可以传给左值，又可以传给右值；如果传递的参数是个右值的话，它将其保留右值属性返回，这样只可以返回给右值。
```

> 总结

```cpp
1. 有两种值类型，左值和右值。
2. 有三种引用类型，左值引用、右值引用和通用引用。左值引用只能绑定左值，右值引用只能绑定右值，通用引用由初始化时绑定的值的类型确定。
3. 左值和右值是独立于他们的类型的，右值引用可能是左值可能是右值，如果这个右值引用已经被命名了，他就是左值。
4. 引用折叠规则：所有的右值引用叠加到右值引用上仍然是一个右值引用，其他引用折叠都为左值引用。当T&&为模板参数时，输入左值，它将变成左值引用，输入右值则变成具名的右值应用。
5. 移动语义可以减少无谓的内存拷贝，要想实现移动语义，需要实现移动构造函数和移动赋值函数。
6. std::move()将一个左值转换成一个右值，强制使用移动拷贝和赋值函数，这个函数本身并没有对这个左值什么特殊操作。
7. std::forward()和universal references通用引用共同实现完美转发。
8. 用empalce_back()替换push_back()增加性能。
```

## 11. 可变模板参数
```cpp
C++可变参数模板，是指能够使用任意数量参数的函数或类模板，能够很有效的提升灵活性。
1. 可变类模板
对于可变类模板，基本示例如下：
template<typename... Arguments>
class classname;
由上式可知，其特殊性在于 ... 的使用，可变参数模板，通过使用 ... 来帮助定义，其中，... 左侧为参数包（parameter pack ），右侧将参数包展开成多个单独的参数。

对于两边的空白可以任意分布，例如：
template<typename ...Arguments>
class classname;
template<typename ... Arguments>
class classname;

上面的类 classname 可以接收任意数量的参数来进行实例化，例如：
classname<> c1();
classname<float, int> c2();
classname<float, std::string, std::vector<int>> c3();

当然，还可以指定必须填充固定数量的参数，例如：
template<typename first, typename... Arguments>
class classname2;
// classname2<> c4(); 这是错误的用法！参数必须大于等于 1
classname2<float> c4();

2. 可变参数的函数模板
可变参数的函数模板基本示例如下：
template<typename... Params>
void functionname(Params... params);
 	
当然，还可以使用引用类型：
template<typename... Params>
void functionname(Params *... params);

template<typename... Params>
void functionname(Params &... params);

template<typename... Params>
void functionname(Params &&... params);

还可以使用不变类型 const 来修饰：
template<typename... Params>
void functionname(const Params &... params);

除此之外，还可以使用和可变参数的类模板用法一样，指定固定数量的参数：
template<typename First, typename... Params>
void functionname(const First &first, const Params... params)

3. 测量可变参数数量
    使用 sizeof 来完成，当然，该方法和之前测量字节大小的 sizeof 不是同一个用法，这里的 sizeof 是用来测量提供了多少个参数，示例如下：
template<typename... Params>
void tfunc(Params... params) {
    std::cout << sizeof...(params) << std::endl;
}

int main() {
    tfunc();        // >: 0
    tfunc(1);       // >: 1
    tfunc(1, 2);    // >: 2
    tfunc(1, 2, "Hello~YouLi~");    // >: 3
}

4.示例程序
下面程序演示了如何使用 可变参数模板 来对函数进行重写定义并使用。
    #include <iostream>
    #include <string>
    #include <vector>
    using namespace std;
    void print() {
        cout << endl;
    }

    template<typename T>
    void print(const T &t) {
        cout << t << endl;
    }

    template<typename First, typename... Rest>
    void print(const First &first, const Rest &...rest) {
        cout << first << ", ";
        print(rest...); // 将会根据语法来递归调用
    }

    int main() {
        print();    // >:
        print(1); // >: 1
        print(10, 20); // >: 10, 20
        print(10, 20, "Alice", 3.14, "Bob"); // >: 10, 20, Alice, 3.14, Bob
    }

```
## 12 . auto 和 register, decltype关键字

> register
```cpp
在早期c语言编译器不会对代码进行优化，因此使用register关键字修饰变量是很好的补充，大大提高的速度。
register关键字请求让编译器将变量a直接放入寄存器里面，以提高读取速度，在C语言中register关键字修饰的变量不可以被取地址，但是c++中进行了优化。
    c++中依然支持register关键字，但是c++编译器也有自己的优化方式，即某些变量不用register关键字进行修饰，编译器也会将多次连续使用的变量优化放入寄存器中，例如入for循环的循环变量i。
    c++中也可以对register修饰的变量取地址，不过c++编译器发现程序中需要取register关键字修饰的变量的地址时，register关键字的声明将变得无效。
```

> auto

```cpp
auto的自动类型推断发生在编译期，所以使用auto并不会造成程序运行时效率的降低。例如上面的代码，编译的时候，就会把a变量转换为int类型
    auto和其他变量类型有明显的区别：
1.auto声明的变量必须要初始化，否则编译器不能判断变量的类型。
2.auto不能被声明为返回值，auto不能作为形参，auto不能被修饰为模板参数
    
- auto 变量必须在定义时初始化，这类似于const关键字。 
- 定义在一个auto序列的变量必须始终推导成同一类型。例如:
auto a4 = 10, a5 = 20, a6 = 30;//正确
auto b4 = 10, b5 = 20.0, b6 = 'a';//错误,没有推导为同一类型

如果auto关键字带上&号，则不去除const语意。
const int a2 = 10;
auto &b2 = a2;//因为auto带上&，故不去除const，b2类型为const int
b2 = 10; //非法

如果初始化表达式为const或volatile（或者两者兼有），则除去const/volatile语义
const int a1 = 10;
auto  b1= a1; //b1的类型为int而非const int（去除const）
const auto c1 = a1;//此时c1的类型为const int
b1 = 100;//合法
c1 = 100;//非法

如果初始化表达式是引用，则去除引用语义。
int a = 10;
int &b = a;
auto c = b;//c的类型为int而非int&（去除引用）
auto &d = b;//此时d的类型才为int&
c = 100;//a =10;
d = 100;//a =100;

初始化表达式为数组时，auto关键字推导类型为指针。
int a3[3] = { 1, 2, 3 };
auto b3 = a3;
cout << typeid(b3).name() << endl;  ==> int *
    
若表达式为数组且auto带上&，则推导类型为数组类型。
int a7[3] = { 1, 2, 3 };
auto & b7 = a7;
cout << typeid(b7).name() << endl; ==> int[3]

函数或者模板参数不能被声明为auto
void func(auto a)  //错误
{
    //... 
}

时刻要注意auto并不是一个真正的类型。
auto仅仅是一个占位符，它并不是一个真正的类型，不能使用一些以类型为操作数的操作符，如sizeof或者typeid。
时刻要注意auto并不是一个真正的类型。
auto仅仅是一个占位符，它并不是一个真正的类型，不能使用一些以类型为操作数的操作符，如sizeof或者typeid。
    
C++不允许直接推导auto返回类型，如果要用auto返回类型，需要用decltype 声明尾返回类型
如下面代码：
template<typename A,typename B>
auto add(A x, B y) ->decltype(x + y)
{

    return x + y;
}
```

> ## decltype

```cpp
decltype关键字和auto相互对应的，它们经常在一些场所配合使用。decltype可以在编译的时候判断出一个变量或者表达式的类型，例如：
    #include <iostream>
    #include <vector>
    #include <map>
    using namespace std;

    void func(auto can) {
        int a;
    }

    int main() {
        auto num = 1;  //num 是int类型
        decltype(num) num2 = num; //num2 也是int类型

        return 0;
    }

```

## 13 c/c++中的static关键字

```cpp
1. C++中几种静态类型      
    	静态成员变量（面向对象）
    	静态成员函数（面向对象）
    	静态全局变量（面向过程）
    	静态局部变量（面向过程）
    	静态函数（面向过程）

```

> ## 静态成员变量

```cpp
在类内成员变量的声明前加上关键字static，该数据成员就是类内的静态数据成员
静态成员变量有以下特点：

    1. 静态成员变量是该类的所有对象所共有的。对于普通成员变量，每个类对象都有自己的一份拷贝。而静态成员变量一共就一份，无论这个类的对象被定义了多少个，静态成员变量只分配一次内存，由该类的所有对象共享访问。所以，静态数据成员的值对每个对象都是一样的，它的值可以更新；
    2. 因为静态数据成员在全局数据区分配内存，由本类的所有对象共享，所以，它不属于特定的类对象，不占用对象的内存，而是在所有对象之外开辟内存，在没有产生类对象时其作用域就可见。因此，在没有类的实例存在时，静态成员变量就已经存在，我们就可以操作它； 
    3. 静态成员变量存储在全局数据区。static 成员变量的内存空间既不是在声明类时分配，也不是在创建对象时分配，而是在初始化时分配。静态成员变量必须初始化，而且只能在类体外进行。否则，编译能通过，链接不能通过。在Example 5中，语句int Myclass::Sum=0;是定义并初始化静态成员变量。初始化时可以赋初值，也可以不赋值。如果不赋值，那么会被默认初始化，一般是 0。静态数据区的变量都有默认的初始值，而动态数据区（堆区、栈区）的变量默认是垃圾值。
    4. static 成员变量和普通 static 变量一样，编译时在静态数据区分配内存，到程序结束时才释放。这就意味着，static 成员变量不随对象的创建而分配内存，也不随对象的销毁而释放内存。而普通成员变量在对象创建时分配内存，在对象销毁时释放内存。
    5. 静态数据成员初始化与一般数据成员初始化不同。初始化时可以不加 static，但必须要有数据类型。被 private、protected、public 修饰的 static 成员变量都可以用这种方式初始化。
    6. 静态数据成员初始化的格式为：＜数据类型＞＜类名＞::＜静态数据成员名＞=＜值＞ 类的静态成员变量访问形式1：＜类对象名＞.＜静态数据成员名＞ 
    7. 类的静态成员变量访问形式2：＜类类型名＞::＜静态数据成员名＞，也即，静态成员不需要通过对象就能访问。
    8. 静态数据成员和普通数据成员一样遵从public,protected,private访问规则； 
    9. 如果静态数据成员的访问权限允许的话（即public的成员），可在程序中，按上述格式来引用静态数据成员 ； 
    10. sizeof 运算符不会计算 静态成员变量。
        class CMyclass{
            int n;
            static int s;
        };    //则sizeof（CMyclass）等于4
```

> **何时采用静态数据成员？**

```cpp
设置静态成员（变量和函数）这种机制的目的是将某些和类紧密相关的全局变量和函数写到类里面，看上去像一个整体，易于理解和维护。如果想在同类的多个对象之间实现数据共享，又不要用全局变量，那么就可以使用静态成员变量。也即，静态数据成员主要用在各个对象都有相同的某项属性的时候。比如对于一个存款类，每个实例的利息都是相同的。所以，应该把利息设为存款类的静态数据成员。这有两个好处：

    不管定义多少个存款类对象，利息数据成员都共享分配在全局数据区的内存，节省存储空间。一旦利息需要改变时，只要改变一次，则所有存款类对象的利息全改变过来了。

你也许会问，用全局变量不是也可以达到这个效果吗？

同全局变量相比，使用静态数据成员有两个优势：

    静态成员变量没有进入程序的全局命名空间，因此不存在与程序中其它全局命名冲突的可能。可以实现信息隐藏。静态成员变量可以是private成员，而全局变量不能。
```

> ## **面向对象的） 静态成员函数**

```cpp
与静态成员变量类似，我们也可以声明一个静态成员函数。

静态成员函数为类服务而不是为某一个类的具体对象服务。静态成员函数与静态成员变量一样，都是类的内部实现，属于类定义的一部分。普通成员函数必须具体作用于某个对象，而静态成员函数并不具体作用于某个对象。

普通的成员函数一般都隐含了一个this指针，this指针指向类的对象本身，因为普通成员函数总是具体地属于类的某个具体对象的。当函数被调用时，系统会把当前对象的起始地址赋给 this 指针。通常情况下，this是缺省的。如函数fn()实际上是this->fn()。

与普通函数相比，静态成员函数属于类本身，而不作用于对象，因此它不具有this指针。正因为它没有指向某一个对象，所以它无法访问属于类对象的非静态成员变量和非静态成员函数，它只能调用其余的静态成员函数和静态成员变量。从另一个角度来看，由于静态成员函数和静态成员变量在类实例化之前就已经存在可以访问，而此时非静态成员还是不存在的，因此静态成员不能访问非静态成员。
    
    静态成员函数的特点：
    1. 出现在类体外的函数定义不能指定关键字static； 
    2. 静态成员之间可以相互访问，即静态成员函数（仅）可以访问静态成员变量、静态成员函数；
    3. 静态成员函数不能访问非静态成员函数和非静态成员变量； 
    4. 非静态成员函数可以任意地访问静态成员函数和静态数据成员； 
    5. 由于没有this指针的额外开销，静态成员函数与类的全局函数相比速度上会稍快； 
    6. 调用静态成员函数，两种方式：
    通过成员访问操作符(.)和(->)，也即通过类对象或指向类对象的指针调用静态成员函数。直接通过类来调用静态成员函数。＜类名＞::＜静态成员函数名＞（＜参数表＞）。也即，静态成员不需要通过对象就能访问。
```

> ## **(面向过程的）静态全局变量**

```cpp
在全局变量前，加上关键字static，该变量就被定义成为一个静态全局变量。
    静态全局变量有以下特点：

    1. 该变量在全局数据区分配内存；
    2. 未经初始化的静态全局变量会被程序自动初始化为0（自动变量的自动初始化值是随机的）；
    3. 静态全局变量在声明它的整个文件都是可见的，而在文件之外是不可见的； 　
    4. 静态变量都在全局数据区分配内存，包括后面将要提到的静态局部变量。对于一个完整的程序，在内存中的分布情况如下：【代码区】【全局数据区】【堆区】【栈区】，一般程序的由new产生的动态数据存放在堆区，函数内部的自动变量存放在栈区，静态数据（即使是函数内部的静态局部变量）存放在全局数据区。自动变量一般会随着函数的退出而释放空间，而全局数据区的数据并不会因为函数的退出而释放空间。
    
    定义全局变量就可以实现变量在文件中的共享，但定义静态全局变量还有以下好处：
    1. 静态全局变量不能被其它文件所用； 
    2. 其它文件中可以定义相同名字的变量，不会发生冲突； 
```

> ## **(面向过程的）静态局部变量**

```cpp
在局部变量前，加上关键字static，该变量就被定义成为一个静态局部变量。
    通常，在函数体内定义了一个变量，每当程序运行到该语句时都会给该局部变量分配栈内存。但随着程序退出函数体，系统就会收回栈内存，局部变量也相应失效。

但有时候我们需要在两次调用之间对变量的值进行保存。通常的想法是定义一个全局变量来实现。但这样一来，变量已经不再属于函数本身了，不再仅受函数的控制，这给程序的维护带来不便。

静态局部变量正好可以解决这个问题。静态局部变量保存在全局数据区，而不是保存在栈中，每次的值保持到下一次调用，直到下次赋新值。
    静态局部变量有以下特点：

    1. 静态局部变量在全局数据区分配内存； 
    2. 静态局部变量在程序执行到该对象的声明处时被首次初始化，即以后的函数调用不再进行初始化； 
    3. 静态局部变量一般在声明处初始化，如果没有显式初始化，会被程序自动初始化为0； 
    4. 静态局部变量始终驻留在全局数据区，直到程序运行结束。但其作用域为局部作用域，当定义它的函数或语句块结束时，其作用域随之结束； 
```

> ## **(面向过程的）静态函数**

```cpp
在函数的返回类型前加上static关键字,函数即被定义为静态函数。静态函数与普通函数不同，它只能在声明它的文件当中可见，不能被其它文件使用。
    定义静态函数的好处：（类似于静态全局变量）
    静态函数不能被其它文件所用； 其它文件中可以定义相同名字的函数，不会发生冲突； 
```

## 14. c++ atomic

```cpp
 std::atomic 扩展实现 std::atomic_char, std::atomic_int, std::atomic_uint 等是stl中的默认实现
这个类型用于对数据进行原子操作，在操作的过程中可以指定内存规则。 主要的函数如下：
    atomic_store | 保存非原子数据到原子数据结构 |
    atomic_load | 读取原子结构中的数据 |
    atomic_exchange | 保存非原子数据到原子数据结构，返回原来保存的数据 |
    atomic_fetch_add | 对原子结构中的数据做加操作 |
    atomic_fetch_sub/atomic_fetch_sub_explicit | 对原子结构中的数据做减操作 |
    atomic_fetch_and | 对原子结构中的数据逻辑与 |
    atomic_fetch_or | 对原子结构中的数据逻辑或 |
    atomic_fetch_xor | 对原子结构中的数据逻辑异或
刚才提到了在原子操作时候的内存操作规则，内存操作规则主要是 std::memory_order，这是个枚举类型，里面包含着N多规则
    memory_order_relaxed | 不保证顺序 |
    memory_order_consume | 类比生产者-消费者模型中的消费者读取动作（仅是读取，无计数器），保证该操作先于依赖于当前读取的数据（比如后面用到了这次读取的数据）不会被提前，但不保证其他读取操 作的顺序。仅对大多编译环境的多线程程序的编译优化过程有影响。 |
    memory_order_acquire | 类比生产者-消费者模型中的消费者读取动作（仅是读取，无计数器），保证在这个操作之后的所有操作不会被提前，同样对大多编译环境的多线程程序的编译优化过程有影响。 |
    memory_order_release | 类比生产者-消费者模型中的生产者创建动作（仅操作一个数据），保证这之前的操作不会被延后。 |
    memory_order_acq_rel | 同时包含memory_order_acquire和memory_order_release标记 |
    memory_order_seq_cst | 全部存取都按顺序执行，在多核系统上容易成为性能瓶颈 |

    在前面的原子操作的函数中，默认规则都是std::memory_order_seq_cst
    此外，atomic还有一些标记类型和测试操作，比较类似操作系统里的原子操作
    std::atomic_flag : 标记类型
	atomic_flag_test_and_set : 尝试设置为占用（原子操作）
	atomic_flag_clear : 释放（原子操作）
        
//example:
atomic_flag;
std::atomic_flag 是原子布尔类型。不同于所有 std::atomic 的特化，它保证是免锁的;
atomic_flag支持test_and_set(),可以用于实现自旋锁;

        近搞线程池并发过程中突然想起来用 原子类型 来替换一些mutex, rwlock, cond这些东西;
比如一个reactor 的epoll+线程池[ 例如4个 ], 在发送数据时为了保证tcp socket的数据顺序,
必须加锁处理 [ 这种模型不容易控制, 实际工程中别用, 对于 socket 还是有一个线程来处理最简单也避免竞争问题].
然后我发现了个好东西;
下面的测试代码用 atomic_flag 来替换mutex;
一共没几行代码, 但基本说明了问题.
如果你做并发send(socket,... )  看懂了下面的代码, 也就知道该怎么做了

using namespace std;
#include <atomic>
atomic_flag lk  = ATOMIC_FLAG_INIT; //初始化,一开始是false
int g = 0; //全局变量, ++用的
unsigned int  __stdcall th_func(void *arg){
    bool ret = 0;
    int c = 0;
    while(1){
        /*
            test_and_set 的作用:  原子的设置为 true.
            只有当你设置成功 为 true时 ,才返回 false ,
            意思是只有当 lk 为 false 时, 你才能设置成功,
            否则返回之前的值 : true;
                  
        */
 
        //原子的设置 true, 50个线程只有一个线程能设置为true,返回false,其他都返回true
        /* 
             可设置 std::memery_order_relaxed
             不需要保证内存顺序, 这里只要保证 原子性就ok , 因此可随意他的内存顺序.
            
        */
        ret = lk.test_and_set();  //就这一行代码 ,看懂了就全懂了
 
        if(!ret){  
// 50个线程中只有唯一一个线程在原子的设置成功后进入
//这里相当于进入 mutex了
 
            ++g;
            ++c;
// 把lk原子的设置为 false , 相当于 unlock
            lk.clear();  
            if(c == 100000)  
                break;
        }
 
    }
 
    return 0;
}
 
 
int main(int argc, char* argv[])
{
 
    static const int n = 50;
    HANDLE * arr = new HANDLE[n];
 
// 起线程
    for(int i = 0; i <  n; ++i){
        arr[i] = (HANDLE)_beginthreadex(0,0,th_func,0,0,0);
    }
 
    WaitForMultipleObjects(n,arr,TRUE,INFINITE);
    cout << "main done :" << g << endl;
    return 0;
}

接下来的问题就容易解决了,
在线程池中:

    ret = lk.test_and_set();
    if(!ret){
        send / write
    }
    else{
        仍回队列 / 某个容器里
        
    }
```

