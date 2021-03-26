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
  4. 补充
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
	typename T::B x;     //前面必须加typename，
	typename T::BOOL y;
}
int main() {
	A a;
	foo(a) 				//隐式推断T为A类型
}
  ```