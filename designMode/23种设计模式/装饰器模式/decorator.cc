#include <iostream>
#include <memory>

using namespace std;

class Interface
{
private:
    /* data */
public:
    Interface(/* args */){}
    virtual ~Interface(){}
    //鸡蛋饼
    virtual void show() {}
};
typedef std::shared_ptr<Interface> InterfacePtr;

class OriginEggPile:public Interface
{
private:
    /* data */
public:
    OriginEggPile(/* args */){}
    ~OriginEggPile(){}
    void show() {
        cout << "鸡蛋饼: 鸡蛋 + 饼" << endl;
    }
};

class Decorator:public Interface
{
private:
    /* data */
public:
    Decorator(){}
    virtual ~Decorator(){}
    virtual void show(){}
    virtual void newRequire() {}
};

//加蔬菜
class VegeDecorator:public Decorator
{
private:
    /* data */
    InterfacePtr base;
public:
    VegeDecorator(InterfacePtr interface){
        this->base = interface;
    }
    virtual ~VegeDecorator(){}
    void show() {
        base->show();
        newRequire();
    }

    void newRequire() {
        cout << "额外在加一点蔬菜"<<endl;
    }
};

//加培根
class BaconDecorator:public Decorator
{
private:
    /* data */
    InterfacePtr base;
public:
    BaconDecorator(InterfacePtr interface){
        this->base = interface;
    }
    virtual ~BaconDecorator(){}
    virtual void show() {
        base->show();
        newRequire();
    }

    void newRequire() {
        cout << "额外在加一点培根"<<endl;
    }
};

int main () {
    auto interface = std::make_shared<OriginEggPile>();
    interface->show();
    cout <<"--------------------" << endl;

    auto vegeAdd = std::make_shared<VegeDecorator>(interface);
    vegeAdd->show();
    cout <<"--------------------" << endl;

    auto baconAdd = std::make_shared<BaconDecorator>(interface);
    baconAdd->show();
    cout <<"--------------------" << endl;

    return 0;
}