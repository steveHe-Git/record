# 谷歌测试中的match的使用方法

| Matcher                     | Description                                      |
| :-------------------------- | :----------------------------------------------- |
| `_`                         | `argument` can be any value of the correct type. |
| `A<type>()` or `An<type>()` | `argument` can be any value of type `type`.      |

## 基本match

```cpp

Matcher 	        Description
Eq(value) or value 	argument == value
Ge(value) 	        argument >= value
Gt(value) 	        argument > value
Le(value) 	        argument <= value
Lt(value) 	        argument < value
Ne(value) 	        argument != value
IsFalse() 	        argument evaluates to false in a Boolean context.
IsTrue() 	        argument evaluates to true in a Boolean context.
IsNull() 	        argument is a NULL pointer (raw or smart).
NotNull() 	        argument is a non-null pointer (raw or smart).
Optional(m) 	    argument is optional<> that contains a value matching m. (For testing whether an optional<> is set, check for equality with nullopt. You may need to use Eq(nullopt) if the inner type doesn’t have ==.)
VariantWith<T>(m) 	argument is variant<> that holds the alternative of type T with a value matching m.
Ref(variable) 	    argument is a reference to variable.
TypedEq<type>(value) 	argument has type type and is equal to value. You may need to use this instead of Eq(value) when the mock function is overloaded.
```

## 浮点匹配器

```cpp
Matcher 	                    Description
DoubleEq(a_double) 	            argument is a double value approximately equal to a_double, treating two NaNs as unequal.
FloatEq(a_float) 	            argument is a float value approximately equal to a_float, treating two NaNs as unequal.
NanSensitiveDoubleEq(a_double) 	argument is a double value approximately equal to a_double, treating two NaNs as equal.
NanSensitiveFloatEq(a_float) 	argument is a float value approximately equal to a_float, treating two NaNs as equal.
IsNan() 	                    argument is any floating-point type with a NaN value.
```

```cpp
Matcher 								            Description
DoubleNear(a_double, max_abs_error) 	            argument is a double value close to a_double (absolute error <= max_abs_error), treating two NaNs as unequal.
FloatNear(a_float, max_abs_error) 	                argument is a float value close to a_float (absolute error <= max_abs_error), treating two NaNs as unequal.
NanSensitiveDoubleNear(a_double, max_abs_error) 	argument is a double value close to a_double (absolute error <= max_abs_error), treating two NaNs as equal.
NanSensitiveFloatNear(a_float, max_abs_error) 	    argument is a float value close to a_float (absolute error <= max_abs_error), treating two NaNs as equal.
```

## srting字符串匹配器

```cpp
Matcher 	             Description
ContainsRegex(string) 	 argument matches the given regular expression.
EndsWith(suffix) 	     argument ends with string suffix.
HasSubstr(string) 	     argument contains string as a sub-string.
IsEmpty() 	             argument is an empty string.
MatchesRegex(string) 	 argument matches the given regular expression with the match starting at the first character and ending at the last character.
StartsWith(prefix) 	     argument starts with string prefix.
StrCaseEq(string) 	     argument is equal to string, ignoring case.
StrCaseNe(string) 	     argument is not equal to string, ignoring case.
StrEq(string) 	         argument is equal to string.
StrNe(string) 	         argument is not equal to string.
```

## 容器的匹配

```cpp
很多STL的容器的比较都支持==这样的操作，对于这样的容器可以使用上述的Eq(container)来比较。但如果你想写得更为灵活，可以使用下面的这些容器匹配方法：
Contains(e) 	             在method的形参中，只要有其中一个元素等于e
Each(e) 	                 参数各个元素都等于e
ElementsAre(e0, e1, …, en) 	 形参有n+1的元素，并且挨个匹配
ElementsAreArray(array)      或者ElementsAreArray(array, count) 	和ElementsAre()类似，除了预期值/匹配器来源于一个C风格数组
ContainerEq(container) 	     类型Eq(container)，就是输出结果有点不一样，这里输出结果会带上哪些个元素不被包含在另一个容器中
Pointwise(m, container) 	 
```

## 成员匹配器

```cpp
Field(&class::field, m) 	argument.field (或 argument->field, 当argument是一个指针时)与匹配器m匹配, 这里的argument是一个class类的实例.
Key(e) 					        形参（argument）比较是一个类似map这样的容器，然后argument.first的值等于e
Pair(m1, m2) 	                形参（argument）必须是一个pair，并且argument.first等于m1，argument.second等于m2.
Property(&class::property, m) 	argument.property()(或argument->property(),当argument是一个指针时)与匹配器m匹配, 这里的argument是一个class类的实例.
    
    
Field(&class::field, m) 	                   argument.field (or argument->field when argument is a plain pointer) matches matcher m, where argument is an object of type class.
Field(field_name, &class::field, m)             The same as the two-parameter version, but provides a better error message.
Key(e) 	                                        argument.first matches e, which can be either a value or a matcher. E.g. Contains(Key(Le(5))) can verify that a map contains a key <= 5.
Pair(m1, m2) 	                                argument is an std::pair whose first field matches m1 and second field matches m2.
FieldsAre(m...) 	                            argument is a compatible object where each field matches piecewise with the matchers m.... A compatible object is any that supports the std::tuple_size<Obj>+get<I>(obj) protocol. In C++17 and up this also supports types compatible with structured bindings, like aggregates.
Property(&class::property, m) 	                argument.property() (or argument->property() when argument is a plain pointer) matches matcher m, where argument is an object of type class. The method property() must take no argument and be declared as const.
Property(property_name, &class::property, m) 	The same as the two-parameter version, but provides a better error message.
```

## 匹配函数或者函数的返回值

```cpp
ResultOf(f, m) 	     f(argument) 与匹配器m匹配, 这里的f是一个函数或函数对象. 
```

## 指针匹配器

```cpp
Pointee(m) 			argument (不论是智能指针还是原始指针) 指向的值与匹配器m匹配.
    
Address(m) 			the result of std::addressof(argument) matches m.
Pointee(m) 			argument (either a smart pointer or a raw pointer) points to a value that matches matcher m.
Pointer(m) 			argument (either a smart pointer or a raw pointer) contains a pointer that matches m. m will match against the raw pointer regardless of the type of argument.
WhenDynamicCastTo<T>(m) 	when argument is passed through dynamic_cast<T>(), it matches matcher m.
```

## 复合匹配器

```cpp
AllOf(m1, m2, …, mn) 	  argument 匹配所有的匹配器m1到mn
AnyOf(m1, m2, …, mn) 	  argument 至少匹配m1到mn中的一个
Not(m) 	                  argument 不与匹配器m匹配
    
AllOf(m1, m2, ..., mn) 	  argument matches all of the matchers m1 to mn.
AllOfArray({m0, m1, ..., mn}), AllOfArray(a_container), AllOfArray(begin, end), AllOfArray(array), or AllOfArray(array, count) 	                            The same as AllOf() except that the matchers come from an initializer list, STL-style container, iterator range, or C-style array.
AnyOf(m1, m2, ..., mn) 	   argument matches at least one of the matchers m1 to mn.
AnyOfArray({m0, m1, ..., mn}), AnyOfArray(a_container), AnyOfArray(begin, end), AnyOfArray(array), or AnyOfArray(array, count) 	                            The same as AnyOf() except that the matchers come from an initializer list, STL-style container, iterator range, or C-style array.
Not(m) 	                   argument doesn’t match matcher m.   

1. EXPECT_CALL(foo, DoThis(AllOf(Gt(5), Ne(10))));
传入的参数必须 >5 并且 <= 10
2. EXPECT_CALL(foo, DoThat(Not(HasSubstr("blah")), NULL));
第一个参数不包含“blah”这个子串
```
## 多参数匹配器
```cpp
从技术上讲，所有匹配器都匹配一个值。 “多参数”匹配器只是匹配元组的匹配器。 以下匹配器可用于匹配元组 (x, y)： 
    Matcher 	Description
    Eq() 		x == y
    Ge() 		x >= y
    Gt() 		x > y
    Le() 		x <= y
    Lt() 		x < y
    Ne() 		x != y
 
您可以使用以下选择器来选择参数的子集（或重新排序）以参与匹配：
    Matcher 	            Description
AllArgs(m) 	                Equivalent to m. Useful as syntactic sugar in .With(AllArgs(m)).
Args<N1, N2, ..., Nk>(m) 	The tuple of the k selected (using 0-based indices) arguments matches m, e.g. Args<1, 2>(Eq()).
```


## 基数

```cpp
基数用于Times()中来指定模拟函数将被调用多少次|
AnyNumber() 	 函数可以被调用任意次.
AtLeast(n) 	     预计至少调用n次.
AtMost(n) 	     预计至多调用n次.
Between(m, n) 	 预计调用次数在m和n(包括n)之间.
Exactly(n) 或 n  预计精确调用n次. 特别是, 当n为0时,函数应该永远不被调用.
```

## 行为（Actions）

```cpp
Actions（行为）用于指定Mock类的方法所期望模拟的行为：比如返回什么样的值、对引用、指针赋上怎么样个值，等等。 值的返回
    
Return() 	让Mock方法返回一个void结果
Return(value) 	返回值value
ReturnNull() 	返回一个NULL指针
ReturnRef(variable) 	返回variable的引用.
ReturnPointee(ptr) 	返回一个指向ptr的指针
    
    
Assign(&variable, value) 	将value分配给variable
    
Invoke(f) 	使用模拟函数的参数调用f, 这里的f可以是全局/静态函数或函数对象.
Invoke(object_pointer, &class::method) 	使用模拟函数的参数调用object_pointer对象的mothod方法.
    
DoAll(a1, a2, …, an) 	每次发动时执行a1到an的所有动作.
IgnoreResult(a) 	执行动作a并忽略它的返回值. a不能返回void.
```

## 基本使用

```cpp
EXPECT_CALL(mock_object, method(matcher1, matcher2, ...))
    .With(multi_argument_matcher) //多个参数的匹配方式指定
    .Times(cardinality) //执行多少次
    .InSequence(sequences) //函数执行顺序
    .After(expectations) //指定某个方法只能在另一个方法后执行
    .WillOnce(action) //一般使用return，指定一次调用的输出
    .WillRepeatedly(action) //一直输出
    .RetiresOnSaturation(); //期待调用不会被相同的函数期待所覆盖
```

## EXPEXT_CALL和ON_CALL区别

```cpp
EXPECT_CALL(mock, methodX(_)).WillRepeatedly(do_action);
当调用方法methodX(_)时，将执行do_action
    
ON_CALL(mock, methodX(_)).WillByDefault(do_action);
当调用方法methodX(_)时，将默认执行do_action， 如果没有设置EXPECT_CALL将调用默认的on_call;
    
ON_CALL(mock, Sign(Gt(0), _)).WillByDefault(Return(1));
EXPECT_CALL(mock, Sign(10, _));
执行mock.Sign(10, p)将返回1，因为int为EXPECT_CALL指定的呼叫提供了默认行为ON_CALL；
    
ON_CALL(mock, Sign(Gt(0), _)).WillByDefault(Return(1));
EXPECT_CALL(mock, Sign(10, _));
EXPECT_CALL(mock, Sign(Gt(0), _).WillRepeatedly(Return(0));
mock.Sign(10, p)的调用将返回0。它将与最后一个期望值匹配。 该期望未指定任何显式操作
```

