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

