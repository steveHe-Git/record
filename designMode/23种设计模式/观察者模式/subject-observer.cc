/*
* 观察者模式
*(标准版本)
* 编译 g++ subject-observer.cc
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

class subject
{
private:
    /* data */
public:
    subject(/* args */){}
    ~subject(){}
    virtual void attachObserver(observer* observer) = 0;
    virtual void detachObserver(observer* observer) = 0;
    virtual void notify() = 0;
};

class creatSubject:public subject
{
private:
    /* data */
    int mStatus;
    std::list<observer*> observerPtrList;
public:
    creatSubject(/* args */){}
    ~creatSubject(){}
    virtual void attachObserver(observer* observer);
    virtual void detachObserver(observer* observer);
    virtual void notify();

    void setstatus(int status) {
        mStatus = status;
    }
};

void creatSubject::attachObserver(observer* observer){
    observerPtrList.push_back(observer);
}

void creatSubject::detachObserver(observer* observer){
    observerPtrList.remove(observer);
}

void creatSubject::notify(){
    std::list<observer*>::iterator it = observerPtrList.begin();
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
    auto publisher = new creatSubject();
    auto subscribe1 = new creatObserver1();
    auto subscribe2 = new creatObserver2();

    publisher->attachObserver(subscribe1);
    publisher->attachObserver(subscribe2);
    publisher->setstatus(1);
    publisher->notify();

    publisher->detachObserver(subscribe1);
    publisher->setstatus(2);
    publisher->notify();

    delete publisher;
    delete subscribe1;
    delete subscribe2;
}






