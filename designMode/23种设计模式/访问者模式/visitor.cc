#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Element;
class Visitor;
using VisitorPtr = std::shared_ptr<Visitor>;
using ElementPtr = std::shared_ptr<Element>;

class Element
{
private:
    /* data */
public:
    Element(/* args */){}
    virtual ~Element(){}
    virtual void manAction(VisitorPtr p_visitor) = 0;
    virtual void womanAction(VisitorPtr p_visitor) = 0;
};

class Visitor
{
private:
    /* data */
public:
    Visitor(/* args */){}
    virtual ~Visitor(){}
    virtual void accept(ElementPtr element) = 0;
    virtual string getName() = 0;
};

class ManVisitor:public Visitor, public enable_shared_from_this<ManVisitor>
{
private:
    /* data */
    string m_name;
public:
    ManVisitor(string name):m_name(name){}
    ~ManVisitor(){}
    void accept(ElementPtr element) {
        element->manAction(this->shared_from_this());
    }
    string getName() {
        return m_name;
    }

    void manOperation() {

    }
};

class WomanVisitor:public Visitor, public enable_shared_from_this<WomanVisitor>
{
private:
    /* data */
    string m_name;
public:
    WomanVisitor(string name):m_name(name){}
    ~WomanVisitor(){}
    void accept(ElementPtr element) {
        element->womanAction(this->shared_from_this());
    }
    string getName() {
        return m_name;
    }

    void womanOperation() {
        
    }
};

//成功状态
class ConcreteElementA:public Element
{
private:
    /* data */
    string m_name;
public:
    ConcreteElementA(string name):m_name(name){}
    ~ConcreteElementA(){}
    void manAction(VisitorPtr p_visitor) {
        cout << p_visitor->getName() << m_name << "时，背后总有一个伟大的女人" << endl;
    }

    void womanAction(VisitorPtr p_visitor) {
        cout << p_visitor->getName() << m_name << "时，背后总有一个失败的男人" << endl;
    }
};

//结婚状态
class ConcreteElementB:public Element, public enable_shared_from_this<ConcreteElementB>
{
private:
    /* data */
    string m_name;
public:
    ConcreteElementB(string name):m_name(name){}
    ~ConcreteElementB(){}

    void manAction(VisitorPtr p_visitor) {
        cout << p_visitor->getName() << m_name << "时，感慨道: 恋爱游戏终结时， ‘有妻徒刑’ 遥无期" << endl;
    }
    void womanAction(VisitorPtr p_visitor) {
        cout << p_visitor->getName() << m_name << "时，欣慰曰: 爱情长跑路漫漫， 婚姻保险保平安" << endl;
    }
};

class ObjectStruct
{
private:
    /* data */
    std::vector<VisitorPtr> p_Visitor_vec;
public:
    ObjectStruct(/* args */){}
    ~ObjectStruct(){}
    void attachVisitor(VisitorPtr p_visitor) {
        p_Visitor_vec.push_back(p_visitor);
    }

    void removeVisitor(VisitorPtr p_visitor) {
        for (auto it = p_Visitor_vec.begin(); it != p_Visitor_vec.end(); it++) {
            if (*it == p_visitor) {
                p_Visitor_vec.erase(it);
                break;
            }
        }
    }

    void accept(ElementPtr p_element) {
        for (auto visitor:p_Visitor_vec) {
            visitor->accept(p_element);
        }
    }
};


int main () {
    auto os = std::make_shared<ObjectStruct>();
    auto man = std::make_shared<ManVisitor>("男人");
    auto women = std::make_shared<WomanVisitor>("女人");
    auto succss = std::make_shared<ConcreteElementA>("成功");
    auto marriage = std::make_shared<ConcreteElementB>("结婚");

    os->attachVisitor(man);
    os->attachVisitor(women);

    os->accept(succss);
    os->accept(marriage);

    return 0;
}