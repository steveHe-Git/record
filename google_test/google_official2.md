# gMock Cookbook
# MATCH
## 使用简单匹配器

```cpp
You can use matchers to match arguments that have a certain property:

using ::testing::NotNull;
using ::testing::Return;
...
  EXPECT_CALL(foo, DoThis(Ge(5)))  // The argument must be >= 5.
      .WillOnce(Return('a'));
  EXPECT_CALL(foo, DoThat("Hello", NotNull()));
      // The second argument must not be NULL.

A frequently used matcher is _, which matches anything:
  EXPECT_CALL(foo, DoThat(_, NotNull()));
```

##  Combining Matchers

```cpp
你可以使用已有的AllOf()，AnyOf()，Not()，组合产生一些复杂的Matchers：
using ::testing::AllOf;
using ::testing::Gt;
using ::testing::HasSubstr;
using ::testing::Ne;
using ::testing::Not;
...
  // The argument must be > 5 and != 10.
  EXPECT_CALL(foo, DoThis(AllOf(Gt(5),
                                Ne(10))));

  // The first argument must not contain sub-string "blah".
  EXPECT_CALL(foo, DoThat(Not(HasSubstr("blah")),
                          NULL));

//匹配器是函数对象，参数化匹配器可以像任何其他函数一样组成。 然而，因为它们的类型可能很长并且很少提供有意义的信息，所以使用 C++14 泛型 lambda 来表达它们以避免指定类型会更容易。 例如，
using ::testing::Contains;
using ::testing::Property;

inline constexpr auto HasFoo = [](const auto& f) {
  return Property(&MyClass::foo, Contains(f));
};
...
  EXPECT_THAT(x, HasFoo("blah"));

```

## Casting Matchers

```cpp
Google Matchers都是静态类型的，即如果你错误地使用Matcher的类型(比如，如果你使用Eq(5)去匹配一个string参数 )，编译器会报错。这也是为你好。
    但有时，你知道你自己在做什么，并希望编译器放你一马。举例来说：如果你有一个针对long类型的Matcher，但你想匹配的是int。虽然这两个类型不同，但实际上用Matcher<long>去匹配int并没有错，毕竟，我们可以先将int参数转换成long，再传给Matcher。
    为了支持这种需求，Google Mock提供了SafeMatcherCast<T>(m)函数。它将一个Matcher m转换成Matcher<T>。为了保证它是安全的转换，Google Mock如检查(令U为m接受的参数 )：
    1. T类型可以隐式地转换为U类型。
	2. 当T和U都是内置数值类型时( bool，integers，float )，从T到U的转换是无损的(换句话说，用T类型的任何值都可以用U类型表示 )。
	3. 当U是一个引用，T必须也是一个引用(因为底层的Matcher也许会对U类型参数的地址感兴趣 )。
	如果上述条件中任何一个没满足，是不会通过编译的。
    
using ::testing::SafeMatcherCast;
// A base class and a child class.
class Base { ... };
class Derived : public Base { ... };

class MockFoo : public Foo {
 public:
  MOCK_METHOD(void, DoThis, (Derived* derived), (override));
};

...
  MockFoo foo;
  // m is a Matcher<Base*> we got from somewhere.
  EXPECT_CALL(foo, DoThis(SafeMatcherCast<Derived*>(m)));
	
	如果你发现SafeMatcherCast<T>(m)太严格，你可以用一个类似的函数MatcherCast<T>(m)。两个函数的区别是如果static_cast可以将T类型转换成类型U，那么MatcherCast就可以转换。
	MatcherCast对你凌驾于C++类型系统之上很重要的( static_cast不总是安全的，比如它可以丢弃一部分信息 )，所以要小心不要误用/滥用它。
```

## Selecting Between Overloaded Functions

```cpp
 如果你期望一个重载函数被调用，编译器需要你来指明你指的是重载函数中的哪一个。
   消除一个对象上关于常量的重载的歧义，使用Const()来指明:
using ::testing::ReturnRef;
class MockFoo : public Foo {
  ...
  MOCK_METHOD(Bar&, GetBar, (), (override));
  MOCK_METHOD(const Bar&, GetBar, (), (const, override));
};

...
  MockFoo foo;
  Bar bar1, bar2;
  EXPECT_CALL(foo, GetBar())         // The non-const GetBar().
      .WillOnce(ReturnRef(bar1));
  EXPECT_CALL(Const(foo), GetBar())  // The const GetBar().
      .WillOnce(ReturnRef(bar2));

( Const()由Google Mock定义，并返回它的参数的const引用。)
   消除函数个数相同，但参数类型不同重载函数的歧义，你也许需要精确指定一个Matcher的匹配类型，在Matcher<type>中修饰你的Matcher，或是使用一个类型是确定的Matcher( TypedEq<type>，An<type>()等等):
using ::testing::An;
using ::testing::Matcher;
using ::testing::TypedEq;

class MockPrinter : public Printer {
 public:
  MOCK_METHOD(void, Print, (int n), (override));
  MOCK_METHOD(void, Print, (char c), (override));
};

TEST(PrinterTest, Print) {
  MockPrinter printer;

  EXPECT_CALL(printer, Print(An<int>()));            // void Print(int);
  EXPECT_CALL(printer, Print(Matcher<int>(Lt(5))));  // void Print(int);
  EXPECT_CALL(printer, Print(TypedEq<char>('a')));   // void Print(char);

  printer.Print(3);
  printer.Print(6);
  printer.Print('a');
}
```

## Performing Different Actions Based on the Arguments

```cpp
当一个Mock函数被调用时，最后一个有效的期望会被匹配( “新规则覆盖老规则” )。所以你可以让一个函数根据它的参数值去做不同的事，如下：
using ::testing::_;
using ::testing::Lt;
using ::testing::Return;
...
  // The default case.
  EXPECT_CALL(foo, DoThis(_))
      .WillRepeatedly(Return('b'));
  // The more specific case.
  EXPECT_CALL(foo, DoThis(Lt(5)))
      .WillRepeatedly(Return('a'));
现在，如果foo.DoThis()被调用时参数值小于5，就会返回’a’，否则返回’b’。
```

##  Matching Multiple Arguments as a Whole

```cpp
有时候只能单独去匹配参数是不够的。比如，我们想设置第一个参数的值必须小于第二个参数的值。With子句可以让我们将Mock函数的参数做为一个整体去匹配。比如：
using ::testing::_;
using ::testing::Ne;
using ::testing::Lt;
...
  EXPECT_CALL(foo, InRange(Ne(0), _))
      .With(Lt());

上面代码意为InRange第一个参数必须非0，并且必须小于第二个参数。with中的意思是InRange中的两个参数 分别是 x1 < x2;
在With内的语句必须是一个Match<tr1::tuple<A1, ..., An> >类型的Matcher，其中A1,..., An是函数参数的类型。
   你还可以用AllArgs(m)来代替将m写在.With()里的写法。两种形式意义相同，但是.With(AllArgs(Lt()))比.With(Lt())更具有可读性。
   你可以用Args<k1, ..., kn>(m)根据m规则来匹配n个选择的参数。比如：
using ::testing::_;
using ::testing::AllOf;
using ::testing::Args;
using ::testing::Lt;
...
  EXPECT_CALL(foo, Blah)
      .With(AllOf(Args<0, 1>(Lt()), Args<1, 2>(Lt())));
Blah被调用的时候 里面的参数 应该是 x < y	 < z
```

## Using Matchers as Predicates

 ```cpp
	你是否注意到Matcher只是一个好看一些的Pridicate，许多已有的算法可将Predicates作为参数(比如，那些在STL的<algorithm>中定义的算法)，如果Google Mock Matchers不能参与到其中，那将是一个遗憾。
    幸运的地，你可以将一元Predicate仿函数放到Matches()函数中来使用Matcher，比如:
#include <algorithm>
#include <vector>

using ::testing::Matches;
using ::testing::Ge;

vector<int> v;
...
// How many elements in v are >= 10?
const int count = count_if(v.begin(), v.end(), Matches(Ge(10)));

你可通过将简单的matchers组合来产生复杂的matchers，那么这就给你了一种构造组合Predicates的方便方法(与在STL中使用<functional>中的函数一样 )。比如，下面是一个任何满足 >=0，<=100，!=50的Predicate。
Matches(AllOf(Ge(0), Le(100), Ne(50)))
 ```

## Using Matchers in Google Test Assertions

```cpp
  因为Matchers基本上就是Predicates，所以这就提供了一种在Google Test中使用它们的好方法。它叫ASSERT_THAT和EXPECT_THAT：
 ASSERT_THAT(value, matcher);  // Asserts that value matches matcher.
 EXPECT_THAT(value, matcher);  // The non-fatal version.  
 #include"gmock/gmock.h"
using ::testing::AllOf;
using ::testing::Ge;
using ::testing::Le;
using ::testing::MatchesRegex;
using ::testing::StartsWith;

...
 EXPECT_THAT(Foo(), StartsWith("Hello"));
 EXPECT_THAT(Bar(), MatchesRegex("Line \\d+"));
 ASSERT_THAT(Baz(), AllOf(Ge(5), Le(10)));

上面的代码(正如你所猜测的 )执行Foo()，Bar()，和Baz()，并验证：
1 Foo()返回一个以”Hello”开头的字符串。
2 Bar()返回一个匹配”Line\\d+”的正则表达式。
3 Baz()返回一个在[5, 10]区间内的数字。
```

## Using Predicates as Matchers

```cpp
Google Mock提供了一系列的内置Matchers。如果发现它们还是不够，你可以用一个任意的一元Predicate函数或是仿函数作为一个Matcher，只要它能接受你想用的类型。你就可以将这个Predicate入到Truly()函数中，比如:
using ::testing::Truly;
int IsEven(int n) { return (n % 2) == 0 ? 1 : 0; }
...
  // Bar() must be called with an even number.
  EXPECT_CALL(foo, Bar(Truly(IsEven)));
注意Predicate函数/仿函数不需要一定返回bool类型。它只要求返回值可以用于if (condition)语句中的condition。
```

## Matching Arguments that Are Not Copyable

```cpp
	当你设置一个EXPECT_CALL(mock_obj, Foo(bar))时，Google Mock会保存bar的一个拷贝。当Foo()被调用时之后时，Google Mock会比较传递给Foo的参数和所保存的bar的拷贝。通过这种方式，你不需要担心在EXPECT_CALL()执行之后bar被修改了或是被销毁了。当你使用如Eq(bar)，Le(bar)等等Matcher时也是这样。

   	但如果bar对象不能拷贝(比如，没有拷贝构造函数 )？你可以定义自己的Matcher并将它放到Truly()中，前几小节已经介绍过如何去做了。或是如果你自己可以保证bar不会在调用EXPECT_CALL之后改变，这样你可以轻松点。只需要告诉Google Mock它应该保存bar的引用，而不是去拷贝它。下面是一个例子：
using ::testing::Eq;
using ::testing::Lt;
...
  // Expects that Foo()'s argument == bar.
  EXPECT_CALL(mock_obj, Foo(Eq(std::ref(bar))));

  // Expects that Foo()'s argument < bar.
  EXPECT_CALL(mock_obj, Foo(Lt(std::ref(bar))));

 //切记：如果你这样做，不要在调用EXPECT_CALL之后改变bar对象，否则结果是未定义的。
```

## Validating a Member of an Object

```cpp
	通常Mock函数将一个对象的引用作为参数。当匹配这个参数时，你可能不想将整个对象与一个固定的对象比较，因为这样过于精确了。相反，你可能想验证几个特定的对象成员或是几个特定的getter函数的结果。你可以用Field()和Property来实现这个功能。具体地讲：
     Field(&Foo::bar, m)
   这是一个匹配Foo对象的bar成员值满足Machter m的一个Matcher。
        
     Property(&Foo::baz, m)
   这是一个匹配Foo对象的baz()函数返回的值满足Matcher m的一个Matcher。
        
Expression 	                                    Description
Field(&Foo::number, Ge(3)) 	                    Matches x where x.number >= 3.
Property(&Foo::name, StartsWith("John ")) 	    Matches x where x.name() starts with "John ".
//分别表示：
匹配x.number >=3的x对象。
匹配x.name()以”John ”开头的x对象。

   注意，在Property(&Foo::baz, ...)中，函数baz()必须是无参的，而且需要声明为const。
   随便提一下，Field()和Property()同样可以匹配指向对象的普通指针，比如：
Field(&Foo::number, Ge(3))
   它的意思是匹配一个p->number>=3的普通指针p，如果p是NULL，匹配总是会失败。
   如果你想一次验证多个成员呢？切记你可以使用AllOf()。
        
//最后 Field() 和 Property() 提供重载，将字段或属性名称作为第一个参数以将其包含在错误消息中。 这在创建组合匹配器时很有用。
using ::testing::AllOf;
using ::testing::Field;
using ::testing::Matcher;
using ::testing::SafeMatcherCast;

Matcher<Foo> IsFoo(const Foo& foo) {						
  return AllOf(Field("some_field", &Foo::some_field, foo.some_field),
               Field("other_field", &Foo::other_field, foo.other_field),
               Field("last_field", &Foo::last_field, foo.last_field));
}
```

## Validating the Value of Pointed to by a Pointer Argument

```cpp
C++函数经常使用指针型参数。你可以用如NULL，NotNULL()以及其它的一些比较Matcher去匹配一个指针，但如果你想通过指针所指向的值去匹配呢？你可以用Pointee(m) Matcher。
    Pointee(m)当且仅当指针指向的值匹配m时才匹配一个指针。比如：
using ::testing::Ge;
using ::testing::Pointee;
...
  EXPECT_CALL(foo, Bar(Pointee(Ge(3))));
上面的代码会在传入Bar函数的指针参数所指向的值大于3才匹配。
    Pointee()的一个亮点是它将NULL指针视为匹配失败，所以你可以写Pointee(m)而不用写：
      AllOf(NotNull(), Pointee(m))
   以这种方式来避免NULL使你的测试崩溃。

   是否我已经告诉你Pointee()还可以使用智能指针( linked_ptr，shared_ptr， scoped_ptr，等等 )？
   如果一个指向指针的指针呢？你可以嵌套使用Pointee来匹配值。比如，Pointee(Pointee(Lt(3)))匹配一个指向一个指向一个大于3的指针的指针(好绕呀 )。
```

##  Testing a Certain Property of an Object

```cpp
	
有时你想指定一个对象参数有某种属性，但又没有现有的Matcher来指定。如果你想要一个好的错误信息，你需要定义一个Matcher。如果你想简单粗暴地解决，你可以用写一个普通函数来解决这个问题。

   例如你有一个接受Foo类型对象的Mock函数，Foo有一个int bar()函数一个int baz()函数，并且你想限定参数对象的bar()的值加上baz()的值等于某个值。你可以像下面这样做：
using ::testing::Matcher;
class BarPlusBazEqMatcher {
 public:
  explicit BarPlusBazEqMatcher(int expected_sum)
      : expected_sum_(expected_sum) {}

  bool MatchAndExplain(const Foo& foo,
                       std::ostream* /* listener */) const {
    return (foo.bar() + foo.baz()) == expected_sum_;
  }

  void DescribeTo(std::ostream& os) const {
    os << "bar() + baz() equals " << expected_sum_;
  }

  void DescribeNegationTo(std::ostream& os) const {
    os << "bar() + baz() does not equal " << expected_sum_;
  }
 private:
  const int expected_sum_;
};

Matcher<const Foo&> BarPlusBazEq(int expected_sum) {
  return BarPlusBazEqMatcher(expected_sum);
}

...
  EXPECT_CALL(..., DoThis(BarPlusBazEq(5)))...;
    
```

## Matching Containers

```cpp
	有时候Mock函数中的参数是STL容器(比如：list，vector，map，... )，你可能想去匹配参数，因为大多STL容器支持==操作符，所以你可以写Eq(expected_container)，或是直接就写expected_container去匹配一个容器。

   可能有时你想更灵活一些(比如，你想第一个元素精确匹配，第二个元素是一个正数，等等 )。同时，用于测试的容器通常都只有很少一些元素，再说定义一个期望的容器也有些麻烦。
        你可以在下面的情况中用ElementsAre() Matcher：
using ::testing::_;
using ::testing::ElementsAre;
using ::testing::Gt;
...
  MOCK_METHOD(void, Foo, (const vector<int>& numbers), (override));
...
  EXPECT_CALL(mock, Foo(ElementsAre(1, Gt(0), _, 5)));
面Matcher是指container必须有4个元素，分别是1，大于0，任意值，和5。
    
using ::testing::_;
using ::testing::Gt;
using ::testing::UnorderedElementsAre;
...
  MOCK_METHOD(void, Foo, (const vector<int>& numbers), (override));
...
  EXPECT_CALL(mock, Foo(UnorderedElementsAre(1, Gt(0), _, 5)));
这意味着容器必须有 4 个元素，其中（在某种排列下）必须分别为 1、大于 0、任何值和 5。
 
    
//作为替代方案，您可以将参数放在 C 样式数组中，并使用 ElementsAreArray() 或 UnorderedElementsAreArray() 代替： 
using ::testing::ElementsAreArray;
...
  // ElementsAreArray accepts an array of element values.
  const int expected_vector1[] = {1, 5, 2, 4, ...};
  EXPECT_CALL(mock, Foo(ElementsAreArray(expected_vector1)));

  // Or, an array of element matchers.
  Matcher<int> expected_vector2[] = {1, Gt(2), _, 3, ...};
  EXPECT_CALL(mock, Foo(ElementsAreArray(expected_vector2)));

//如果是数组需要动态创建的情况(所以数组的大小不可能在编译时知道 )，你可以给ElementsAreArray()一个附加的参数指定数组的大小：
using ::testing::ElementsAreArray;
...
  int* const expected_vector3 = new int[count];
  ... fill expected_vector3 with values ...
  EXPECT_CALL(mock, Foo(ElementsAreArray(expected_vector3, count)));

//使用pair但比较map或者其他关联性容器时
using testing::ElementsAre;
using testing::Pair;
...
  std::map<string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
  EXPECT_THAT(m, ElementsAre(Pair("a", 1), Pair("b", 2), Pair("c", 3)));

技巧：
1. ElementsAre*()可以用于任意实现了STL iterator概念的容器(比如它有一个const_iterator并支持begin()和end() )并且支持size()，它不仅支持STL中的容器，也支持任何满意上述两个条件的任何容器。
2. 你可以用嵌套的ElementAre*()去匹配嵌套的(多维 )容器。
3. 如果容器是通过指针而不是引用传递的，你只需要写Pointee(ElementAre*(...))。
4. 顺序对于ElementsAre*()是有影响的。所以不要将它用于顺序是不确定的容器(比如，hash_map )。
```

## Sharing Matchers

```cpp
	本质上，一个Google Mock Matcher对象包含一个指向引用计数的对象。拷贝Matchers是允许的并且很高效，因为只是指针被拷贝了。当最后一个引用实现对象的Matcher生命结束时，实现对象也会被释放。
   所以，如果你有一些复杂的Matcher，你想重复使用，是不需要每次都创建一个的。只需要将它赋值给另一个Matcher变量，并使用那个变量！比如：

  Matcher<int> in_range = AllOf(Gt(5), Le(10));
... use in_range as a matcher in multiple EXPECT_CALLs ...
```

## Setting Expectations

```cpp
//ON_CALL EXPECT_CALL 区别
基本上有两种用于定义模拟对象行为的构造：ON_CALL 和 EXPECT_CALL。区别？ ON_CALL 定义了调用模拟方法时会发生什么，但并不暗示对被调用方法的任何期望。 EXPECT_CALL 不仅定义了行为，而且还设置了一个期望，即该方法将使用给定的参数调用给定的次数（并且在您指定顺序时也按照给定的顺序）。
    既然 EXPECT_CALL 做的更多，那不是比 ON_CALL 更好吗？并不真地。每个 EXPECT_CALL 都会对被测代码的行为添加一个约束。拥有比必要更多的约束是比没有足够的约束更糟糕。
	这可能是违反直觉的。验证更多的测试如何比验证更少的测试更糟糕；
    请记住，一个人不必在一项测试中验证多个属性。事实上，在一个测试中只验证一件事是一种很好的风格。如果你这样做，一个 bug 可能只会破坏一两个测试而不是几十个（你更愿意调试哪种情况？）。如果您也习惯于为测试提供描述性名称来说明它们验证的内容，那么您通常可以仅从测试日志本身轻松猜测出问题所在。
    所以默认情况下使用 ON_CALL，并且仅在您真正打算验证调用是否进行时才使用 EXPECT_CALL。例如，您的测试装置中可能有一堆 ON_CALL 来设置同一组中所有测试共享的通用模拟行为，并在不同的 TEST_F 中编写（几乎）不同的 EXPECT_CALL 以验证代码行为的不同方面。与每个 TEST 都有许多 EXPECT_CALL 的样式相比，这会导致测试对实现更改更具弹性（因此不太可能需要维护）并使测试的意图更加明显（因此当您这样做时它们更容易维护需要维护它们）；

```

## Ignoring Uninteresting Calls

```cpp
	如果你对一个Mock函数如何被调用不感兴趣，你可以不对它指定任何事。如果你这样做了，当这个函数被调用时，Google Mock会使用它的默认行为使测试可以得以进行下去。如果你不太喜欢这个默认的行为，你可以用DefaultValue<T>::Set()(在这个文档的后面会讲到 )或是ON_CALL去覆盖默认行为。

   	请注意一旦你对某个Mock函数表现出了兴趣(通过EXPECT_CALL() )，对它的调用都需要匹配某个期望。如果这个函数被调用了，但参数没有匹配任何一个EXPECT_CALL语句，它将会产生一个错误。
```

## Disallowing Unexpected Calls

```cpp
如果一个Mock函数根本不应该被调用，可以明确地指出：
using ::testing::_;
...
  EXPECT_CALL(foo, Bar(_))
      .Times(0);

//如果对一个函数的某些调用是允许的，其它的调用则不行，则可以列出所有期望的调用：
using ::testing::AnyNumber;
using ::testing::Gt;
...
  EXPECT_CALL(foo, Bar(5));
  EXPECT_CALL(foo, Bar(Gt(10)))
      .Times(AnyNumber());
  如果一个调用不匹配任一EXPECT_CALL()，刚它会产生一个错误。    
```
## Understanding Uninteresting vs Unexpected Calls
```cpp
Uninteresting calls and unexpected calls 在 gMock 中是不同的概念。非常不一样。
    没有兴趣的调用和非期望的调用：
    
    1. 如果甚至没有一个 EXPECT_CALL(x, Y(...)) 集合，则调用 x.Y(...) 没有兴趣的调用；（会警告）
    2. 如果设置了一些 EXPECT_CALL(x, Y(...)) ，则调用 x.Y(...) 是意外的，但没有一个与调用匹配； （会报错）；
    
    //NiceMock and StrictMock 
    使用NiceMock可以使警告消息；
    使用StrictMock可以使警告升级成错误；

//DoThis之外的方法被调用，不会产生警告
using ::testing::NiceMock;
TEST(...) {
  NiceMock<MockFoo> mock_foo;
  EXPECT_CALL(mock_foo, DoThis());
  ... code that uses mock_foo ...
}

//DoThis之外的方法被调用，将会出现错误
using ::testing::NiceMock;
TEST(...) {
  NiceMock<MockFoo> mock_foo(5, "hi");  // Calls MockFoo(5, "hi").
  EXPECT_CALL(mock_foo, DoThis());
  ... code that uses mock_foo ...
}
```

## Expecting Ordered Calls

```cpp
	尽管在Google Mock尝试匹配一个设置了期望的函数时，会优先匹配先定义的EXPECT_CALL语句，但默认的调用并不是必须以EXPECT_CALL()所写的顺序进行匹配。比如，如果参数匹配了第三个EXPECT_CALL，但没有匹配前两个EXPECT_CALL，那么就会使用第三个期望。
    如果你更好所有调用都以期望的顺序进行，将EXPECT_CALL()语句放到一个InSequence对象的生命周期中：
using ::testing::_;
using ::testing::InSequence;
  {
    InSequence s;

    EXPECT_CALL(foo, DoThis(5));
    EXPECT_CALL(bar, DoThat(_))
        .Times(2);
    EXPECT_CALL(foo, DoThis(6));
  }
在这个例子中，我们期望调用以顺序如下：先调用foo.DoThis(5)，然后两次参数为任意的bar.DoThat()调用，最后调用一次foo.DoThis()。如果调用的顺序与上面不符，则Google Mock会报告一个错误。
```

##  Expecting Partially Ordered Calls

```cpp
	有时要求所有的调用都以一个预定义的顺序会导致测试脆弱。比如，也许我们会关心A在B和C之前调用，但不关心B和C的调用顺序先后。在这种情况下，测试应当反应我们真正的意图，而不是写一个约束过强的语句。
    Google Mock在调用上设置一个顺序的任意DAG( directed acyclic graph有向无环图 )。表达有DAG的一种方式是用EXPECT_CALL的After子句。
    另一种方法是通过InSequence()子句(不是InSequence类 )，这是我们从jMock 2中借鉴而来的。它比之After()稍失灵活，但当你有一长串顺序调用之时会更方便，因为它不要求你对长串中的期望都起一个不同的名字，下面是它如何工作的：

   如果我们视EXPECT_CALL()语句为图中的结点，添加一条从结点A到结点B的边，意思是A必须先于B出现，我们可以得到一个DAG。如果DAG被分解成了单独的顺序，我们只需要知道每个EXPECT_CALL在顺序中的位置，我们就可以重构出原来的DAG。

   因此要为指定在期望之上的部分有序我们需要做两件事：第一定义一些Sequence对象，然后指明Sequence在DAG中的部分。在同一顺序(sequence)中的期望必须以定义的先后(order)出现。比如：
using ::testing::Sequence;
...
  Sequence s1, s2;

  EXPECT_CALL(foo, A())
      .InSequence(s1, s2);
  EXPECT_CALL(bar, B())
      .InSequence(s1);
  EXPECT_CALL(bar, C())
      .InSequence(s2);
  EXPECT_CALL(foo, D())
      .InSequence(s2);
specifies the following DAG (where s1 is A -> B, and s2 is A -> C -> D): （ABCD）
```

## Controlling When an Expectation Retires

```cpp
当一个Mock函数被调用时，Google Mock只考虑那些仍然有效的期望。一个期望在创建之时是有效的，当在它之上发生一次调用后，它就变为失效的了。比如，在：
using ::testing::_;
using ::testing::Sequence;
...
  Sequence s1, s2;

  EXPECT_CALL(log, Log(WARNING, _, "File too large."))      // #1
      .Times(AnyNumber())
      .InSequence(s1, s2);
  EXPECT_CALL(log, Log(WARNING, _, "Data set is empty."))   // #2
      .InSequence(s1);
  EXPECT_CALL(log, Log(WARNING, _, "User not found."))      // #3
      .InSequence(s2);
从后往前匹配，只要#2或#3任一匹配，#1就会失效。(被覆盖了)，如果一个警告”File too large”在此之后调用，它将会产生一个错误。
    
//注意一个期望在它饱和后不会自动失效。例如：
using ::testing::_;
...
  EXPECT_CALL(log, Log(WARNING, _, _));                     // #1
  EXPECT_CALL(log, Log(WARNING, _, "File too large."));     // #2
上面的代码意思是有仅只能有一个”File too large”的警告。如果第二个警告仍然是”File too large”，#2仍然会匹配并且产生一个超出上界的错误。
    
//如果这不是你想要的，你可以让一个期望在饱和之后就失效：
using ::testing::_;
...
  EXPECT_CALL(log, Log(WARNING, _, _));                     // #1
  EXPECT_CALL(log, Log(WARNING, _, "File too large."))      // #2
      .RetiresOnSaturation();

```

# Actions
## Returning References from Mock Methods
```cpp
如果一个Mock函数的返回类型是引用，你需要用ReturnRef()而不是Return()
using ::testing::ReturnRef;

class MockFoo : public Foo {
 public:
  MOCK_METHOD(Bar&, GetBar, (), (override));
};
...
  MockFoo foo;
  Bar bar;
  EXPECT_CALL(foo, GetBar())
      .WillOnce(ReturnRef(bar));
...

```

## Returning Live Values from Mock Methods

```cpp
	Return(x)这个动作在创建时就会保存一个x的拷贝，在它执行时总是返回相同的值。但有时你可能不想每次返回x的拷贝。

   如果Mock函数的返回类型是引用，你可以用ReturnRef(x)来每次返回不同的值。但是Google Mock不允许在Mock函数返回值不是引用的情况下用ReturnRef()返回，这样做的后果通常是提示一个错误，所以，你应该怎么做呢？
   你可以尝试ByRef()：
using testing::Return;
class MockFoo : public Foo {
 public:
  MOCK_METHOD(int, GetValue, (), (override));
};
...
  int x = 0;
  MockFoo foo;
  EXPECT_CALL(foo, GetValue())
      .WillRepeatedly(Return(std::ref(x)));  // Wrong!
  x = 42;
  EXPECT_EQ(42, foo.GetValue());
//不幸的是，上面的代码不能正常工作，它会提示以下错误：
 Value of: foo.GetValue()
 Actual: 0
 Expected: 42

   不能正常工作的原因是在Return(value)这个动作创建时将x转换成Mock函数的返回类型，而不是它执行时再进行转换(这个特性是为保证当值是代理对象引用一些临时对象时的安全性 )。结果是当期望设置时ByRef(x)被转换成一个int值(而不是一个const int& )，且Return(ByRef(x)会返回0。
                                                                                                                                                     
//ReturnPointee(pointer)就是专门用来解决这个问题的。 它返回执行动作时指针指向的值：                                                          using testing::ReturnPointee;
...
  int x = 0;
  MockFoo foo;
  EXPECT_CALL(foo, GetValue())
      .WillRepeatedly(ReturnPointee(&x));  // Note the & here.
  x = 42;
  EXPECT_EQ(42, foo.GetValue());  // This will succeed now.                                                                                                      
```

## Combining Actions

```cpp
 你想当一个函数被调用时做更多的事吗？这个需求是合理的。DoAll()允许你每次执行一系列动作。只有最后一个动作的返回值会被使用。
using ::testing::_;
using ::testing::DoAll;

class MockFoo : public Foo {
 public:
  MOCK_METHOD(bool, Bar, (int n), (override));
};
...
  EXPECT_CALL(foo, Bar(_))
      .WillOnce(DoAll(action_1,
                      action_2,
                      ...
                      action_n));

```

## Verifying Complex Arguments

```cpp
如果您想验证是否使用特定参数调用了某个方法，但匹配条件很复杂，则可能难以区分基数失败（调用方法的次数错误）和参数匹配失败。 同样，如果您匹配多个参数，则可能不容易区分哪个参数匹配失败。 例如：
   // Not ideal: this could fail because of a problem with arg1 or arg2, or maybe
  // just the method wasn't called.
  EXPECT_CALL(foo, SendValues(_, ElementsAre(1, 4, 4, 7), EqualsProto( ... )));

您可以改为保存参数并单独测试它们：
  EXPECT_CALL(foo, SendValues)
      .WillOnce(DoAll(SaveArg<1>(&actual_array), SaveArg<2>(&actual_proto)));
  ... run the test
  EXPECT_THAT(actual_array, ElementsAre(1, 4, 4, 7));
  EXPECT_THAT(actual_proto, EqualsProto( ... ));
```

## Mock Side Effects

```cpp
有时一个函数的作用不是通过返回值来体现，而是通过副作用。比如，你可以改变一些全局状态或是修改一个输入参数的值。要Mock副作用，通常你可以通过实现::testing::AtionInterface定义你自己的动作。

   如果你要做的仅仅是改变一个输入参数，内置的SetArgPointee()动作是很方便的：
using ::testing::_;
using ::testing::SetArgPointee;

class MockMutator : public Mutator {
 public:
  MOCK_METHOD(void, Mutate, (bool mutate, int* value), (override));
  ...
}
...
  MockMutator mutator;
  EXPECT_CALL(mutator, Mutate(true, _))
      .WillOnce(SetArgPointee<1>(5));

在这个例子中，当mutator.Mutate()被调用时，我们将赋给由第二个参数指针指向的值为5。
   SetArgPointee()将传入的值进行了一次拷贝，所以你不需要保证这个值的生命周期。但这也意味着这个对象必须有一个拷贝构造函数和赋值操作符。
//如果Mock函数还需要返回一个值，你可以将SetArgPointee()和Return()放到DoAll()中：
using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;

class MockMutator : public Mutator {
 public:
  ...
  MOCK_METHOD(bool, MutateInt, (int* value), (override));
}
...
  MockMutator mutator;
  EXPECT_CALL(mutator, MutateInt(_))
      .WillOnce(DoAll(SetArgPointee<0>(5),
                      Return(true)))
      
//如果输出参数是一个数组，用SetArrayArgument<N>[first, last)动作。它将源范围[first, last)中的元素拷贝到一个新的以0开始的新数组中：
using ::testing::NotNull;
using ::testing::SetArrayArgument;

class MockArrayMutator : public ArrayMutator {
 public:
  MOCK_METHOD(void, Mutate, (int* values, int num_values), (override));
  ...
}
...
  MockArrayMutator mutator;
  int values[5] = {1, 2, 3, 4, 5};
  EXPECT_CALL(mutator, Mutate(NotNull(), 5))
      .WillOnce(SetArrayArgument<0>(values, values + 5));


//当参数是一个输出迭代器时也是可以工作的：
using ::testing::_;
using ::testing::SetArrayArgument;

class MockRolodex : public Rolodex {
 public:
  MOCK_METHOD(void, GetNames, (std::back_insert_iterator<vector<string>>),
              (override));
  ...
}
...
  MockRolodex rolodex;
  vector<string> names;
  names.push_back("George");
  names.push_back("John");
  names.push_back("Thomas");
  EXPECT_CALL(rolodex, GetNames(_))
      .WillOnce(SetArrayArgument<0>(names.begin(), names.end()));
...
```

## Changing a Mock Object’s Behavior Based on the State

```cpp
如果你期望一个调用改变mock对象的行为，你可以用::testing::InSequence来指定在这个调用之前和之后的对象行为：
using ::testing::InSequence;
using ::testing::Return;

...
  {
     InSequence seq;
     EXPECT_CALL(my_mock, IsDirty())
         .WillRepeatedly(Return(true));
     EXPECT_CALL(my_mock, Flush());
     EXPECT_CALL(my_mock, IsDirty())
         .WillRepeatedly(Return(false));
  }
  my_mock.FlushIfDirty();
 这可以让my_mock.IsDirty()在my_mock.Flush()调用之前返回true，而在之后返回false。
     
//如果要改变的对象动作更复杂，你可以保存保存这些效果到一个变量中，并使一个Mock函数从这个变量中得到它的返回值：
using ::testing::_;
using ::testing::SaveArg;
using ::testing::Return;

ACTION_P(ReturnPointee, p) { return *p; }
...
  int previous_value = 0;
  EXPECT_CALL(my_mock, GetPrevValue)
      .WillRepeatedly(ReturnPointee(&previous_value));
  EXPECT_CALL(my_mock, UpdateValue)
      .WillRepeatedly(SaveArg<0>(&previous_value));
  my_mock.DoSomethingToUpdateValue();
这样，m_mock.GetPrevValue()总是会返回上一次UpdateValue调用的参数值。
```

##  Setting the Default Value for a Return Type

```cpp
如果一个Mock函数返回类型是一个内置的C++类型或是指针，当它调用时默认会返回0。如果默认值不适合你，你只需要指定一个动作。
有时，你也许想改变默认值，或者你想指定一个Google Mock不知道的类型的默认值。你可以用::testing::DefaultValue类模板：

using ::testing::DefaultValue;
class MockFoo : public Foo {
 public:
  MOCK_METHOD(Bar, CalculateBar, (), (override));
};


...
  Bar default_bar;
  // Sets the default return value for type Bar.
  DefaultValue<Bar>::Set(default_bar);

  MockFoo foo;

  // We don't need to specify an action here, as the default
  // return value works for us.
  EXPECT_CALL(foo, CalculateBar());

  foo.CalculateBar();  // This should return default_bar.

  // Unsets the default return value.
  DefaultValue<Bar>::Clear();
 
//请注意改变一个类型的默认值会让你的测试难于理解。我们建议你谨慎地使用这个特性。比如，你最好确保你在使用这个特性代码之前之后要加上Set()和Clear()调用。
```

## Setting the Default Actions for a Mock Method

```cpp
	如果你掌握了如何改变一个类型的默认值。但是也许这对于你也许是不够的：也许你有两个Mock函数，它们有相同的返回类型，并且你想它们有不同的行为。ON_CALL()宏允许你在函数级别自定义你的Mock函数行为：
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Gt;
using ::testing::Return;
...
  ON_CALL(foo, Sign(_))
      .WillByDefault(Return(-1));
  ON_CALL(foo, Sign(0))
      .WillByDefault(Return(0));
  ON_CALL(foo, Sign(Gt(0)))
      .WillByDefault(Return(1));

  EXPECT_CALL(foo, Sign(_))
      .Times(AnyNumber());

  foo.Sign(5);   // This should return 1.
  foo.Sign(-9);  // This should return -1.
  foo.Sign(0);   // This should return 0.

	正如你所猜测的，当有多个ON_CALL()语句时，新的语句(即后写的语句 )会优先匹配。换言之，最后一个匹配参数的Mock函数会被调用。这种匹配顺序允许你开始设置比较宽松的行为，然后再指定这个Mock函数更具体的行为。
```

## Using Functions / Methods / Functors as Actions

```cpp
如果内置动作不适合你，你可以轻松地用一个已有的函数，方法，仿函数作为一个动作：
using ::testing::_; using ::testing::Invoke;

class MockFoo : public Foo {
 public:
  MOCK_METHOD(int, Sum, (int x, int y), (override));
  MOCK_METHOD(bool, ComplexJob, (int x), (override));
};

int CalculateSum(int x, int y) { return x + y; }
int Sum3(int x, int y, int z) { return x + y + z; }

class Helper {
 public:
  bool ComplexJob(int x);
};

...
  MockFoo foo;
  Helper helper;
  EXPECT_CALL(foo, Sum(_, _))
      .WillOnce(&CalculateSum)
      .WillRepeatedly(Invoke(NewPermanentCallback(Sum3, 1)));
  EXPECT_CALL(foo, ComplexJob(_))
      .WillOnce(Invoke(&helper, &Helper::ComplexJob))
      .WillOnce([] { return true; })
      .WillRepeatedly([](int x) { return x > 0; });

  foo.Sum(5, 6);         // Invokes CalculateSum(5, 6).
  foo.Sum(2, 3);         // Invokes Sum3(1, 2, 3).
  foo.ComplexJob(10);    // Invokes helper.ComplexJob(10).
  foo.ComplexJob(-1);    // Invokes the inline lambda.

	唯一的要求是这些函数，方法，仿函数的类型必须与Mock函数兼容，即后者的参数必须可以隐式转换成Mock函数中相应的参数，前者的返回值可以隐式转换成Mock函数的返回类型。所以，你可以调用一个与Mock函数定义不完全一致的函数，只要这样做是安全的，精彩吧，huh？
        
        

```

## Invoking an Argument of the Mock Function

```cpp
有时一个Mock函数会接收一个函数指针或是一个仿函数(换言之，一个”callable” )参数，比如：
class MockFoo : public Foo {
 public:
  MOCK_METHOD(bool, DoThis, (int n, (ResultCallback1<bool, int>* callback)),
              (override));
};
你也许想调用这个函数指针参数： 
using ::testing::_;
...
  MockFoo foo;
  EXPECT_CALL(foo, DoThis(_, _))
      .WillOnce(...);
      // Will execute callback->Run(5), where callback is the
      // second argument DoThis() receives.

//你需要引用一个Mock函数的参数，但C++还没有lambda表示式，所以你需要定义你自己的动作
InvokeArgument<N>(arg_1, arg_2, ..., arg_m)
    它会调用Mock函数接收到的第N个参数，并将arg_1, arg2, ..., arg_m作为参数。无论参数是一个函数指针或是一个仿函数，Google Mock都可以处理。
    
使用它，你可以写：
    using ::testing::_;
using ::testing::InvokeArgument;
...
  EXPECT_CALL(foo, DoThis(_, _))
      .WillOnce(InvokeArgument<1>(5));
      // Will execute callback->Run(5), where callback is the
      // second argument DoThis() receives.


//如果一个函数是有一个参数是引用呢？没问题，把它放到ByRef()/ref()中：
  ...
  MOCK_METHOD(bool, Bar,
              ((ResultCallback2<bool, int, const Helper&>* callback)),
              (override));
  ...
  using ::testing::_;
  using ::testing::InvokeArgument;
  ...
  MockFoo foo;
  Helper helper;
  ...
  EXPECT_CALL(foo, Bar(_))
      .WillOnce(InvokeArgument<1>(5, std::ref(helper)));
      // std::ref(helper) guarantees that a reference to helper, not a copy of
      // it, will be passed to the callback.

//如果函数指针接收的是引用参数，但我们没有将参数放到ByRef()/ref()中呢？那么InvokeArgument()会拷贝这个参数，将传递拷贝后的值的引用给函数指针，而不是原来值的引用。这在参数是一个临时变量时特别方便：
  ...
  MOCK_METHOD(bool, DoThat, (bool (*f)(const double& x, const string& s)),
              (override));
  ...
  using ::testing::_;
  using ::testing::InvokeArgument;
  ...
  MockFoo foo;
  ...
  EXPECT_CALL(foo, DoThat(_))
      .WillOnce(InvokeArgument<0>(5.0, string("Hi")));
      // Will execute (*f)(5.0, string("Hi")), where f is the function pointer
      // DoThat() receives.  Note that the values 5.0 and string("Hi") are
      // temporary and dead once the EXPECT_CALL() statement finishes.  Yet
      // it's fine to perform this action later, since a copy of the values
      // are kept inside the InvokeArgument action.
```

##  Ignoring an Action’s Result

```cpp
	有时你有一个返回值的动作，但你需要一个返回void的动作(也许你想在一个返回void的Mock函数中用它，或是它在DoAll()中要用它，但它不是DoAll()中最后一个 )。IgnoreResult()允许你实现这个功能。比如：
 using ::testing::_;
using ::testing::DoAll;
using ::testing::IgnoreResult;
using ::testing::Return;

int Process(const MyData& data);
string DoSomething();

class MockFoo : public Foo {
 public:
  MOCK_METHOD(void, Abc, (const MyData& data), (override));
  MOCK_METHOD(bool, Xyz, (), (override));
};

  ...
  MockFoo foo;
  EXPECT_CALL(foo, Abc(_))
      // .WillOnce(Invoke(Process));
      // The above line won't compile as Process() returns int but Abc() needs
      // to return void.
      .WillOnce(IgnoreResult(Process));
  EXPECT_CALL(foo, Xyz())
      .WillOnce(DoAll(IgnoreResult(DoSomething),
                      // Ignores the string DoSomething() returns.
                      Return(true)));

注意你不能将IgnoreResult()用在一个已经是返回void的动作上。如果你这样做，你会得到一个丑陋的编译错误。
```

## Selecting an Action’s Arguments

 ```cpp
假使你有一个Mock函数Foo()，它接受七个参数，并且你想在Foo调用时使用一个自定义的动作。但问题是，这个自定义的动作只有三个参数：
using ::testing::_;
using ::testing::Invoke;
...
  MOCK_METHOD(bool, Foo,
              (bool visible, const string& name, int x, int y,
               (const map<pair<int, int>>), double& weight, double min_weight,
               double max_wight));
...
bool IsVisibleInQuadrant1(bool visible, int x, int y) {
  return visible && x >= 0 && y >= 0;
}
...
  EXPECT_CALL(mock, Foo)
      .WillOnce(Invoke(IsVisibleInQuadrant1));  // Uh, won't compile. :-(

//为了取悦编译器，你可以定义一个配接器，它有着与Foo()相同的定义，然后用它调用自定义动作：
using ::testing::_;
using ::testing::Invoke;
...
bool MyIsVisibleInQuadrant1(bool visible, const string& name, int x, int y,
                            const map<pair<int, int>, double>& weight,
                            double min_weight, double max_wight) {
  return IsVisibleInQuadrant1(visible, x, y);
}
...
  EXPECT_CALL(mock, Foo)
      .WillOnce(Invoke(MyIsVisibleInQuadrant1));  // Now it works.
但这要写不笨拙吗？


//google Mock提供了一个通用的动作配接器，所以你可以把时间用到更重要的事情上去，而不是写你自己的配接器。下面是它的语法：
WithArgs<N1, N2, ..., Nk>(action)
    它创建一个动作，将Mock函数的参数传给内部的动作，用WithArgs，我们前面的例子可以写为：
using ::testing::_;
using ::testing::Invoke;
using ::testing::WithArgs;
...
  EXPECT_CALL(mock, Foo)
      .WillOnce(WithArgs<0, 2, 3>(Invoke(IsVisibleInQuadrant1)));  // No need to define your own adaptor.


为了更好的可读性，Google Mock提供给你了：
1 WithoutArgs(action)当内部动作不接受参数
2 WithArg<N>(action)(在Arg后没有s )当内部动作接受一个参数。
正如你所认识到的，InvokeWithoutArgs(...)只是WithoutArgs(Invoke(...))的语法简写。

这里有几个小提示：
1 在WithArgs内部的动作并不一定要是Invoke()，它可以是任意的。
2 在参数列表中的参数可以重复的，比如WithArgs<2,3,3,5>(...)。
3 你可以改变参数的顺序，比如WithArgs<3, 2, 1>(...)。
4 所选的参数类型并不一定要完全匹配内部动作的定义。只要它们可以隐式地被转换成内部动作的相应参数就可以了。例如，如果Mock函数的第4个参数是int，而my_action接受一个double参数，WithArg<4>(my_action)可以工作。
 ```

## Ignoring Arguments in Action Functions

```cpp
	Selecting-an-action’s-arguments中介绍了一种使参数不匹配的动作和Mock函数结合使用的方法。但这种方法的缺点是要将动作封装到WithArgs<...>()中，这会使测试者感到麻烦。

	如果你定义要用于Invoke*的一个函数，方法，或是仿函数，并且你对它的一些函数不感兴趣，另一种做法是声明你不感兴趣的参数为Unused。这会使定义更清爽，并在不感兴趣的参数发生变化时更健壮。而且它可以增加一个动作函数被重用的可能性。比如，有：
  public:
  MOCK_METHOD(double, Foo, double(const string& label, double x, double y), (override));
  MOCK_METHOD(double, Bar, (int index, double x, double y), (override));

//你除了可以像下面一样写：
using ::testing::_;
using ::testing::Invoke;

double DistanceToOriginWithLabel(const string& label, double x, double y) {
  return sqrt(x*x + y*y);
}
double DistanceToOriginWithIndex(int index, double x, double y) {
  return sqrt(x*x + y*y);
}
...
  EXPECT_CALL(mock, Foo("abc", _, _))
      .WillOnce(Invoke(DistanceToOriginWithLabel));
  EXPECT_CALL(mock, Bar(5, _, _))
      .WillOnce(Invoke(DistanceToOriginWithIndex));

//你还可以写：
using ::testing::_;
using ::testing::Invoke;
using ::testing::Unused;

double DistanceToOrigin(Unused, double x, double y) {
  return sqrt(x*x + y*y);
}
...
  EXPECT_CALL(mock, Foo("abc", _, _))
      .WillOnce(Invoke(DistanceToOrigin));
  EXPECT_CALL(mock, Bar(5, _, _))
      .WillOnce(Invoke(DistanceToOrigin));

```

## Sharing Actions

```cpp
 	如匹配器一样，Google Mock动作对象中也有一个指针指向引用计数的实现对象。所以拷贝动作是允许的并且也是高效的。当最后一个引用实现对象的动作死亡后，现实对象会被delete。

	如果你有一些想重复使用的复杂动作。你也许不想每次都重新产生一次。如果这个动作没有一个内部状态(比如：它在每次调用都做相同的事)，你可以将它赋值给一个动作变量，以后就可以重复使用这个变量了，比如：
using ::testing::Action;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
...
  Action<bool(int*)> set_flag = DoAll(SetArgPointee<0>(5),
                                      Return(true));
  ... use set_flag in .WillOnce() and .WillRepeatedly() ...

//但是，如果一个动作有自己的状态，那你共享这个动作对象时，你也许会得到一些意外的结果。假设你有一个动作工厂IncrementCounter(init)，它创建一个动作，这个动作中的计数器初始值是init，每次调用增加计数器的值并返回计数器值，使用从相同的语句中产生的两个动作和使用一个共享动作会产生不同的行为。比如:
  EXPECT_CALL(foo, DoThis())
      .WillRepeatedly(IncrementCounter(0));
  EXPECT_CALL(foo, DoThat())
      .WillRepeatedly(IncrementCounter(0));
  foo.DoThis();  // Returns 1.
  foo.DoThis();  // Returns 2.
  foo.DoThat();  // Returns 1 - Blah() uses a different
                 // counter than Bar()'s.

相较：
using ::testing::Action;
...
  Action<int()> increment = IncrementCounter(0);
  EXPECT_CALL(foo, DoThis())
      .WillRepeatedly(increment);
  EXPECT_CALL(foo, DoThat())
      .WillRepeatedly(increment);
  foo.DoThis();  // Returns 1.
  foo.DoThis();  // Returns 2.
  foo.DoThat();  // Returns 3 - the counter is shared.


```

##  Testing Asynchronous Behavior

```cpp
gMock 经常遇到的问题之一是很难测试异步行为。假设您有一个要测试的 EventQueue 类，并且您创建了一个单独的 EventDispatcher 接口，以便您可以轻松地模拟它。但是，该类的实现在后台线程上触发了所有事件，这使得测试时间变得困难。您可以只插入 sleep() 语句并希望最好，但这会使您的测试行为不确定。更好的方法是使用 gMock 操作和 Notification 对象来强制您的异步测试同步运行。
 class MockEventDispatcher : public EventDispatcher {
  MOCK_METHOD(bool, DispatchEvent, (int32), (override));
};

TEST(EventQueueTest, EnqueueEventTest) {
  MockEventDispatcher mock_event_dispatcher;
  EventQueue event_queue(&mock_event_dispatcher);

  const int32 kEventId = 321; //消息
  absl::Notification done;
  EXPECT_CALL(mock_event_dispatcher, DispatchEvent(kEventId))
      .WillOnce([&done] { done.Notify(); });

  event_queue.EnqueueEvent(kEventId);
  done.WaitForNotification();
}
在上面的示例中，我们设置了正常的 gMock 期望值，然后添加了一个额外的操作来通知 Notification 对象。现在我们可以在主线程中调用 Notification::WaitForNotification() 来等待异步调用完成。之后，我们的测试套件就完成了，我们可以安全地退出。

//注意：这个例子有一个缺点：即如果不满足期望，我们的测试将永远运行。它最终会超时并失败，但需要更长的时间并且调试起来会稍微困难一些。为了缓解这个问题，您可以使用 WaitForNotificationWithTimeout(ms) 而不是 WaitForNotification()。 
```

## Making the Compilation Faster

```cpp
 	无论你相信与否，编译一个Mock类的大部分时间都花费在产生它的构造函数和析构函数上了，因为它们要做很多的任务(比如，对期望的验证)。更严重的是，有不同函数声明的Mock函数，它们的构造函数/析构函数需要由编译器分别产生，所以，如果你Mock许多不同类型的函数，编译你的Mock类会非常慢。

	如果你现在发现编译很慢，你可以将Mock类的构造函数/析构函数移出Mock类，将它们放到.cpp文件中。这样做后，即使你在多个文件中#include你的Mock文件，编译器只用产生一次constructor和destructor，这样做编译的更快。
        
// File mock_foo.h.
...
class MockFoo : public Foo {
 public:
  // Since we don't declare the constructor or the destructor,
  // the compiler will generate them in every translation unit
  // where this mock class is used.

  MOCK_METHOD(int, DoThis, (), (override));
  MOCK_METHOD(bool, DoThat, (const char* str), (override));
  ... more mock methods ...
};
替换为：
// File mock_foo.h.
...
class MockFoo : public Foo {
 public:
  // The constructor and destructor are declared, but not defined, here.
  MockFoo();
  virtual ~MockFoo();

  MOCK_METHOD(int, DoThis, (), (override));
  MOCK_METHOD(bool, DoThat, (const char* str), (override));
  ... more mock methods ...
};

// File mock_foo.cc.
#include "path/to/mock_foo.h"

// The definitions may appear trivial, but the functions actually do a
// lot of things through the constructors/destructors of the member
// variables used to implement the mock methods.
MockFoo::MockFoo() {}
MockFoo::~MockFoo() {}

```

## Forcing a Verification

```cpp
	当你的Mock对象销毁的时候，它会自动检查所有的期望是否满足，如果没满足，会产生一个Google Test失败。这种方式让你可以少去操心一件事。但是如果你不确定你的Mock对象是否会被销毁时，你还是要操心了。

	一个Mock对象怎么会最终没有被销毁呢？嗯，它可以是在由被测试的代码在堆上分配的。假设代码中有一个bug，它没能正常地delete Mock对象，你最终可能会在测试中有bug时，让测试通过。

	使用一个堆检查工具是一个好主意，可以减少了一些担心，但它的实现不是100%可靠的。所以有时你想在一个Mock对象(希望如些 )销毁前，强制Google Mock去检查它。你可以写：
        
TEST(MyServerTest, ProcessesRequest) {
  using ::testing::Mock;

  MockFoo* const foo = new MockFoo;
  EXPECT_CALL(*foo, ...)...;
  // ... other expectations ...

  // server now owns foo.
  MyServer server(foo);
  server.ProcessRequest(...);

  // In case that server's destructor will forget to delete foo,
  // this will verify the expectations anyway.
  Mock::VerifyAndClearExpectations(foo);
}  // server is destroyed when it goes out of scope here.

//提示：Mock::VerifyAndClearExpectations()函数返回一个bool值来标明检查是否成功(成功为true)，所以你可以将函数放到ASSERT_TRUE()中，如果这个断言失败，就没有必要再继续了。
```

## Using Check Points

```cpp
	有时你也许也在多个检查点”重置”一个Mock对象：在每个检查点，你可以检查在这个Mock对象上的所有设置的期望是否满足，并且你可以设置在它上面设置一些新的期望，就如同这个Mock对象是新创建的一样。这样做可以让你让你的测试”分段”地使用Mock对象。
    其中一个使用场景是在你的测试的SetUp()函数中，你也许想借助Mock对象，将你测试的对象放到一个特定的状态。如果在一个合适的状态后，你清除所在Mock对象上的所有期望，这样你可以在TEST_F中设置新的期望。
    正如你也许会发现的一样，Mock::VerifyAndClearExpectations()函数会在这帮助你。或是如果你正用ON_CALL()设置在这个Mock对象上的默认动作，并且想清除这个Mock对象上的默认动作，就用Mock::VerifyAndClear(&mock_object)。这个函数会做如Mock::VerifyAndClearExpectations(&mock_object)相同的工作，并返回相同的bool值，但它还会清除设置在mock_object上设置的ON_CALL()语句。
    一个可以达到相同效果的技巧是将期望放到序列(sequence)中，在指定的位置将调用放到无效果的(dummy)检查点函数中。然后你可以检查Mock函数在指定时间的行为了。比如，你有下面的代码：
    Foo(1);
    Foo(2);
    Foo(3);
//你想验证Foo(1)和Foo(3)都调用了mock.Bar(“a”)，但Foo(2)没有调用任何函数，你可以写：
       
using ::testing::MockFunction;
TEST(FooTest, InvokesBarCorrectly) {
  MyMock mock;
  // Class MockFunction<F> has exactly one mock method.  It is named
  // Call() and has type F.
  MockFunction<void(string check_point_name)> check;
  {
    InSequence s;

    EXPECT_CALL(mock, Bar("a"));
    EXPECT_CALL(check, Call("1"));
    EXPECT_CALL(check, Call("2"));
    EXPECT_CALL(mock, Bar("a"));
  }
  Foo(1);
  check.Call("1");
  Foo(2);
  check.Call("2");
  Foo(3);
}
//期望指明了第一个Bar(“a”)必须在检查点”1”之前发生，第二个Bar(“a”)必须在检查点”2”之后发生，并且两个检查点之间不应该发生任何事。这种使用检查点的明确写法很容易指明哪个Foo函数调用的Bar(“a”)。 
```

## Mocking Destructors

```cpp
	有时你想明确一个Mock对象在指定时间销毁，比如，在bar->A()调用之后，但在bar->()调用之前。我们已经知道可以指定Mock函数调用的顺序，所以我们需要做的是Mock所Mock函数的析构函数。

这听起来简单，但一个问题：析构函数是一个特殊的函数，它有着特殊的语法和语义，MOCK_METHOD0对它是无效的：
    MOCK_METHOD(void, ~MockFoo, ());  // Won't compile!
 	好消息是你可以用一个简单的模式来达到相同的效果。首先，在你的Mock类中添加一个Mock函数Die()，并在析构函数中调用它，如下：
class MockFoo : public Foo {
  ...
  // Add the following two lines to the mock class.
  MOCK_METHOD(void, Die, ());
  ~MockFoo() override { Die(); }
};

//(如果Die()与已有符号冲突，选另一个名字)，现在，我们将测试一个MockFoo对象析构时的问题，转化为测试当Die函数被调用时的问题了：
  MockFoo* foo = new MockFoo;
  MockBar* bar = new MockBar;
  ...
  {
    InSequence s;

    // Expects *foo to die after bar->A() and before bar->B().
    EXPECT_CALL(*bar, A());
    EXPECT_CALL(*foo, Die());
    EXPECT_CALL(*bar, B());
  }

```

## Using Google Mock and Threads

```cpp
	这节所描述的只有在Google Mock是线程安全的平台上才是成立的。现在只有支持pthreads库的平台(这包括Linux和Mac)才可以。要让它在其它平台上线程安全，我们只需要在”gtest/internal/gtest-port.h”中实现一些同步操作。
    在一个单元测试中，如果你可以将一块代码独立出来在单线程环境中测试是最好的。这可以防止竞争和死锁，并使你debug你的测试容易的多。
    但许多程序是多线程的，并有时我们需要在多线程环境中测试它，Google Mock也提供了这个功能。

//回忆使用一个Mock的步骤：
1. 创建一个Mock对象foo。
2. 用ON_CALL()和EXPECT_CALL()设置它的默认行为和期望。
3. 测试调用foo的函数的代码。
4. 可选的，检查和重置mock。
5. 你自己销毁mock，或是让测试代码销毁mock，析构函数会自动检查是否满足。

如果你能遵循下面的简单规则，你的Mock和线程可以幸福地生活在一起：
1. 在单线程中执行你的测试代码(相对于被测试代码)。这可以保证你的测试容易被跟踪。
2. 显然，你可以在第一步中不用锁。
3. 当你做第2步和第5步，要保证没有其它线程访问foo。很显然，不是吗？
4. 第3步和第4步可以在单线程或是多线程中进行—随便你。Google Mock会去处理锁，所以你不需要再做什么事，除非是你测试需要。
如果你违反了这些规则(比如，如果你在其它线程正在访问的一个Mock上测试了期望)，你会得到一个不确定的行为。这不会是什么令你开心的事，所以别去尝试。
Google Mock保证一个Mock函数的动作会在调用这个Mock的线程中进行。比如： 
  EXPECT_CALL(mock, Foo(1))
      .WillOnce(action1);
  EXPECT_CALL(mock, Foo(2))
      .WillOnce(action2);

 	如果在线程1中被调用Foo(1)，且在线程2中调用Foo(2)，Google Mock会在线程1中执行action1，在线程2中执行action2。
    Google Mock不会对多个线程设置动作的顺序(这样做可能会产生死锁，因为动作可能需要配合 )。这意味着在上面的例子中执行action1和action2可能会交错。如果这样不符合你的意思，你应该对action1和action2加入同步逻辑，来保证测试线程安全。
    同样，要记得DefaultValue<T>是一个全局资源，所以它会影响你程序中的所有活动的Mock对象。多线程不推荐使用这个
```

## Controlling How Much Information Google Mock Prints

```cpp
 	当Google Mock看到有潜在的错误时(比如，一个没有设置期望的Mock函数被调用了 )，它会打印一些警告信息，包括这个函数的参数和返回值，并希望你可以提醒你关注一下，看这里是不是的确是一个错误。
    有时你对你的测试正确性比较自信，也许就不喜欢这些友好的提示。有的时候，你在debug你的测试，并在推敲你所测试的代码的行为，那你也许会希望看到每个Mock函数被调用的信息(包括参数值和返回值 )。很明显，一种打印级别是不是满足所有需求的。
    你可以通过—gmock_verbose=LEVEL命令参数来设置打印级别，其中LEVEL是一个有三种取值的字符串。
        
1. info：Google Mock会打印所有的信息，包括正常的消息，警告，错误。在这种级别设置上，Google Mock会记录所有对于ON_CALL/EXPECT_CALL的调用。
2 . warning：Google Mock会打印警告和错误信息，这是默认的。
3. error：Google仅会打印错误。
        
另外，你可以在你的测试中设置打印级别，如：
::testing::FLAGS_gmock_verbose ="error";
现在，请明智地选择打印级别，让Google Mock更好地为你服务。
```

## Writing New Matchers Quickly

```cpp
 MATCHER*宏系列可以很容易地用来定义自己的匹配器。语法是：
   MATCHER(name, description_string_expression) { statements; }

   这个宏会定义一个名为name的匹配器，这个匹配器执行statements语句，statetements必须返回一个bool值，以来表示这次匹配是否成功。在statements内部，你可以用arg来表示被匹配的值，这个值的类型用arg_type表示。

   Description_string是一个字符串，用来描述这个匹配器的行为，并且在匹配失败的时候产生失败信息。它能(并且应该)对非逻辑进行判断( ::test::Not )，产生对应的错误描述。

   为了方便起见，我们允许描述字符串为空，在这种情况下Google Mock会用匹配器的名字中的单词作为描述。
   比如：
MATCHER(IsDivisibleBy7,"") {return (arg % 7) == 0; }

或是：
using ::testing::Not;
...
EXPECT_THAT(some_expression, IsDivisibleBy7());
EXPECT_THAT(some_other_expression, Not(IsDivisibleBy7()));

 当上面的断言失败时，它们会打印下面的信息：
Value of: some_expression
Expected: is divisible by 7
Actual: 27
...
Value of: some_other_expression
Expected:not (is divisible by 7)
Actual: 21

   其中描述”is divisible by 7”和”not (is divisiable by 7)”是通过IsDivisibleBy7这个匹配器名字自动产生的。
   正如你所注意到的，自动产生的描述(特别是由非逻辑产生的)并不是那么好。你可以自定义描述。
MATCHER(IsDivisibleBy7, std::string(negation ?"isn't" :"is") +
                  " divisible by 7") {
 return (arg % 7) == 0;
}

   或者，你可以将更多的信息用一个隐藏变量result_listener输出，来解释匹配结果。比如，一个更好的IsDivisibleBy7的更好定义是：
MATCHER(IsDivisibleBy7,"") {
  if ((arg % 7) == 0)
     return true;
  *result_listener << "the remainder is " << (arg % 7);
  return false;
}

   有了这个定义，上面断言会给出一个更好的提示：
Value of: some_expression
Expected: is divisible by 7
Actual: 27 (the remainder is 6)

   你应该让MatchAndExplain()打印其它附加信息，这些信息可以帮助一个用户理解匹配结果。注意它可以在成功的情况下解释为什么匹配成功(除非它是显然的 )-这在Not内部的匹配器中是很有效的。没有必要打印参数本身，因为Google Mock已经为你打印了。

注意：
1. 所匹配的值的类型(arg_type)是由你使用匹配器的上下文决定的，它是由编译器提供给你的，所以你不用操心如何去定义它(你也没法定义)。这样允许匹配器是多形的(即支持多种类型的)。比如，IsDivisibleBy7()可以用于匹配任何支持arg % 7 == 0转换为bool的类型。在上面的Bar(IsDivisibleBy7())例子中，如果Bar()接受int参数，arg_type就是int，如果它接受unsigned long整形，arg_type就是unsigned long，等等。

2. Google Mock不保证匹配器何时和被调用多少次。所以匹配器的逻辑必须是纯功能性的(比如，它不能有任何副作用，并且结果也不能依赖于匹配的值和匹配器参数之外的东西 )。无论你如何定义，这个条件是你在定义一个匹配器时必须要满足的( 比如，用下面章节介绍的方法 )。特别是，一个匹配器决不能调用一个Mock函数，因为这会改变Mock对象和Google Mock的状态；
```

## Writing New Parameterized Matchers Quickly

```cpp
有时你想定义有一个有参数的匹配器。对于这个要求你可以使用宏：
	MATCHER_P(name, param_name, description_string) { statements; }
其中description_string可以是””或是引用了param_name的描述。
    
比如：
	MATCHER_P(HasAbsoluteValue, value,"") {return abs(arg) == value; }
你可以写：
 	EXPECT_THAT(Blah("a"), HasAbsoluteValue(n));

这会得到下面的信息(假设n是10)：
Value of: Blah("a")
Expected: has absolute value 10
Actual: -9
    
    注意匹配器的描述和它的参数都被打印了，使信息更友好。
    在匹配器定义内，你可以写foo_type来引用一个名为foo的参数类型。比如在上例MATCHER_P(HasAbsoluteValue, Value)中，你可以用value_type来引用value的类型。
     Google Mock还提供MATCHER_P2，MATCHER_P3，...，MATCHER_P10以来支持多参数的匹配器：
	MATCHER_Pk(name, param_1, ..., param_k, description_string) { statements; }

//请注意匹配器的提示信息是针对匹配器一个特定的实例的，即参数是与真实值相关的。所以通常你会想要参数值成为描述的一部分。Google Mock可以让你通过在描述字符串中引用匹配器参数来达到这个目的。
using ::testing::PrintToString;
MATCHER_P2(InClosedRange, low, hi,
      std::string(negation ? "isn't" :"is") +" in range [" +
      PrintToString(low) + ", " + PrintToString(hi) +"]") {
  return low <= arg && arg <= hi;
}
EXPECT_THAT(3, InClosedRange(4, 6));

出于输入的方便，你可以视
MATCHER_Pk(Foo, p1, ..., pk, description_string) { ... }
为下面的简写：
template <typename p1_type, ..., typename pk_type>
FooMatcherPk<p1_type, ..., pk_type>
Foo(p1_type p1, ..., pk_type pk) { ... }
```

## Wrting New Monomorphic Matchers

```cpp
	一个实现了::testing::MatcherInterface<T>的T参数类型的匹配器可以做两种事：它判断参数T是否匹配匹配器，并可以描述它所匹配的类型。后一种能力可以在期望失败时给出可读的错误信息。
        
using ::testing::Matcher;
class DivisibleBy7Matcher {
 public:
  using is_gtest_matcher = void;

  bool MatchAndExplain(int n, std::ostream*) const {
    return (n % 7) == 0;
  }

  void DescribeTo(std::ostream* os) const {
    *os << "is divisible by 7";
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "is not divisible by 7";
  }
};

Matcher<int> DivisibleBy7() {
  return DivisibleBy7Matcher();
}

...
  EXPECT_CALL(foo, Bar(DivisibleBy7()));

 你可以通过输出更多的信息到MatchAndExplan()函数中的listener变量来改进提示信息：
 class DivisibleBy7Matcher :publicMatcherInterface<int> {
 public:
 virtual bool MatchAndExplain(int n,
                              MatchResultListener* listener)const {
   constint remainder = n % 7;
   if (remainder != 0) {
     *listener << "the remainder is " << remainder;
   }
   return remainder == 0;
 }
 ...
};

Then, EXPECT_THAT(x, DivisibleBy7()); may generate a message like this:
Value of: x
Expected: is divisible by 7
  Actual: 23 (the remainder is 2)
```

##  Writing New Actions Quickly

```cpp
	如果内置的动作不适合你，并且发现用Invoke()很不方便，你可以用ACTION*宏系列中的宏来快速定义一个新动作，它可以像内置的动作一样用于你的代码中。
通过在命名空间中写：
	ACTION(name) { statements; }


Sometimes you’ll want to parameterize an action you define. For that we have another macro
	ACTION_P(name, param) { statements; }
//For example,
	ACTION_P(Add, n) { return arg0 + n; }
//will allow you to write
// Returns argument #0 + 5.
	... WillOnce(Add(5));


//example 
ACTION_P2(ReturnDistanceTo, x, y) {
  double dx = arg0 - x;
  double dy = arg1 - y;
  return sqrt(dx*dx + dy*dy);
}
... WillOnce(ReturnDistanceTo(5.0, 26.5));

//
ACTION_P(Plus, a) { ... }
ACTION_P2(Plus, a, b) { ... }
```

## Restricting the Type of an Argument or Parameter in an ACTION

```cpp
ACTION(Foo) {
  // Makes sure arg0 can be converted to int.
  int n = arg0;
  ... use n instead of arg0 here ...
}

ACTION_P(Bar, param) {
  // Makes sure the type of arg1 is const char*.
  ::testing::StaticAssertTypeEq<const char*, arg1_type>();

  // Makes sure param can be converted to bool.
  bool flag = param;
}

where StaticAssertTypeEq is a compile-time assertion in googletest that verifies two types are the same.
```

