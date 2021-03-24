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
    ~Mouse(){}
    virtual void GetInfo() {}
};

class DellMouse:public Mouse
{
private:
    /* data */
public:
    DellMouse(/* args */){}
    ~DellMouse(){}
    virtual void GetInfo() {
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
    virtual void GetInfo() {
        cout << "thinkPad mouse" <<endl;
    }
};

class Factory
{
private:
    /* data */
public:
    Factory(/* args */){}
    ~Factory(){}
    Mouse* produceMouse(string name) {
        Mouse* mouse = nullptr;
        if (name == "dell") {
            mouse = new DellMouse();
        } else if (name == "thinkPad") {
            mouse = new ThinkPadMouse();
        }
        return mouse;
    }
};

int main () {
    Factory factory;
    Mouse* dellMouse = factory.produceMouse("dell");
    Mouse* thinkPadMouse = factory.produceMouse("thinkPad");
    dellMouse->GetInfo();
    thinkPadMouse->GetInfo();

    delete dellMouse;
    delete thinkPadMouse;
    return 0;
}











