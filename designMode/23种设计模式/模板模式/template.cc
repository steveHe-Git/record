/*
* 模板模式(行为模式)
* 因为采用了c++ 11标准
* 编译 g++ template.cc -std=c++11
*/

#include <iostream>
#include <list>
#include <memory>
using namespace std;

class people
{
protected:
    virtual void outOfBed(){};
    virtual void inWork(){};
    virtual void offWork(){};
    virtual void goToBed(){};

public:
    void oneDayLife() {
        outOfBed();
        inWork();
        offWork();
        goToBed();
    }
    people(/* args */){}
    ~people(){}
};
typedef std::shared_ptr<people> peoplePtr;

class jack:public people
{
protected:
    virtual void outOfBed(){
        cout << name << "out of bed" << endl;
    }
    virtual void inWork(){
        cout << name << "in work" << endl;
    }
    virtual void offWork(){
        cout << name << "off work" << endl;
    }
    virtual void goToBed(){
        cout << name << "go to bed" << endl;
    }

public:
    std::string name;
    jack(std::string name):name(name){

    }
    ~jack(){}
};

class steve:public people
{
protected:
    virtual void outOfBed(){
        cout << name << " out of bed" << endl;
    }
    virtual void inWork(){
        cout << name << " in work" << endl;
    }
    virtual void offWork(){
        cout << name << " off work" << endl;
    }
    virtual void goToBed(){
        cout << name << " go to bed" << endl;
    }

public:
    std::string name;
    steve(std::string name) {
        this->name = name;
    }
    ~steve(){}
};

int main () {
    peoplePtr jackPtr = std::make_shared<jack>("jack");
    peoplePtr stevePtr = std::make_shared<steve>("steve");
    jackPtr->oneDayLife();
    stevePtr->oneDayLife();

    return 0;
}





