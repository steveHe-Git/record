## C++ STL(vector、set、map、stack、queue)的使用

### 1. vector

方法 | 功能
-------- | -----
push_back | 在末尾加如一个元素
pop_back | 在尾部弹出有一个元素
size | 获取长度
clear | 清空 
- clear: C++ 中都只需要调用clear()方法就可清空vector。
C++ 中vector的clear()只是清空vector，并不会清空开的内存。用一种方法可以清空vector的内存：
vector<int> v;
vector<int>().swap(v)

### 2. set
> 集合是由一些不重复的数据组成;

方法 | 功能|含义
-------- | -----|
insert | 插入一个元素|
erase| 删除一个元素|
count | 统计集合中模某个元素的个数|
size | 获取元素的个数|
clear | 清空|

- insert: C++ 中用insert()函数向集合中插入一个新的元素。注意如果集合中已经存在了某个元素，再次插入不会产生任何效果，集合中是不会出现重复元素的
- erase: C++ 中通过erase()函数删除集合中的一个元素，如果集合中不存在这个元素，不进行任何操作。
- count: C++ 中如果你想知道某个元素是否在集合中出现，你可以直接用count()函数。如果集合中存在我们要查找的元素，返回 1，否则会返回 0。
- size: 获取容器中元素的个数
- 迭代器： 迭代器的写法比较固定，set<T>::iterator it就定义了一个指向set<T>这种集合的迭代器 it，T是任意的数据类型。其中::iterator是固定的写法
- clear: 调用clear()函数就可清空set，同时会清空set占用的内存。

### 3.map
方法 | 功能|含义
-------- | -----|
insert | 插入一对映射|
count | 判断关键字是否存在|
size | 获取映射对个数|
clear | 清空|

- insert:  C++ 中通过insert()函数向集合中插入一个新的映射，参数是一个pair。pair是一个标准库类型，定义在头文件utility中。可以看成是有两个成员变量first和second的结构体，并且重载了<运算符（先比较first大小，如果一样再比较second）。当我们创建一个pair时，必须提供两个类型。
pair<string, int> p;
```
#include<map>
#include<string>
#include<utility>
using namespace std;
int main() {
    map<string, int> dict;              // dict 是一个 string 到 int 的映射，存放每个名字对应的班级号，初始时为空
    dict.insert(make_pair("Tom", 1));   // {"Tom"->1}
    dict.insert(make_pair("Jone", 2));  // {"Tom"->1, "Jone"->2}
    dict.insert(make_pair("Mary", 1));  // {"Tom"->1, "Jone"->2, "Mary"->1}
    dict.insert(make_pair("Tom", 2));   // {"Tom"->1, "Jone"->2, "Mary"->1}
    return 0;
}
```
- map迭代器： map的迭代器的定义和set差不多，map<T1, T2>::iterator it就定义了一个迭代器，其中T1、T2分别是key和value的类型
- count: 在 C++ 中，如果你想知道某个关键字是否被映射过，你可以直接用count()函数。如果关键字存在，返回 1，否则会返回 0。

### 4.stack
方法 | 功能|含义
-------- | -----|
push | 压入栈顶元素|
pop | 弹出栈顶元素|
top | 返回栈顶元素|
empty | 栈是否为空|
size |栈的元素个数|


### 5.queue
方法 | 功能|含义
-------- | -----|
push | 入队|
pop | 出对|
front | 访问队首元素|
empty | 判断队列是否为空|
size |统计队列元素个数|

- push: 通过push()方法在队尾插入一个新的元素。
- pop:  通过pop()方法可以让队首元素出队。
- front:  通过front()方法可以获取到当前的队首元素。
- empty: empty()方法可以判断队列是否为空，如果为空，方法返回true，否则返回false。这个方法的意义是，在我们每次调用front()和pop()之前，都要检查一下，保证队列不为空，否则去访问一个空队列的首部或者让一个空的队列出队就会发生错误
- 注意：队列没有 clear 方法
只能手动清空：
```c++
// 如果队列不空，用这样的方法清空一个队列
while (!q.empty()) 
{
    q.pop();
}
```

###  6.unordered_map
方法 | 功能
-------- | -----
begin()|返回指向容器中第一个键值对的正向迭代器。
end() |返回指向容器中最后一个键值对之后位置的正向迭代器。
cbegin() |和 begin() 功能相同，只不过在其基础上增加了 const 属性，即该方法返回的迭代器不能用于修改容器内存储的键值对。
cend()|和 end() 功能相同，只不过在其基础上，增加了 const 属性，即该方法返回的迭代器不能用于修改容器内存储的键值对。
empty()|若容器为空，则返回 true；否则 false。
size()|返回当前容器中存有键值对的个数。
max_size()|返回容器所能容纳键值对的最大个数，不同的操作系统，其返回值亦不相同。
operator[key]|该模板类中重载了 [] 运算符，其功能是可以向访问数组中元素那样，只要给定某个键值对的键 key，就可以获取该键对应的值。注意，如果当前容器中没有以 key 为键的键值对，则其会使用该键向当前容器中插入一个新键值对。
at(key) |返回容器中存储的键 key 对应的值，如果 key 不存在，则会抛出 out_of_range 异常。 
find(key) |查找以 key 为键的键值对，如果找到，则返回一个指向该键值对的正向迭代器；反之，则返回一个指向容器中最后一个键值对之后位置的迭代器（如果 end() 方法返回的迭代器）。
count(key) |在容器中查找以 key 键的键值对的个数。
equal_range(key) |返回一个 pair 对象，其包含 2 个迭代器，用于表明当前容器中键为 key 的键值对所在的范围。
emplace() |向容器中添加新键值对，效率比 insert() 方法高。
emplace_hint() |向容器中添加新键值对，效率比 insert() 方法高。
insert() |向容器中添加新键值对。
erase() |删除指定键值对。
clear() |清空容器，即删除容器中存储的所有键值对。
swap() |交换 2 个 unordered_map 容器存储的键值对，前提是必须保证这 2 个容器的类型完全相等。
bucket_count()|返回当前容器底层存储键值对时，使用桶（一个线性链表代表一个桶）的数量。
max_bucket_count()|返回当前系统中，unordered_map 容器底层最多可以使用多少桶。
bucket_size(n)|返回第 n 个桶中存储键值对的数量。
bucket(key) |返回以 key 为键的键值对所在桶的编号。
load_factor()|返回 unordered_map 容器中当前的负载因子。负载因子，指的是的当前容器中存储键值对的数量（size()）和使用桶数（bucket_count()）的比值，即 load_factor() = size() / bucket_count()。
max_load_factor() |返回或者设置当前 unordered_map 容器的负载因子。
rehash(n) |将当前容器底层使用桶的数量设置为 n。
reserve() |将存储桶的数量（也就是 bucket_count() 方法的返回值）设置为至少容纳count个元（不超过最大负载因子）所需的数量，并重新整理容器。
hash_function() |返回当前容器使用的哈希函数对象。



###  7.unordered_map和map的区别
1. 头文件
- map: #include < map >
- unordered_map: #include < unordered_map >
2. 内部实现机理
- map： map内部实现了一个红黑树，该结构具有自动排序的功能，因此map内部的所有元素都是有序的，红黑树的每一个节点都代表着map的一个元素，因此，对于map进行的查找，删除，添加等一系列的操作都相当于是对红黑树进行这样的操作，故红黑树的效率决定了map的效率。
- unordered_map: unordered_map内部实现了一个哈希表，因此其元素的排列顺序是杂乱的，无序的
3. 优点以及适用处
>map
- 优点：有序性，这是map结构最大的优点，其元素的有序性在很多应用中都会简化很多的操作红黑树，内部实现一个红黑书使得map的很多操作在lgn的时间复杂度下就可以实现，因此效率非常的高
- 缺点：空间占用率高，因为map内部实现了红黑树，虽然提高了运行效率，但是因为每一个节点都需要额外保存父节点，孩子节点以及红/黑性质，使得每一个节点都占用大量的空间
- 适用处：对于那些有顺序要求的问题，用map会更高效一些

> unordered_map
- 优点：因为内部实现了哈希表，因此其查找速度非常的快
- 缺点：哈希表的建立比较耗费时间
- 适用处：对于查找问题，unordered_map会更加高效一些，因此遇到查找问题，常会考虑一下用unordered_map