/*
* 观察者模式
* 因为采用了c++ 11标准
* 编译 g++ subject-observer.cc -std=c++11
*/

#include <iostream>
#include <list>
#include <memory>
using namespace std;

class observer
{
private:
    /* data */
public:
    observer(/* args */){}
    ~observer(){}
    virtual void update(int state) = 0;
};
typedef std::shared_ptr<observer> observerPtr;

class subject
{
private:
    /* data */
public:
    subject(/* args */){}
    ~subject(){}
    virtual void attachObserver(observerPtr observer) = 0;
    virtual void detachObserver(observerPtr observer) = 0;
    virtual void notify() = 0;
};

class creatSubject:public subject
{
private:
    /* data */
    int mStatus;
    std::list<observerPtr> observerPtrList;
public:
    creatSubject(/* args */){}
    ~creatSubject(){}
    virtual void attachObserver(observerPtr observer);
    virtual void detachObserver(observerPtr observer);
    virtual void notify();

    void setstatus(int status) {
        mStatus = status;
    }
};

void creatSubject::attachObserver(observerPtr observer){
    observerPtrList.push_back(observer);
}

void creatSubject::detachObserver(observerPtr observer){
    observerPtrList.remove(observer);
}

void creatSubject::notify(){
    std::list<observerPtr>::iterator it = observerPtrList.begin();
    while (it != observerPtrList.end()) {
        (*it)->update(mStatus);
        it++;
    }
}

class creatObserver1:public observer
{
private:
    /* data */
public:
    creatObserver1(/* args */){}
    ~creatObserver1(){}
    virtual void update(int state) {
        std::cout << "creatObserver1 update status: " << state << endl;
    }
};

class creatObserver2:public observer
{
private:
    /* data */
public:
    creatObserver2(/* args */){}
    ~creatObserver2(){}
    virtual void update(int state) {
        std::cout << "creatObserver2 update status: " << state << endl;
    }
};

int main () {
    
    /*采用智能指针管理内存，推荐使用这种方法*/
    auto publisher = std::make_shared<creatSubject>();
    auto subscribe1 = std::make_shared<creatObserver1>();
    auto subscribe2 = std::make_shared<creatObserver2>();

    publisher->attachObserver(subscribe1);
    publisher->attachObserver(subscribe2);
    publisher->setstatus(1);
    publisher->notify();

    publisher->detachObserver(subscribe1);
    publisher->setstatus(2);
    publisher->notify();
}






