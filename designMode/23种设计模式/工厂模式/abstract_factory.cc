/*
*抽象工厂模式中我们可以定义实现不止一个接口，一个工厂也可以生成不止一个产品类，抽象工厂模式较好的实现了“开放-封闭”原则，
*是三个模式中较为抽象，并具一般性的模式。我们在使用中要注意使用抽象工厂模式的条件。
*无论是工厂模式增加代码复制度，有没有一种办法，不需要创建工厂，也能解决代码以后变动修改少方法。
*答案是有的，通过（ioc）控制反转，对象在被创建的时候，由一个调控系统内所有对象的外界实体，将其所依赖的对象的引用传递给它。也可以说，(di)依赖被注入到对象中。
*/
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
        cout << "produce dell mouse"<<endl;
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
        cout << "produce thinkPad mouse" <<endl;
    }
};

class KeyBoard
{
private:
    /* data */
public:
    KeyBoard(/* args */){}
    ~KeyBoard(){}
    virtual void GetInfo() {}
};

class DellKeyBoard:public KeyBoard
{
private:
    /* data */
public:
    DellKeyBoard(/* args */){}
    ~DellKeyBoard(){}
    virtual void GetInfo() {
        cout << "produce dell KeyBoard"<<endl;
    }
};

class ThinkPadKeyBoard:public KeyBoard
{
private:
    /* data */
public:
    ThinkPadKeyBoard(/* args */){}
    ~ThinkPadKeyBoard(){}
    virtual void GetInfo() {
        cout << "produce thinkPad KeyBoard" <<endl;
    }
};

class Factory
{
private:
    /* data */
public:
    Factory(/* args */){}
    virtual ~Factory(){}
    virtual Mouse* CreatMouse() {}
    virtual KeyBoard* CreatKeyBoard() {}
};

class DellFactory:public Factory
{
private:
    /* data */
public:
    DellFactory(/* args */){}
    virtual ~DellFactory(){}
    virtual Mouse* CreatMouse() {
        return new DellMouse();
    }
    virtual KeyBoard* CreatKeyBoard() {
        return new DellKeyBoard();
    }
};

class ThinkPadFactory:public Factory
{
private:
    /* data */
public:
    ThinkPadFactory(/* args */){}
    virtual ~ThinkPadFactory(){}
    virtual Mouse* CreatMouse() {
        return new ThinkPadMouse();
    }
    virtual KeyBoard* CreatKeyBoard() {
        return new ThinkPadKeyBoard();
    }
};

int main () {
    Factory* dellFactory = new DellFactory();
    Mouse* dellMouse = dellFactory->CreatMouse();
    KeyBoard* dellKeyBoard = dellFactory->CreatKeyBoard();
    dellMouse->GetInfo();
    dellKeyBoard->GetInfo();
    delete dellKeyBoard;
    delete dellMouse;
    delete dellFactory;

    Factory* thinkPadFactory = new ThinkPadFactory();
    Mouse* thinkPadMouse = thinkPadFactory->CreatMouse();
    KeyBoard* thinkPadKeyBoard = thinkPadFactory->CreatKeyBoard();
    thinkPadMouse->GetInfo();
    thinkPadKeyBoard->GetInfo();
    delete thinkPadKeyBoard;
    delete thinkPadMouse;
    delete thinkPadFactory;

    return 0;
}











