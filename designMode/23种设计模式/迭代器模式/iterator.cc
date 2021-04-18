/* 迭代器模式
 *使用vector实现一个二次封装的容器，并定义容器的迭代器，实现二次开发的迭代器的便捷访问
*/

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

template<typename T>
class Iterator
{
private:
    /* data */
public:
    Iterator(/* args */){}
    virtual ~Iterator(){}
    virtual bool isEnd() = 0;
    virtual void first() = 0;
    virtual void next() = 0;
    virtual T* curItem() = 0;
};

template<typename T>
using Iterator_ptr = std::shared_ptr<Iterator<T>>;

template<typename T>
class container
{
private:
    /* data */
public:
    container(/* args */){}
    virtual ~container(){}
    virtual T& getIntem(int index) = 0;
    virtual void pushItem(T item) = 0;
    virtual size_t size() = 0;
    //创建一个迭代器对象
    virtual Iterator_ptr<T> creatInterator() = 0;
};

template<typename T>
using container_ptr = std::shared_ptr<container<T>>;

template<typename T>
class concreatInterator:public Iterator<T>
{
private:
    /* data */
    container_ptr<T> m_container;
    int cur_pos;
public:
    concreatInterator(container_ptr<T> container):m_container(container),cur_pos(0){}
    ~concreatInterator(){}
    virtual bool isEnd(){
        return cur_pos >= m_container->size();
    }

    virtual void first(){
        cur_pos = 0;
    }

    virtual void next(){
        if (cur_pos < m_container->size()){
            cur_pos++;
        }
    }

    T* curItem(){
        if (cur_pos < m_container->size()) {
            return &(m_container->getIntem(cur_pos));
        } else {
            return nullptr;
        }
    }
};

template<typename T>
class concreatContainer:public container<T>,public std::enable_shared_from_this<concreatContainer<T>>
{
private:
    std::vector<T> m_vector;
public:
    concreatContainer(/* args */){}
    ~concreatContainer(){}

    virtual T& getIntem(int index){
        return m_vector[index];
    }

    virtual void pushItem(T item) {
        m_vector.push_back(item);
    }

    virtual size_t size(){
        return m_vector.size();
    }

    //创建一个迭代器对象
    virtual Iterator_ptr<T> creatInterator() {
        return std::make_shared<concreatInterator<T>>(shared_from_this());
    }
};

int main(){
    container_ptr<int> con = std::make_shared<concreatContainer<int>>();
    con->pushItem(1);
    con->pushItem(2);
    con->pushItem(3);

    Iterator_ptr<int> iner = con->creatInterator();
    for (iner->first(); !(iner->isEnd()); iner->next()) {
        std::cout << *(iner->curItem()) << std::endl;
    }

    return 0;
}
