# Gmock的简单使用

> 介绍

```cpp
Gmock是google开发的一套辅助测试的工具，它往往和GTest结合在一起使用。在实际工作中，一个人不可能完成整条线的开发工作。于是我们会在约定接口的前提下，各自完成各自的模块。自己的模块开发完之后，我们需要自测。但是这个时候别人的模块可能还没完成，那么我们就需要模拟约定的接口进行自测。Gmock就是一个强大的模拟接口的工具。
```

> 使用方法

```c++
我们假设一个支付场景逻辑开发业务。我们开发复杂的业务模块，而团队其他成员开发用户行为模块。他们和我们约定了如下接口:
class User {
public:
    User() {};
    ~User() {};
public:
    // 登录
    virtual bool Login(const std::string& username, const std::string& password) = 0;
    // 支付
    virtual bool Pay(int money) = 0;
    // 是否登录
    virtual bool Online() = 0;
};

我们的业务模块要让用户登录，并发起支付行为。于是我们的代码如下:
class Biz {
public:
    void SetUser(User* user) {
        _user = user;
    }
 
    std::string pay(const std::string& username, const std::string& password, int money) {
        std::string ret;
        if (!_user) {
            ret = "pointer is null.";
            return ret;
        }
        
        if (!_user->Online()) {
            ret = "logout status.";
            // 尚未登录，要求登录
            if (!_user->Login(username, password)) {
                // 登录失败
                ret += "login error.";
                return ret;
            } else {
                // 登录成功
                ret += "login success.";
            }
        } else {
            // 已登录
            ret = "login.status";
        }
 
        if (!_user->Pay(money)) {
            ret += "pay error.";
        } else {
            ret += "pay success.";
        }
 
        return ret;
    }
 
private:
    User* _user;
};

这段逻辑的口语描述就是：我们先看看用户登录了没，如果没有登录则要求用户登录。如果登录失败，则直接返回；如果登录成功，则执行支付行为。最后将流程的状态输出。
```

> 使用Gmock辅助测试

```cpp
1. 需要使用Mock接口类
class TestUser : public User {
public:
    MOCK_METHOD2(Login, bool(const std::string&, const std::string&));
    MOCK_METHOD1(Pay, bool(int));
    MOCK_METHOD0(Online, bool());
};
可以发现其方法的声明非常有规律。MOCK_METHOD后跟一个数字，该数字表明需要mock的函数有几个参数（除去this)。像第5行，Online()方法没有入参，则使用的是MOCK_METHOD0。而又两个参数的Login使用的是MOCK_METHOD2。MOCK_METHOD系列宏的第一个参数是函数名，第二个参数是函数指针的类型。

2. 设计测试场景
在设计场景之前，我们先看一些Gmock的方法：
//   EXPECT_CALL(mock_object, Method(argument-matchers))
//       .With(multi-argument-matchers)
//       .Times(cardinality)
//       .InSequence(sequences)
//       .After(expectations)
//       .WillOnce(action)
//       .WillRepeatedly(action)
//       .RetiresOnSaturation();
//
// where all clauses are optional, and .InSequence()/.After()/
// .WillOnce() can appear any number of times.
    
- EXPECT_CALL声明一个调用期待，就是我们期待这个对象的这个方法按什么样的逻辑去执行。
- mock_object是我们mock的对象，上例中就是TestUser的一个对象。
- Method是mock对象中的mock方法，它的参数可以通过argument-matchers规则去匹配。
- With是多个参数的匹配方式指定。
- Times表示这个方法可以被执行多少次。如果超过这个次数，则按默认值返回了。
- InSequence用于指定函数执行的顺序。它是通过同一序列中声明期待的顺序确定的。
- After方法用于指定某个方法只能在另一个方法之后执行。
- WillOnce表示执行一次方法时，将执行其参数action的方法。一般我们使用Return方法，用于指定一次调用的输出。
- WillRepeatedly表示一直调用一个方法时，将执行其参数action的方法。需要注意下它和WillOnce的区别，WillOnce是一次，WillRepeatedly是一直。
- RetiresOnSaturation用于保证期待调用不会被相同的函数的期待所覆盖。
```

> example1
```cpp
举一个例子，我们要求Online在第一调用时返回true，之后都返回false。Login一直返回false。Pay一直返回true。也就是说用户第一次支付前处于在线状态，并可以支付成功。而第二次将因为不处于在线状态，要触发登录行为，而登录行为将失败。我们看下这个逻辑该怎么写;
2    {
3        TestUser test_user;
4        EXPECT_CALL(test_user, Online()).WillOnce(testing::Return(true));
5        EXPECT_CALL(test_user, Login(_,_)).WillRepeatedly(testing::Return(false));
6        EXPECT_CALL(test_user, Pay(_)).WillRepeatedly(testing::Return(true));
 
7        Biz biz;
8        biz.SetUser(&test_user);
9        std::string admin_ret = biz.pay("user", "", 1);
10       admin_ret = biz.pay("user", "", 1);
    }
第4行的意思是Online在调用一次后返回true，之后的调用返回默认的false。第5行意思是Login操作一直返回false，其中Login的参数是两个下划线（_），它是通配符，就是对任何输入参数都按之后要求执行。第6行意思是Pay操作总是返回true。那么我们在第10行和第11行分别得到如下输出:
login status.pay success.
logout status.login error.
```

> example2

```cpp
我们再看一种场景，这个场景我们使用了函数参数的过滤。比如我们不允许admin的用户通过我们方法登录并支付，则可以这么写
 1   {
 2       TestUser test_user;
 3       EXPECT_CALL(test_user, Online()).WillOnce(testing::Return(false));
 4       EXPECT_CALL(test_user, Login("admin",_)).WillRepeatedly(testing::Return(false));
 
        Biz biz;
        biz.SetUser(&test_user);
        std::string admin_ret = biz.pay("admin", "", 1);
    }
第3行表示，如果Login的第一个参数是admin，则总是返回false。输出为：
    logout status.login error.
    
    
那么如果不是admin的用户登录，则返回成功，这个案例要怎么写呢？
1    {
2        TestUser test_user;
3        EXPECT_CALL(test_user, Online()).WillOnce(testing::Return(false));
4        EXPECT_CALL(test_user, Login(StrNe("admin"),_)).WillRepeatedly(testing::Return(true));
5        EXPECT_CALL(test_user, Pay(_)).WillRepeatedly(testing::Return(true));
 
        Biz biz;
        biz.SetUser(&test_user);
        std::string user_ret = biz.pay("user", "", 1);
    }
04行使用了StrNe的比较函数，即Login的第一个参数不等于admin时，总是返回true。输出是
  logout status.login success.pay success.
    
 
我们再看一个例子，我们要求非admin用户登录成功后，只能成功支付2次，之后的支付都失败。这个案例可以这么写
   {
        TestUser test_user;
        EXPECT_CALL(test_user, Online()).WillOnce(testing::Return(false));
        EXPECT_CALL(test_user, Login(StrNe("admin"),_)).WillRepeatedly(testing::Return(true));
        EXPECT_CALL(test_user, Pay(_)).Times(5).WillOnce(testing::Return(true)).WillOnce(testing::Return(true)).WillRepeatedly(testing::Return(false));
 
        Biz biz;
        biz.SetUser(&test_user);
        std::string user_ret = biz.pay("user", "", 1);
        user_ret = biz.pay("user", "", 1);
        user_ret = biz.pay("user", "", 1);
    }
我们使用Times函数，它的参数5表示该函数期待被调用5次，从第6次的调用开始，返回默认值。Times函数后面跟着两个WillOnce，其行为都是返回true。这个可以解读为第一次和第二次调用Pay方法时，返回成功。最后的WillRepeatedly表示之后的对Pay的调用都返回false。我们看下执行的结果
logout status.login success.pay success.
logout status.login success.pay success.
logout status.login success.pay error.
从结果上看，前两次都支付成功了，而第三次失败。符合我们的期待。
```

# Gmock详讲

> Gmock 匹配器

```cpp
    EXPECT_CALL(mock_object, method(matcher1, matcher2, ...))
        .With(multi_argument_matcher)
        .Times(cardinality)
        .InSequence(sequences)
        .After(expectations)
        .WillOnce(action)
        .WillRepeatedly(action)
        .RetiresOnSaturation();
method(matcher1, matcher2, …)中的method就是你Mock类中的某个方法名，比如上述的getArbitraryString;而matcher（匹配器）的意思是定义方法参数的类型;


EXPECT_CALL(mockTurtle, getX()).Times(testing::AtLeast(5)).
    WillOnce(testing::Return(100)).WillOnce(testing::Return(150)).
    WillRepeatedly(testing::Return(200))
这个期望行为的定义的意思是：
    调用mockTurtle的getX()方法
    这个方法会至少调用5次
    第一次被调用时返回100
    第2次被调用时返回150
    从第3次被调用开始每次都返回200  
    
匹配器:
_	         可以代表任意类型
A() or An()	 
    
一般比较：
Eq(value) 或者 value	argument == value，method中的形参必须是value
Ge(value)	         argument >= value，method中的形参必须大于等于value
Gt(value)	         argument > value
Le(value)	         argument <= value
Lt(value)	         argument < value
Ne(value)	         argument != value
IsNull()	         method的形参必须是NULL指针
NotNull()	         argument is a non-null pointer
Ref(variable)	     形参是variable的引用
TypedEq(value)	     形参的类型必须是type类型，而且值必须是value

浮点比较：
DoubleEq(a_double)	形参是一个double类型，比如值近似于a_double，两个NaN是不相等的
FloatEq(a_float)	同上，只不过类型是float
NanSensitiveDoubleEq(a_double)	形参是一个double类型，比如值近似于a_double，两个NaN是相等的，这个是用户所希望的方式
NanSensitiveFloatEq(a_float)	同上，只不过形参是float

字符串匹配
这里的字符串即可以是C风格的字符串，也可以是C++风格的。
ContainsRegex(string)	形参匹配给定的正则表达式
EndsWith(suffix)	    形参以suffix截尾
HasSubstr(string)	    形参有string这个子串
MatchesRegex(string)	从第一个字符到最后一个字符都完全匹配给定的正则表达式.
StartsWith(prefix)	    形参以prefix开始
StrCaseEq(string)	    参数等于string，并且忽略大小写
StrCaseNe(string)	    参数不是string，并且忽略大小写
StrEq(string)	        参数等于string
StrNe(string)	        参数不等于string
    
容器的匹配
很多STL的容器的比较都支持==这样的操作，对于这样的容器可以使用上述的Eq(container)来比较。但如果你想写得更为灵活，可以使用下面的这些容器匹配方法：
Contains(e)	                在method的形参中，只要有其中一个元素等于e
Each(e)	                    参数各个元素都等于e
ElementsAre(e0, e1, …, en)	形参有n+1的元素，并且挨个匹配
ElementsAreArray(array) 或者ElementsAreArray(array, count)	和ElementsAre()类似，除了预期值/匹配器来源于一个C风格数组
ContainerEq(container)	     类似Eq(container)，就是输出结果有点不一样，这里输出结果会带上哪些个元素不被包含在另一个容器中
Pointwise(m, container)

上述的一些匹配器都比较简单，我就随便打包举几最简单的例子演示一下吧： 我稍微修改一下之前的Foo.h和MockFoo.h
FooInterface.h
#ifndef FOOINTERFACE_H_
#define FOOINTERFACE_H_
#include <string>
namespace seamless {
class FooInterface {
public:
        virtual ~FooInterface() {}

public:
        virtual std::string getArbitraryString() = 0;
};
}  // namespace seamless
    
MockFoo.h 增加了2个方法
    #ifndef MOCKFOO_H_
    #define MOCKFOO_H_

    #include <gmock/gmock.h>
    #include <string>
    #include <vector>
    #include "FooInterface.h"

    namespace seamless {

    class MockFoo: public FooInterface {
    public:
            MOCK_METHOD0(getArbitraryString, std::string());
            MOCK_METHOD1(setValue, void(std::string& value));
            MOCK_METHOD2(setDoubleValues, void(int x, int y));
    };

    }  // namespace seamless

    #endif // MOCKFOO_H_

FooMain.h
1    #include <cstdlib>
2    #include <gmock/gmock.h>
3    #include <iostream>
4    #include <string>

5    #include "MockFoo.h"



12   int main(int argc, char** argv) {
13           ::testing::InitGoogleMock(&argc, argv);

14           string value = "Hello World!";
15           MockFoo mockFoo;

16           EXPECT_CALL(mockFoo, setValue(testing::_));
17           mockFoo.setValue(value);

             // 这里我故意犯错
18           EXPECT_CALL(mockFoo, setDoubleValues(Eq(1), Ge(1)));
19           mockFoo.setDoubleValues(1, 0)；
             return EXIT_SUCCESS;
    }

    第16行，让setValue的形参可以传入任意参数
    另外，我在第18~19行故意犯了个错（为了说明上述这些匹配器的作用），我之前明明让setDoubleValues第二个参数得大于等于1,但我实际传入时却传入一个0。这时程序运行时就报错了：
```

> Gmock复杂的匹配器

```
## 成员匹配器
Field(&class::field, m)	        argument.field (或 argument->field, 当argument是一个指针时)与匹配器中的m匹配, 这里的argument是一个class类的实例.
Key(e)	                        形参（argument）比较是一个类似map这样的容器，然后argument.first的值等于e
Pair(m1, m2)	                形参（argument）必须是一个pair，并且argument.first等于m1，argument.second等于m2.
Property(&class::property, m)	argument.property()(或argument->property(),当argument是一个指针时)与匹配器m匹配, 这里的argument是一个class类的实例.
---------------------------------------------------------------------------------------------------------------------------------   
TEST(TestField, Simple) {
    MockFoo mockFoo;
    Bar bar;
5    EXPECT_CALL(mockFoo, get(Field(&Bar::num, Ge(0)))).Times(1);
    mockFoo.get(bar);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
这里我们使用Google Test来写个测试用例，这样看得比较清楚。
    第5行，我们定义了一个Field(&Bar::num, Ge(0))，以说明Bar的成员变量num必须大于等于0。
    
上面这个是正确的例子，我们为了说明Field的作用，传入一个bar.num = -1试试。
TEST(TestField, Simple) {
    MockFoo mockFoo;
    Bar bar;
    bar.num = -1;
    EXPECT_CALL(mockFoo, get(Field(&Bar::num, Ge(0)))).Times(1);
    mockFoo.get(bar);
}

## 匹配函数或函数对象的返回值
ResultOf(f, m)	f(argument) 与匹配器中的m匹配, 这里的f是一个函数或函数对象.

## 指针匹配器
Pointee(m)	argument (不论是智能指针还是原始指针) 指向的值与匹配器中的m匹配.
    
## 复合匹配器
AllOf(m1, m2, …, mn)	argument 匹配所有的匹配器m1到mn
AnyOf(m1, m2, …, mn)	argument 至少匹配m1到mn中的一个
Not(m)	                argument 不与匹配器m匹配
    
EXPECT_CALL(foo, DoThis(AllOf(Gt(5), Ne(10))));          //传入的参数必须 >5 并且 <= 10
EXPECT_CALL(foo, DoThat(Not(HasSubstr("blah")), NULL));  //第一个参数不包含“blah”这个子串

### 基数
AnyNumber()	     函数可以被调用任意次.
AtLeast(n)	     预计至少调用n次.
AtMost(n)	     预计至多调用n次.
Between(m, n)	 预计调用次数在m和n(包括n)之间.
Exactly(n) 或 n	预计精确调用n次. 特别是, 当n为0时,函数应该永远不被调用.
    
### 行为（Actions）
    Actions（行为）用于指定Mock类的方法所期望模拟的行为：比如返回什么样的值、对引用、指针赋上怎么样个值，等等。 值的返回
Return()	        让Mock方法返回一个void结果
Return(value)	    返回值value
ReturnNull()	    返回一个NULL指针
ReturnRef(variable)	返回variable的引用.
ReturnPointee(ptr)	返回一个指向ptr的指针

Assign(&variable, value)	将value分配给variable
#使用函数或者函数对象（Functor）作为行为
Invoke(f)	  使用模拟函数的参数调用f, 这里的f可以是全局/静态函数或函数对象.
Invoke(object_pointer, &class::method)	使用模拟函数的参数调用object_pointer对象的mothod方法.
#复合动作
DoAll(a1, a2, …, an)	每次发动时执行a1到an的所有动作.
IgnoreResult(a)	执行动作a并忽略它的返回值. a不能返回void.
    
这里我举个例子来解释一下DoAll()的作用，我个人认为这个DoAll()还是挺实用的。例如有一个Mock方法：
    virtual int getParamter(std::string* name,  std::string* value) = 0
对于这个方法，我这回需要操作的结果是将name指向value的地址，并且得到方法的返回值。
类似这样的需求，我们就可以这样定义期望过程：
TEST(SimpleTest, F1) {
    std::string* a = new std::string("yes");
    std::string* b = new std::string("hello");
    MockIParameter mockIParameter;
    EXPECT_CALL(mockIParameter, getParamter(testing::_, testing::_)).Times(1).\
        WillOnce(testing::DoAll(testing::Assign(&a, b), testing::Return(1)));
    mockIParameter.getParamter(a, b);
} 
这时就用上了我们的DoAll()了，它将Assign()和Return()结合起来了。

### 序列（Sequences）
默认时，对于定义要的期望行为是无序（Unordered）的，即当我定义好了如下的期望行为：
            MockFoo mockFoo;
            EXPECT_CALL(mockFoo, getSize()).WillOnce(Return(1));
            EXPECT_CALL(mockFoo, getValue()).WillOnce(Return(string("Hello World")));
对于这样的期望行为的定义，我何时调用mockFoo.getValue()或者何时mockFoo.getSize()都可以的。
但有时候我们需要定义有序的（Ordered）的调用方式，即序列 (Sequences) 指定预期的顺序. 在同一序列里的所有预期调用必须按它们指定的顺序发生; 反之则可以是任意顺序.
    using ::testing::Return;
    using ::testing::Sequence;

    int main(int argc, char **argv) {
            ::testing::InitGoogleMock(&argc, argv);

            Sequence s1, s2;
            MockFoo mockFoo;
            EXPECT_CALL(mockFoo, getSize()).InSequence(s1, s2).WillOnce(Return(1));
            EXPECT_CALL(mockFoo, getValue()).InSequence(s1).WillOnce(Return(
                    string("Hello World!")));
            cout << "First:\t" << mockFoo.getSize() << endl;
            cout << "Second:\t" << mockFoo.getValue() << endl;

            return EXIT_SUCCESS;
    }
    首先在建立两个序列：s1、s2。
    EXPECT_CALL(mockFoo, getSize()).InSequence(s1, s2)说明getSize()的行为优先于s1、s2.
    EXPECT_CALL(mockFoo, getValue()).InSequence(s1)说明getValue()的行为在序列s1中。
        
当我尝试一下把mockFoo.getSize()和mockFoo.getValue()的调用对调时试试：
            cout << "Second:\t" << mockFoo.getValue() << endl;
            cout << "First:\t" << mockFoo.getSize() << endl;
会测试不通过，并且报错
```

# 单元测试的经验之道

```cpp
给自己的单元测试设置了5个级别：
    1. Level1：正常流程可用，即一个函数在输入正确的参数时，会有正确的输出;
	2. Level2：异常流程可抛出逻辑异常，即输入参数有误时，不能抛出系统异常，而是用自己定义的逻辑异常通知上层调用代码其错误之处;
    3. Level3：极端情况和边界数据可用，对输入参数的边界情况也要单独测试，确保输出是正确有效的;
	4. Level4：所有分支、循环的逻辑走通，不能有任何流程是测试不到的;
	5. Level5：输出数据的所有字段验证，对有复杂数据结构的输出，确保每个字段都是正确的
    如上的单元测试分级是我2007年整理出来的，后来在我做的各种项目中，一般只做到Level2，重要系统或者底层服务，要做到Level3或Level4。而很少做到Level5。即便如此，就已经实现了如上所说的，很难被测试工程师发现bug。
    除了级别外，测试方法也要区分不同系统的玩法。比如基于WEB的系统，就需要确保单元测试里可以模拟发送请求，这个一般是WEB框架提供支持的。比如我常用的web.py、Flask、Django都有支持。不仅仅可以模拟简单的请求，还可以模拟POST、cookie等。另外一般建议单独写个函数来模拟登录过程，这样系统登录后行为的测试就不必反复模拟登录了。
    
    ###重点1
    单元测试一大痛苦是构造测试数据。我的看法是测试数据应该是人造的，而不是随便从产品环境dump出来一份。只有人造的数据能确保环境可控，每次运行不会因为环境改变而频繁修改testcase。我的常用玩法是测试数据分为基础数据和附加数据两部分。基础数据是所有testcase共享的，比如建立几个常用角色的用户等等。附加数据是testcase内部自己建立的。这样每次testcase运行时，先清空数据库，导入基础数据，导入附加数据，然后执行测试，验证结果。
    ###重点2
    各类程序的函数可以分为纯函数和副作用函数。纯函数对应的是数学里函数的概念，输出和输入是一一对应的。对一个输入有确定的输出。比如1+1=2。而副作用函数则相反，同样的输入，在不同时间和环境里，可能有不同的输出。比如任何涉及IO、网络、数据库的。副作用函数的测试比纯函数麻烦的多，因为你必须要完整的构造其所依赖的所有环境，才能够复现一个副作用函数的行为。也正因为如此，副作用函数出bug的概率比纯函数高的多。理解这个概念以后，应该尽可能的把程序里的纯函数和副作用函数进行拆解，降低副作用函数的比例和逻辑复杂度。还有，副作用函数是会传染的，一个函数如果调用了副作用函数，那么它也会变成副作用函数。
```

