# c++学习记录

## C++ stl 泛型编程学习

### 1. stl基础语法

1. 函数的返回值，参数，和局部变量都可以使用类型参数；

2. 模板形式

   定义用法：

   template<typename 类型形参1, typename 类型形参2 ...>

   调用用法：

   函数模板名<类型实参1, 类型实参2, ...>

### 2. stl支持支持隐式推断(通过隐式推断实例化函数模板)

- 比如example1 
  ```
  int a,b=0; double c, d=0;
  template<typename T>
  foo(T const& x, T const& y)
  foo(a, b)                        //合法
  foo(a, c)                        //不合法的(T的类型必须一致)，隐式推断不允许隐式类型转换

  但是可以显示类型转换
  foo(a, static_cast<int>(c);      //合法
  foo(static_cast<double>(a), c);  //合法
  ```

- 比如example2
  ```
  template<typename R, typename T>
  R bar(T const& t) {R r; return r}
  i = bar(j) //不合法，返回类型不能隐式推断；
  i = bar<int ,double>(j)  //合法
  i = bar<int>(j)  //合法， 指定返回值的类型为int;
  ```
  
### 3. stl函数模板的重载
- 形参不同的同名函数模板之间，以及函数模板和同名具体函数之间，都可以构成重载关系
  ```
  template<typename T>
  T const & max(T const& x, T const& y)   //1
  T* const& max(T* const& x, T* const& y) //2
  const char* const& max(const char* const& x, const char* const& y) //3
  template<typename T>
  T const& max(T const& x, T const& y, T const& z) //4
  ```

  上面四个函数可以构成重载关系
  ```
  char const* x= "ABC" 
  char const* y= "AB"
  char const* z= "C"
  cout << max(x, y) << endl;               // 调用3
  cout << max<>(x, y) << endl;             // 调用2
  cout << max<const char*> (y, z)<< endl;  // 调用1
  cout << max(x, y, z)<< endl;             // 调用4
  ```

- 补充
  ```
  char const* x= "ABC"     //const 修饰的是指针指向的变量为常量，值不能改变，但是指针地址可以修改(改变指向)
  const char*              //同上
  char* const x            //const 修饰的是指针，表示的是指针指向的内容可以改变，但是指针的地址不可改变    
  ```
  
### 4. stl类模板
- 类的成员函数、成员变量和基类都可以含有参数化的类型，这样的类就是类模板
  ```
  template<typename T>
  class Comparator{
  public:
  	Comparator(T x, T y);
  	T max(void) const;
  	T min(void) const;
  private:
  	T m_x;
  	T m_y;
  }

  template<typename T>
  Comparator<T>::Comparator(T x, T y):m_x(x), m_y(y){}
  
  Comparator<int> ci //构造类
  ```
  
- 补充说明:
  T max(void) const{};
  使用const关键字进行说明的成员函数，称为常成员函数。只有常成员函数才有资格操作常量或常对象，没有使用const关键字说明的成员函数不能用来操作常对象。常成员函数说明格式如下：
  <类型说明符(int)> <函数名(fun)> (<参数表(int,int)>() const；

- 类模板的静态成员变量，每个实例化类各有一份
  ```
  template<typename T>
  class A {
  public:
  static void print(void){
		cout<< &m_i << " " << &m_t<< endl;
  }
  private:
  	static int m_i;      //静态成员变量的声明
  	static T m_t;
  }
  template<typename T>
  int A<T>::m_i;       //静态成员变量的定义
  template<typename T>
  T A<T>::m_t;
  ```

- 类模板模板参数可以带缺省值
  ```
  template<typename X = int, typename Y = double, typename Z = string>
  class A{
  public:
  private:
  	X m_x;
  	Y m_y;
  	Z m_z;
  }
  ```
  
- 如果某个类模板参数带有缺省值，那么它后面的所有的参数必须带有缺省值
  ```
  template<typename X , typename Y = double, typename Z = string> //合法
  template<typename X = int, typename Y = double, typename Z> //不合法
  ```

- 函数模板的参数不能带有缺省值(因为类模板不支持隐式推断，但是函数模板支持隐式推断和缺省值，两者不知道用哪一个，有冲突)
  
- 函数模板的特化
  ```
template<typename T>
T max(T x, T y) {
	return x>y ? x:y;
}
//针对char const* 类型的特化版本
template<>
char const* max<char const *>(char const * *x, char const* *y){
	return strcmp(x, y)<0?y:x;
}
int main() {
	cout << max<string>("hello", "world"); //输出是world，进行ASCII码的比较；
	cout<<max("hello", "world");           //输出是hello，进行地址的比较；没有指定类型的化，字面值类型为 char const*(指针类型，指针指向的内容不能发生改变)
return 0;
}
  ```

- 类模板的特化
  1.  全类特化版本
  ```
  template<>
  class Comparator<char const *>{
  public:
  Comparator(char const* x, char const * y):m_x(x), m_y(y){}
  char const* max() const {
  	return strcmp(m_x, m_y) < 0 ? m_x:m_y;
  }
  private:
  char const* m_x;
  char const* m_y;
  }
  ```

  2.  成员的特化版本
  ```
  template<>
  char const* Comparator<char const *>::max() const {
  	return strcmp(m_x, m_y) < 0 ? m_x:m_y;
  }
  ```

  3.  类模板的局部特化(偏特化)
  + 针对部分类型参数取特定类型进行特化
  + 针对类型参数之间的某种关联性进行特化
  + 针对指针或者数组进行特化
  ```
  //通用版本
  template<typename T1, typename T2>
  class Dual {
  public:
  	static void printType(void){
  		cout << "Dual<T1, T2>" << endl;
  	}
  };
  // 针对第二个类型参数short的局部特化版本
  template<typename T>
  class Dual<T, short>{
  	static void printType(void){
  		cout << "Dual<T, short>" << endl;
  	}
  };
  //针对两个类型参数取相同类型的局部特化版本
  template<typename T>
  class Dual<T, T>{
  	static void printType(void){
  		cout << "Dual<T, T>" << endl;
  	}
  };
  //针对两个类型参数取某种类型指针局部特化版本
  template<typename T1, typename T2>
  class Dual<T1*, T2*>{
  	static void printType(void){
  		cout << "Dual<T1*, T2*>" << endl;
  	}
  };
  ```
  4. 可变参数模板
  ```cpp
      typename...：声明一个可变参数模板形参
    sizeof...：获取参数包内参数的数量
      Pattern...：以某一模式展开参数包
       
      //example
          
      // 递归终点
      void print() {}
  
      // 分解出一个val + 剩下的所有val
      // 相当于：void print(const T &val, const Types1 &Args1, const Types2 &Args2, const Types3 &Args3, ...)
      template <typename T, typename... Types>
      void print(const T &val, const Types &... Args)
      {
          // 每次打印一个val
          cout << val << endl;
          //打印参数的个数
          cout << sizeof...(Args) << endl;
  
          // 相当于：print(Args1, Args2, Args3, ...);
          // 递归地继续分解...
          print(Args...);
      }
  
  
      int main()
      {
          print(1, 2., '3', "4");
      } 
  
  
  	//稻草人函数
  	//如何实现：判定类型A是否能够基于隐式类型转换转为B类型？
  	template <typename A, typename B>
      class IsCastable
      {
      private:
  
          // 定义两个内存大小不一样的类型，作为“布尔值”
          typedef char __True;
          typedef struct { char _[2]; } __False;
  
  
          // 稻草人函数
          static A __A();
  
  
          // 只要A到B的隐式类型转换可用，重载确定的结果就是此函数...
          static __True __Test(B);
  
  
          // ...否则，重载确定的结果才是此函数(“...”参数的重载确定优先级低于其他一切可行的重载版本)
          static __False __Test(...);
  
  
      public:
  
          // 根据重载确定的结果，就能够判定出隐式类型转换是否能够发生
          static constexpr bool Value = sizeof(__Test(__A())) == sizeof(__True);
      };
  
  	首先，我们声明了两个大小不同的类型，作为假想的“布尔值”。也许你会有疑问，这里为什么不使用int或double之类的类型作为False？这是由于C语言并未规定“int、double必须比char大”，故为了“强行满足标准”（你完全可以认为这是某种“教条主义或形式主义”），这里采用了“两个char一定比一个char大一倍”这一简单道理，定义了False。
  	然后，我们声明了一个所谓的“稻草人函数”，这个看似毫无意义的函数甚至没有函数体（因为并不需要，且接下来的两个函数也没有函数体，与此函数同理）。这个函数唯一的目的就是“获得”一个A类型的值“给sizeof看”。由于sizeof的不求值特性，此函数也就不需要（我们也无法提供）函数体了。那么，为什么不直接使用形如“T()”这样的写法，而需要声明一个“稻草人函数”呢？我想，不用我说你就已经明白原因了：这是因为并不是所有的T都具有默认构造函数，而如果T没有默认构造函数，那么“T()”就是错误的。
  接下来是最关键的部分，我们声明了一对重载函数，这两个函数的区别有二：
      返回值不同，一个是sizeof的结果为1的值，而另一个是sizeof的结果为2的值
      形参不同，一个是B，一个是“...”
  也就是说，如果我们给这一对重载函数传入一个A类型的值时，由于“...”参数的重载确定优先级低于其他一切可行的重载版本，只要A到B的隐式类型转换能够发生，重载确定的结果就一定是调用第一个版本的函数，返回值为__True；否则，只有当A到B的隐式类型转换真的不可行时，编译器才会“被迫”选择那个编译器“最不喜欢的版本”，从而使得返回值为__False。返回值的不同，就能够直接体现在sizeof的结果不同上。所以，只需要判定sizeof(__Test(__A()))是多少，就能够达到我们最终的目的了。下面请看使用示例：
      int main()
      {
          cout << IsCastable<int, double>::Value << endl;  // true
          cout << IsCastable<int, string>::Value << endl;  // false
      }
  ```
  
  5. 补充
  + 针对部分类型参数取特定类型进行特化
    编译器优先选择特化程度最高的版本
  + 针对类型参数之间的某种关联性进行特化
    编译器优先选择匹配程度最高的版本
  + 针对指针或者数组进行特化
    编译器优先选择指针或者数组的特化版本
  
- 模板类的应用(auto_ptr 智能指针)

  ```
  class A{
  public:
    A(int data =0 ): m_data=data{
          cout<< "A构造"<<endl;
      }
      ~A{
          cout<< "A析构"<<endl;
      }
      void print() const{
          cout << m_date;
      }
  private
  	m_date;
  }
  ```
  ```
template<typename T>
  class good_ptr{
  public:
  good_ptr(T* p = NULL) : m_p(p){}
  ~good_ptr(){
  	if (m_p) {
  		delete m_p;
  		m_p = NULL;
  	}
  }
  //转移语义
  good_ptr (good_ptr& that):m_p(that.release()){}
  good_ptr& operator= (good_ptr& that){
  	if (&that != this)
      	reset(that.release());
      return *this;
  }

  T& operator* ()const {   //&&&
  	return *m_p;
  }
  T* operator->() const {
  	//或者
  	//return &*(*this); // (*this)目标对象, *(*this)复用&&& 获取对象的引用，&获取引用对象的地址；
  	return m_P;
  }
  private:
  	//转移语义
  	T* release(){
  		T* p = m_p;
  		m_p = NULL;
  		return p;
  	}
  	//重置指针
  	void reset(T* p){
  		if(m_p){
  			delete m_p;
  			m_p = p;
  		}
  	}
  	T* m_p;
  }
  //数组特化版本(类的局部特化只能是全类进行)
  template<typename T>
  class good_ptr<T[]>{
  public:
  good_ptr(T* p = NULL) : m_p(p){}
  ~good_ptr(){
  	if (m_p) {
  		delete[] m_p;
  		m_p = NULL;
  	}
  }
  //转移语义
  good_ptr (good_ptr& that):m_p(that.release()){}
  good_ptr& operator= (good_ptr& that){
  if (&that != this)
      	reset(that.release());
      return *this;
  }

  T& operator* ()const {   //&&&
  	return *m_p;
  }
  T* operator->() const {
  	//或者
  	//return &*(*this); // (*this)目标对象, *(*this)复用&&& 获取对象的引用，&获取引用对象的地址；
  	return m_P;
  }
  private:
  	//转移语义
  	T* release(){
  		T* p = m_p;
  		m_p = NULL;
  		return p;
  	}
  	//重置指针
  	void reset(T* p){
  		if(m_p){
  			delete[] m_p;
  			m_p = p;
  		}
  	}
  	T* m_p;
  }

  int main(){
  	good_ptr<A> pa(new A(1234));
  	cout << (*pa).m_data<<endl;
  	pa->print();
  	good_ptr<A> pb = pa;//默认的赋值拷贝。浅拷贝，两个指针指向同一块内存，会出现double free,需要转移语义
  	good_ptr<A[]>pa=(new A[2]);
  }
  ```
  

### 5. stl非类型参数
- stl非类型参数实例(可以有缺省值)
  ```
template<typename T = int, size_t num = 256>
class Array{
	public:
	T& operator[] (size_t i){
		return m_array[i];
	}
	const T& operator[] (size_t i)const{
		return const_cast<Array>(*this)[i];
}
	private:
T m_array[num];
}
	int main() {
	Array<int, 10>ai;
	Array<double, 10+5>di;
	int x =5 ;
Array<double, 10+x>fi;         //不合法，模板是在编译期间, 变量x=5是在运行期间；
  }
  ```
- 模板的非类型参数不能是浮点类型(float,double)，类(string)，字符串字面值(char const*)
  ```
template<double VAT>
void foo(){
	cout << VAT <<endl;
}
foo<3.14>()                     //不合法出错，float,double, 类(string)类型不可以，int ,char,uint,long long类型都可以
  ```
- 模板的非类型参数
  ```
  //不合法，模板的非类型实参不能是去全局指针
  char const* g_global = "global";
  //合法，模板的非类型实参可以是外部变量
  extern const char g_extern[] = "extern"; //
  template<char const* VAT>
void hum(void){
	cout << VAT <<endl;
}
  ```
  
### 6. stl模板的嵌套和递归
1. List<Array<int> > la;		//嵌套

2. Array<Array<int> > aa;	//递归(使用自己构造自己)

3. 类模板的模板参数

  ```
class A{
public:
	A(T const& data):m_data(data){}
	T m_data;
}
template<typename K, typename V,
	template<typename T> class C> //说明是一个类模板(参数型类模板)
class B {
public:
	B(K const&k, V const& v):m_k(k), m_v(v){}
	C<K> m_k;
	C<V> m_v;
}
int main(void){
	B<string, double, A>b("PAI", 3.14);
	cout << b.m_k.m_data << "="
		<<b.m_v.m_data;
}
  ```
4. 类模板中的模板函数

  ```cpp
template<typename K>
class A{
public:
	A(K const& data):m_data(data){}
	template<typename V>
    void foo(V const&data){
    	cout<<m_data<<"="<<data<<endl;
    }
    K m_data;
}
//template<typename K>
//template<typename V>
//void A<K>::foo(V const&data){
//	cout<<m_data<<"="<<data<<endl;
//}
int main(){
	A<string> a("PAI");
	foo<double>(3.14);
}
  ```
5. class和typename
class    -类
		 \模板（定义模板参数的时候class和typename可以替换）
		 /参数
typename -解决嵌套依赖
  1. 解决嵌套依赖实例
  ```cpp
class A {
public:
	class B{};
	typedef int BOOL;
}
template<typename T>
void foo(T t) {
    //因为T::B不知道是什么类型(可能是T中的变量，变量不能定义变量，加上typename T::B表明是一个类型)
	typename T::B x;     //前面必须加typename，
	typename T::BOOL y;
}
int main() {
	A a;
	foo(a) 				//隐式推断T为A类型
}

//vector打印(解决嵌套依赖的问题)
template<typename T>
void disp(vector<T>::const& vec) {
    //加上typename解决嵌套依赖问题(不加会报错)
    for(typename vector<T>::const_interator it = vec.begin(); it != vec.end();++it)
        cout << *it << ' ';
    cout <<endl;   
}

//example:
struct A { typedef int TypeOrValue; };
struct B { static constexpr int TypeOrValue = 0; };
 
template <typename T>
struct C
{
    T::TypeOrValue;  // 这是什么？
};

上例中，如果T是A，则T::TypeOrValue是一个类型；而如果T是B，则T::TypeOrValue是一个数。我们称这种含有模板参数的，无法立即确定语义的名称为“依赖型名称”。所谓“依赖”，意即此名称的确切语义依赖于模板参数的实际类型。

对于依赖型名称，C++规定：默认情况下，编译器应认为依赖型名称不是一个类型；如果需要编译器将依赖型名称视为一个类型，则需要前置typename关键词。请看以下示例以进行辨析
    
T::TypeOrValue * N;           // T::TypeOrValue是一个值，这是一个乘法表达式
typename T::TypeOrValue * N;  // typename T::TypeOrValue是一个类型，声明了一个这样类型的指针
  ```

### 7 STL各种容器(vector, list, map, deque, stack)

>STL的特性
```cpp
1.STL里的所有的组件，都是通过模板定义的，全面支持泛型
2.STL所追求的目标就是最大程度上降低数据结构和算法与具体算法的相关性
3.STL的设置宗旨是通过一个尽量小的框架，实现尽量打的弹性
    
STL主要的容器类型
1. 向量(vector)： 内存连续，下标访问
2. 列表(list): 内存不连续。不支持下标访问，随机插值高效(链表)
3. 双端队列(deque): 类似于向量，但是两端是开放的
以上三种容器合称为线性容器，强调元素之间的前后关系
4. 堆栈(stack)：在一端的压入和弹出，后进先出
5. 队列(queue):从一端压入，从另外一端弹出，先进先出
6.优先队列(priority_queue):从一端压入，从另外一端弹出，优者先出
以上三种容器合称为适配器容器，构建三种线性容器之前的封装
7.映射(map):通过平衡有序二叉树，存放key-value对的集合，提高树搜索性能。key必须唯一，一对一对应
8.多重映射(multimap):允许key重复的映射，一对多对应
9.集合(set):没有value的映射(强调唯一性)
10.多重集合(multiset)：没有value的多重映射
以上四种容器合称为关联容器
    
STL容器的共性
1.所有的容器都支持深拷贝
2.所有的容器都可以和同型容器做关系比较
3.容器的元素都是副本
4.所有的容器都支持迭代器
```
> vector

```cpp
1. 从一段连续的内存空间存放数据；
2. 具备常数时间的随机访问特性；
3. 支持下标运算符；
4. 支持动态内存管理:向量把他们所有的元素存放在一个连续的内存块中，这并不会妨碍新元素被无限地插入到容器中，如果当前内存内无法满足一个向量中所有元素存放的需要，那么这个向量就会自动转移到一个新的内存，原先位置上的所有的元素都将被复制到新的内存中，而原先的内存空间则将被释放；
5.通过预分配空间降低动态内存管理的开销；
6.也支持随机位置的插入和删除，但是只有在向量尾部做插入和删除是高效的；
    
实例化：
#include <vector>
vector<元素类型> 向量对象
vector<int> vi;
vector<元素类型> 向量对象(初始大小)
vector<int> vi(10);
初始大小范围内的元素，基本类型，初始化为0(10个元素，全部初始化为0)
vector<int> vi(10, 1234);
初始大小范围内的元素，基本类型，初始化为0(10个元素，全部初始化为1234)
vector<int> vi(起始迭代器, 终止迭代器);
用另一个容器从起始迭代器开始，到终止迭代器之前为止的范围内的元素，初始化所构造的向量

int a[5] = {1, 2, 3, 4, 5};
vector<int>(a, a+5)
vector<int>(&a[0], &a[5]) <==>vector<int>(&*(a+0), &*(a+5);

front/back/push_back/pop_back                                         
迭代器
四个迭代器
interator/const_iterator
reverse_interator/const_reverse_interator
随机迭代器
1. 只有连续内存容器(vector/deque)才有随机迭代器
2. (随机迭代器)支持和整数的加减运算，支持迭代器的比较和相减运算 
                                          
insert/erase
insert: 插入元素，返回插入元素的迭代器；
erase:删除元素，返回删除元素的后一个元素的迭代器；
                                          
大小和容量
大小：实际容纳元素的个数
容量：最多容纳元素的个数
size: 返回大小； resize:改变大小，可增可减，增构造，减析构
clear:清空，等价于resize(0)
empty: 判断是否为空
capacity:返回容量
reserve:预留容量，只增不减，新增部分不初始化

查找和排序(find sort)
interator find(interator begin, interator end, const value_type& val)
在begin和end之间查找相匹配的第一个元素，返回该元素的迭代器。如果查找失败，返回第二个参数
void sort(interator begin, interator end);
void sort(interator begin, interator end, 比较器函数名/或者比较器类());
所谓比较器，就是返回bool类型值，用于判断第一个元素参数是否小于第二个元素参数的函数或函数对象
bool 比较器函数(元素类型 const&a, 元素类型 const&b) {
    如果认为a小于b,则返回true, 否则返回false;
}
class 比较器类 {
public:
    bool operator()(元素类型 const&a, 元素类型 const&b) {
    	如果认为a小于b,则返回true, 否则返回false;
	}
}
sort函数的第三个参数，可以是比较器函数指针，或者是比较器类对象;                                         
                                          
向量不适应保存大对象，如果对象需要使用较多的内存资源，建议通过构造和析构函数动态分配和释放该资源
                                          
7. 类类型的向量
元素类型往往需要考虑一下几个问题：
是否支持深拷贝 拷贝构造和拷贝赋值
是否支持相等性比较 find "==" 或类型转换
是否支持小于运算， 小于号，sort 或者比较器                      
```

> 字符串string

```cpp
1. 源自basic_string<T>模板的类型别名
typedef basic_string<char>string
typedef basic_string<wchar_t>wstring
char-1个字节-string表示多字节字符串
wchar_t-2/4个字节-wstring表示宽字节字符串
    
2. 创建字符串对象
string str;
string str("");
string str = "";
string str("Hello world!");
string str = "Hello world!";
string str = string("");
string str = string("hello world!");
string* str = new string;
string* str = new string();
string* str = new string("");
string* str = new string("hello world");
C风格字符串->c++风格字符串
char cstr[] = "hello world";
string cpps(cstr);
string cpps = cstr;
C++风格字符串->C风格字符串
string cpps(hello world);
char const* cstr = cpps.c_str();

3.字符串的运算
1. 拼接： +
2. 赋值： =/+=
3. 关系：</<=/>/>=/==/!= 大小写敏感
4. 输入、输出： >>/<<
5. 下标：[]

4. 大小和容量
size/resize/clear/empty/capacity/reserve
string str("123456789");
str.length() -> 9
str.size() ->9
sizeof(str)->4/8
strlen(str.c_str()) ->9
    
5. find_first_of 与 find_first_not_of (两者是相反的)；substr
    size_type string::find_first_of(string const& str, size_type pos = 0);
返回调用字符串中，从pos处开始的，第一个出现在str中的字符串的下标，如果没有出现在字符串中的下标，则返回string::npos
string s1("hello123world");
string s2("0123456789");
s1.find_first_of(s2) --> 5
s1.find_first_of(s2, 6) --> 6
s1.find_first_of(s2, 8) --> string::npos   
    
string substr (size_type pos, size_type len = string::npos)
返回调用字符串中从pos处开始len个字符的子串
 
6.字符串的单字符访问
1. 下标运算符:str[i], 不检查下标越界
2. char& at(size_type index);
    char const& at(size_type index)const;
检查下标越界，抛出std::out_of_range异常
    
7. 查找与替换
size_type find(const string& str, size_type pos);
返回调用字符串从pos开始，第一次出现str的首个字符的下标
string s1 = "abchelloxyz";
string s2 = "hello"
s1.find(s2, 0) ->3
string& repalce (size_type pos size_type len, string const& str)
将调用字符串从第pos个字符开始的len个字符替换为str字符串，同时返回调用字符串的自引用
string s1 = "abchelloxyz";
string s2 = "steve"
s1.replace(3, 5, s2) s1->abcstevexyz
    
8.插入删除
string& insert(size_type pos, const string& str)
在调用字符串第pos个字符前面插入str字符串
string& erase(size_type pos = 0, size_type len = string::npos)
在调用字符串第pos个字符开始删除len个字符
    
9.交换和复制
void swap(string& from)
将调用字符串和from字符串的内容做交换
string& assign(string const& str, size_type pos, size_type len)
将str字符串从pos开始的len个字符复制到调用字符串
```

> 双端队列

```cpp
1. 除了capacity()和reserve()以外向量的接口双端队列都有
2. 双端队列增加了在头部压入和弹出的接口
   push_front/pop_front
3. 虽然双端队列的物理结构也是连续的内存空间，但是它在其内存空间的首尾两端保持了适度的开放性，因此在距离首尾两端等距的位置做insert和erase,性能是一致的
4. 和向量一样也支持通过下标和随机迭代器访问容器中的元素
5. 所占用的内存比向量略多，动态内存管理所花费的时间也比向量略多
注意
    （push_back/push_front/pop_back/pop_front/insert/erase/resize/clear）会导致原先初始化的迭代器不在指向先前的元素，如果需要保证迭代器和其目标的相对位置不变，需要人为地同步
```

> list(列表)

```cpp
1. 列表按照链式线性表的形式存储数据，每个数据元素存放在内存不连续的节点之中
2. 不支持下标运算符，也不支持随机迭代
3. 大部分成员函数和双端队列一样
  front/push_front/pop_front
  back/push_back/pop_back
  insert/erase
  size/resize/clear/empty
  begin/end/rbegin/rend
4. 特有的成员函数
  remove 删除所有匹配的元素
  unique 连续重复的元素做唯一化
  10 20 20 20 30 10 20 20 40
           \|/
  10 20 30 10 20 40
  splice -将参数列表的全部或者一部分插入到调用列表中
  void splice(interator pos, list& lst);
  将lst全部插入到调用列表的pos之前
  void splice(interator pos, list& lst， interator del);
  将lst中del所指向的元素剪切到调用列表的pos之前
  void splice(interator pos, list& lst， interator begin, interator end);
  将lst中begin开始end结束的元素剪切到调用列表的pos之前
      
  merge - 将有序的参数列表合并到有序的调用列表中，并保证合并以后的结果依然有序
  void merge(list& lst)
  void merge(list& lst, less cmp) //比较器
      
  sort -排序
  sort(void)
  sort(less cmp)
```

> 堆栈

```cpp
push  ->push_back
pop   ->pop_back
top	  ->back
size	->size
empty	->empty
三种线性容器中的任何一种都可以适配堆栈
缺省底层容器是deque
stack<元素类型， 底层容器类型> st;
stack<元素类型> st; 底层容器是deque (后进先出)
```

