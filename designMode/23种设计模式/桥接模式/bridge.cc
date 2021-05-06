#include <iostream>
#include <memory>
using namespace std;

class Implementor
{
private:
    /* data */
public:
    Implementor(/* args */){}
    virtual ~Implementor(){}
    virtual void operation() = 0;
};
using ImplementorPtr = std::shared_ptr<Implementor>;

class ImplementorA:public Implementor
{
private:
    /* data */
public:
    ImplementorA(/* args */){}
    ~ImplementorA(){}
    void operation() {
        cout << "A operation" << endl;;
    }
};

class ImplementorB:public Implementor
{
private:
    /* data */
public:
    ImplementorB(/* args */){}
    ~ImplementorB(){}
    void operation() {
        cout << "B operation" << endl;;
    }
};

class Abstraction
{
protected:
    ImplementorPtr m_implementor;
public:
    Abstraction(/* args */){}
    ~Abstraction(){}
    virtual void setImplementor(ImplementorPtr ptr) {
        m_implementor = ptr;
    }

    virtual void operation() {
        cout << "..." << endl;
    }
};

class AbstarctA:public Abstraction
{
private:
    /* data */
public:
    AbstarctA(/* args */){}
    ~AbstarctA(){}

    void operation() {
        m_implementor->operation();
    }
};

class AbstarctB:public Abstraction
{
private:
    /* data */
public:
    AbstarctB(/* args */){}
    ~AbstarctB(){}
    void operation() {
        m_implementor->operation();
    }
};

int main () {
    auto abstract = std::make_shared<Abstraction>();
    auto abstractA = std::make_shared<AbstarctA>();
    abstractA->setImplementor(std::make_shared<ImplementorA>());
    abstractA->operation();
    abstractA->setImplementor(std::make_shared<ImplementorB>());
    abstractA->operation();

    auto abstractB = std::make_shared<AbstarctB>();
    abstractB->setImplementor(std::make_shared<ImplementorA>());
    abstractB->operation();
    abstractB->setImplementor(std::make_shared<ImplementorB>());
    abstractB->operation();
    return 0;
}