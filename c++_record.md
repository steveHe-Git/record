# c++ 随笔记录

1.  C++11 中的std::function和std::bind

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