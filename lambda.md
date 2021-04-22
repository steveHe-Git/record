
## 1. 概述
```cpp
C++ 11 中的 Lambda 表达式用于定义并创建匿名的函数对象，以简化编程工作。
Lambda 的语法形式如下：

[函数对象参数] (操作符重载函数参数) mutable 或 exception 声明 -> 返回值类型 {函数体}

可以看到，Lambda 主要分为五个部分：[函数对象参数]、(操作符重载函数参数)、mutable 或 exception 声明、-> 返回值类型、{函数体}.
```

## 2. Lambda 语法分析
> 2.1 [函数对象参数]
```cpp
标识一个 Lambda 表达式的开始，这部分必须存在，不能省略。函数对象参数是传递给编译器自动生成的函数对象类的构造
函数的。函数对象参数只能使用那些到定义 Lambda 为止时 Lambda 所在作用范围内可见的局部变量(包括 Lambda 所在类
的 this)。函数对象参数有以下形式：

    空。没有任何函数对象参数。
    =。函数体内可以使用 Lambda 所在范围内所有可见的局部变量（包括 Lambda 所在类的 this），并且是值传递方式（相
    当于编译器自动为我们按值传递了所有局部变量）。
    &。函数体内可以使用 Lambda 所在范围内所有可见的局部变量（包括 Lambda 所在类的 this），并且是引用传递方式
    （相当于是编译器自动为我们按引用传递了所有局部变量）。
    this。函数体内可以使用 Lambda 所在类中的成员变量。
    a。将 a 按值进行传递。按值进行传递时，函数体内不能修改传递进来的 a 的拷贝，因为默认情况下函数是 const 的，要
    修改传递进来的拷贝，可以添加 mutable 修饰符。
    &a。将 a 按引用进行传递。
    a，&b。将 a 按值传递，b 按引用进行传递。
    =，&a，&b。除 a 和 b 按引用进行传递外，其他参数都按值进行传递。
    &，a，b。除 a 和 b 按值进行传递外，其他参数都按引用进行传递。

###捕获外部变量
[] 	不捕获任何外部变量
[变量名, …] 	默认以值得形式捕获指定的多个外部变量（用逗号分隔），如果引用捕获，需要显示声明（使用&说明符）
[this] 	以值的形式捕获this指针
[=] 	以值的形式捕获所有外部变量
[&] 	以引用形式捕获所有外部变量
[=, &x] 	变量x以引用形式捕获，其余变量以传值形式捕获
[&, x] 	变量x以值的形式捕获，其余变量以引用形式捕获

//Lambda表达式通过在最前面的方括号[]来明确指明其内部可以访问的外部变量，这一过程也称过Lambda表达式“捕获”了外部变量
int main() {
    int a = 123;
    auto f = [a] { cout << a << endl; }; 
    f(); // 输出：123

    //或通过“函数体”后面的‘()’传入参数
    auto x = [](int a){cout << a << endl;}(123); 
}

//在Lambda表达式中，如果以传值方式捕获外部变量，则函数体中不能修改该外部变量，否则会引发编译错误。那么有没有办法可以修改值捕获的外部变量呢？这是就需要使用mutable关键字，该关键字用以说明表达式体内的代码可以修改值捕获的变量
//被mutable修饰的lambda表达式就算没有参数也要写明参数列表 
int main() {
    int a = 123;
    auto f = [a]()mutable { cout << ++a; }; // 不会报错//()必须有，不然会出错
    cout << a << endl; // 输出：123
    f(); // 输出：124
}

//
class A
{
    public:
    int i_ = 0;

    void func(int x,int y){
        auto x1 = [] { return i_; };                             //error,没有捕获外部变量
        auto x2 = [=] { return i_ + x + y; };               //OK
        auto x3 = [&] { return i_ + x + y; };               //OK
        auto x4 = [this] { return i_; };                        //OK
        auto x5 = [this] { return i_ + x + y; };            //error,没有捕获x,y
        auto x6 = [this, x, y] { return i_ + x + y; };    //OK
        auto x7 = [this] { return i_++; };                   //OK
    }
};
 
 int a=0 , b=1;
 auto f1 = [] { return a; };                            //error,没有捕获外部变量    
 auto f2 = [&] { return a++ };                      //OK
 auto f3 = [=] { return a; };                         //OK
 auto f4 = [=] {return a++; };                     //error,a是以复制方式捕获的，无法修改
 auto f5 = [a] { return a+b; };                     //error,没有捕获变量b
 auto f6 = [a, &b] { return a + (b++); };      //OK
 auto f7 = [=, &b] { return a + (b++); };     //OK

//没有捕获变量的lambda表达式可以直接转换为函数指针，而捕获变量的lambda表达式则不能转换为函数指针
typedef void(*Ptr)(int*);
Ptr p = [](int* p) { delete p; };              //OK
Ptr p1 = [&] (int* p) { delete p; };         //error
```

> 2.2 (操作符重载函数参数)
```cpp
标识重载的 () 操作符的参数，没有参数时，这部分可以省略。参数可以通过按值（如: (a, b)）和按引用 (如: (&a, &b)) 两种
方式进行传递。

void biggies(vector<string> &words, 
                  vector<string> ::size_type sz,
                  ostream &os = cout, char c = ' ')
{
     //打印count的语句改为打印到os
    for_each(words.begin(), words.end(),
                  [&os, c] (const string &s) { os << s << c; });          
}

我们不能拷贝ostream对象，因此捕获os的唯一方法就是捕获其引用（或指向os的指针);
当引用方式捕获一个变量时，必须保证lambda执行时变量是存在的;
一般来说，我们应该尽量减少捕获的数据量，来避免潜在的捕获导致的问题。而且，如果可能的话，应该避免捕获指针或引用;
```

> 2.3 mutable 或 exception 声明
```cpp
这部分可以省略。按值传递函数对象参数时，加上 mutable 修饰符后，可以修改传递进来的拷贝（注意是能修改拷贝，而不是
值本身）。exception 声明用于指定函数抛出的异常，如抛出整数类型的异常，可以使用 throw(int)。
```

> 2.4 -> 返回值类型
```cpp
标识函数返回值的类型，当返回值为 void，或者函数体中只有一处 return 的地方（此时编译器可以自动推断出返回值类型）
时，这部分可以省略。
```
> 2.5 {函数体}
```cpp
标识函数的实现，这部分不能省略，但函数体可以为空。
```

## 3. 示例
```cpp
[] (int x, int y) { return x + y; } // 隐式返回类型
[] (int& x) { ++x;  } // 没有 return 语句 -> Lambda 函数的返回类型是 'void'
[] () { ++global_x;  } // 没有参数，仅访问某个全局变量
[] { ++global_x; } // 与上一个相同，省略了 (操作符重载函数参数)

可以像下面这样显示指定返回类型：

[] (int x, int y) -> int { int z = x + y; return z; }

在这个例子中创建了一个临时变量 z 来存储中间值。和普通函数一样，这个中间值不会保存到下次调用。什么也不返回的
Lambda 函数可以省略返回类型，而不需要使用 -> void 形式。

Lambda 函数可以引用在它之外声明的变量. 这些变量的集合叫做一个闭包. 闭包被定义在 Lambda 表达式声明中的方括
号 [] 内。这个机制允许这些变量被按值或按引用捕获。如下图的例子：
```

> 3.1 示例 1
```cpp
std::vector<int> some_list;
int total = 0;
for (int i = 0; i < 5; ++i) some_list.push_back(i);
std::for_each(begin(some_list), end(some_list), [&total](int x)
{
    total += x;
});

此例计算 list 中所有元素的总和。变量 total 被存为 Lambda 函数闭包的一部分。因为它是栈变量（局部变量）total 引
用，所以可以改变它的值。
```

> 3.2 示例 2
```cpp
std::vector<int> some_list;
int total = 0;
int value = 5;
std::for_each(begin(some_list), end(some_list), [&, value, this](int x)
{
    total += x * value * this->some_func();
});

此例中 total 会存为引用, value 则会存一份值拷贝。对 this 的捕获比较特殊，它只能按值捕获。this 只有当包含它的最靠近
它的函数不是静态成员函数时才能被捕获。对 protect 和 private 成员来说，这个 Lambda 函数与创建它的成员函数有相同
的访问控制。如果 this 被捕获了，不管是显式还是隐式的，那么它的类的作用域对 Lambda 函数就是可见的。访问 this 的
成员不必使用 this-> 语法，可以直接访问。
```
## 4. 总结
```cpp
不同编译器的具体实现可以有所不同，但期望的结果是: 按引用捕获的任何变量，Lambda 函数实际存储的应该是这些变量在
创建这个 Lambda 函数的函数的栈指针，而不是 Lambda 函数本身栈变量的引用。不管怎样，因为大多数 Lambda 函数都
很小且在局部作用中，与候选的内联函数很类似，所以按引用捕获的那些变量不需要额外的存储空间。

如果一个闭包含有局部变量的引用，在超出创建它的作用域之外的地方被使用的话，这种行为是未定义的!

Lambda 函数是一个依赖于实现的函数对象类型,这个类型的名字只有编译器知道. 如果用户想把 lambda 函数做为一个参数来
传递, 那么形参的类型必须是模板类型或者必须能创建一个 std::function 类似的对象去捕获 lambda 函数.使用 auto 关键字
可以帮助存储 lambda 函数,

auto my_lambda_func = [&](int x) { /* ... */ };
auto my_onheap_lambda_func = new auto([=](int x) { /* ... */ });

这里有一个例子, 把匿名函数存储在变量、数组或 vector 中,并把它们当做命名参数来传递

一个没有指定任何捕获的 lambda 函数,可以显式转换成一个具有相同声明形式函数指针.所以,像下面这样做是合法的:

auto a_lambda_func = [](int x) { /* ... */ };
void (*func_ptr)(int) = a_lambda_func;
func_ptr(4); // calls the lambda
```
