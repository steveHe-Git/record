#include <iostream>
#include <memory>
using namespace std;

class SubSystemOne
{
private:
    /* data */
public:
    SubSystemOne(/* args */){}
    ~SubSystemOne(){}
    void MethodOne(){
        cout << "call subsystem method one" << endl;
    }
};

class SubSystemTwo
{
private:
    /* data */
public:
    SubSystemTwo(/* args */){}
    ~SubSystemTwo(){}
    void MethodTwo(){
        cout << "call subsystem method two" << endl;
    }
};

class SubSystemThree
{
private:
    /* data */
public:
    SubSystemThree(/* args */){}
    ~SubSystemThree(){}
    void MethodThree(){
        cout << "call subsystem method three" << endl;
    }
};

class SubSystemFour
{
private:
    /* data */
public:
    SubSystemFour(/* args */){}
    ~SubSystemFour(){}
    void MethodFour(){
        cout << "call subsystem method four" << endl;
    }
};

class FacadeSimple
{
private:
    SubSystemOne subSystemOne;
    SubSystemTwo subSystemTwo;
    SubSystemThree subSystemThree;
    SubSystemFour subSystemFour;
public:
    FacadeSimple(/* args */){}
    ~FacadeSimple(){}

    void MethodA() {
        cout << "-------------call A method----------" <<endl;
        subSystemOne.MethodOne();
    }

    void MethodB() {
        cout << "-------------call B method----------" <<endl;
        subSystemFour.MethodFour();
        subSystemOne.MethodOne();
    }

    void MethodC() {
        cout << "-------------call C method----------" <<endl;
        subSystemThree.MethodThree();
        subSystemTwo.MethodTwo();
        subSystemOne.MethodOne();
    }

    void MethodD() {
        cout << "-------------call D method----------" <<endl;
        subSystemOne.MethodOne();
        subSystemFour.MethodFour();
    }
};


int main () {
    auto facade = std::make_shared<FacadeSimple>();
    facade->MethodA();
    facade->MethodB();
    facade->MethodC();
    facade->MethodD();

    return 0;
}