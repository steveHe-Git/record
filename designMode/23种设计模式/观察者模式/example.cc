/*
* 观察者模式
* 因为采用了c++ 11标准
* 编译 g++ example.cc -std=c++11
*/

#include <iostream>
#include <list>
#include <memory>
using namespace std;

class observer
{
protected:
    std::string name;
public:
    observer(std::string name){
        this->name = name;
    }
    ~observer(){}
    virtual void update(std::string event) = 0;
};
typedef std::shared_ptr<observer> observerPtr;

class watchTvSubcribe:public observer
{
private:
    /* data */
public:
    watchTvSubcribe(std::string action_name) :observer(action_name){}
    ~watchTvSubcribe(){}
    void update(std::string event) {
        cout << name <<" recieve event->"<< event <<endl;
        if (event == "Boss") {
            cout << event <<" arrived," << name << " stop watching TV" << endl;
        } else if (event == "Leave") {
            cout << event <<" leave," << name << " keep on watching TV" << endl;
        }
    }
};

class palyGameSubscribe:public observer
{
private:
    /* data */
public:
    palyGameSubscribe(std::string action_name):observer(action_name){}
    ~palyGameSubscribe(){}
    void update(std::string event) {
        cout << name <<" recieve event->"<< event <<endl;
        if (event == "Boss") {
            cout << event <<" arrived," << name << " stop playing Games" << endl;
        }
    }
};

class readBookSubscribe:public observer
{
private:
    /* data */
public:
    readBookSubscribe(std::string action_name):observer(action_name){}
    ~readBookSubscribe(){}
    void update(std::string event) {
        cout << name <<" recieve event->"<< event <<endl;
        if (event == "Boss") {
            cout << event <<" arrived," << name << " stop reading book" << endl;
        }
    }
};

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

class publishSubject:public subject
{
private:
    /* data */
    std::string event;
    std::list<observerPtr> observerPtrList;
public:
    publishSubject(/* args */){}
    ~publishSubject(){}
    virtual void attachObserver(observerPtr observer);
    virtual void detachObserver(observerPtr observer);
    virtual void notify();

    void updateEvent(std::string e){
        event = e;
    }
};

void publishSubject::attachObserver(observerPtr observer){
    observerPtrList.push_back(observer);
}

void publishSubject::detachObserver(observerPtr observer){
    std::list<observerPtr>::iterator it = observerPtrList.begin();
    while(it != observerPtrList.end()){
        if((*it) == observer) {
            /*特别注意erase的返回值是一个指向下一个元素的迭代器指针*/
            it = observerPtrList.erase(it);
        } else {
            ++it;
        }
    }
}

void publishSubject::notify(){
    std::list<observerPtr>::iterator it = observerPtrList.begin();
    while (it != observerPtrList.end()) {
        (*it)->update(event);
        ++it;
    }
}

int main() {
    /*采用智能指针管理内存，推荐使用这种方法*/
    auto publisher = std::make_shared<publishSubject>();
    auto subscribe1 = std::make_shared<watchTvSubcribe>("jack");
    auto subscribe2 = std::make_shared<palyGameSubscribe>("tom");
    auto subscribe3 = std::make_shared<readBookSubscribe>("steve");

    publisher->attachObserver(subscribe1);
    publisher->attachObserver(subscribe2);
    publisher->attachObserver(subscribe3);
    publisher->updateEvent("Boss");
    publisher->notify();

    publisher->detachObserver(subscribe1);
    publisher->updateEvent("Boss");
    publisher->notify();
}

