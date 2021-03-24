#include <iostream>
#include <memory>
#include <string>
using namespace std;

class Mouse
{
private:
    /* data */
public:
    Mouse(/* args */){}
    virtual ~Mouse(){}
    virtual void GetInfo() {}
};

class DellMouse:public Mouse
{
private:
    /* data */
public:
    DellMouse(/* args */){}
    ~DellMouse(){}
    void GetInfo() {
        cout << "dell mouse"<<endl;
    }
};

class ThinkPadMouse:public Mouse
{
private:
    /* data */
public:
    ThinkPadMouse(/* args */){}
    ~ThinkPadMouse(){}
    void GetInfo() {
        cout << "thinkPad mouse" <<endl;
    }
};

class Factory
{
private:
    /* data */
public:
    Factory(/* args */){}
    virtual ~Factory(){}
    virtual Mouse* GetMouse() {}
};

class DellFactory:public Factory
{
private:
    /* data */
public:
    DellFactory(/* args */){}
    ~DellFactory(){}
    Mouse* GetMouse() {
        return new DellMouse();
    }
};

class ThinkPadFactory:public Factory
{
private:
    /* data */
public:
    ThinkPadFactory(/* args */){}
    ~ThinkPadFactory(){}
    Mouse* GetMouse() {
        return new ThinkPadMouse();
    }
};

int main () {
    Factory* dellFactory = new DellFactory();
    Factory* thinkPadFactory = new ThinkPadFactory();
    Mouse* dellMouse = dellFactory->GetMouse();
    Mouse* thinkPadMouse = thinkPadFactory->GetMouse();

    dellMouse->GetInfo();
    thinkPadMouse->GetInfo();

    delete dellMouse;
    delete thinkPadMouse;
    delete thinkPadFactory;
    delete dellFactory;
    return 0;
}











