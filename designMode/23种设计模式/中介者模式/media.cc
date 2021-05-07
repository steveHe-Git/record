#include <iostream>
#include <memory>
#include <string>
using namespace std;

class Colleage;
class Media
{
private:
    /* data */
public:
    Media(/* args */){}
    virtual ~Media(){}
    virtual void send(string message, Colleage* coll){}
};


class Colleage
{
protected:
    /* data */
    Media* p_media;
public:
    Colleage(Media* ptr):p_media(ptr){}
    virtual ~Colleage(){}
    virtual void send(string message) = 0;
    virtual void notify(string message) = 0;
};

class ConcreteColleageA:public Colleage
{
private:
    /* data */
public:
    ConcreteColleageA(Media* ptr):Colleage(ptr){}
    ~ConcreteColleageA(){}
    void send(string message) {
        p_media->send(message, this);
    }

    void notify(string message) {
        cout << "ConcreteColleageA recieve message: " << message <<endl;
    }
};

class ConcreteColleageB:public Colleage
{
private:
    /* data */
public:
    ConcreteColleageB(Media* ptr):Colleage(ptr){}
    ~ConcreteColleageB(){}
    void send(string message) {
        p_media->send(message, this);
    }

    void notify(string message) {
        cout << "ConcreteColleageB recieve message: " << message <<endl;
    }
};

class ConcreteMedia:public Media
{
private:
    /* data */
public:
    ConcreteColleageA* colleageA;
    ConcreteColleageB* colleageB;
    ConcreteMedia(/* args */){}
    ~ConcreteMedia(){}
    void send(string message, Colleage* coll) {
        if (colleageA == dynamic_cast<ConcreteColleageA*>(coll)) {
            colleageB->notify(message);
        } else if (colleageB == dynamic_cast<ConcreteColleageB*>(coll)) {
            colleageA->notify(message);
        }
    }
};

int main () {
    auto m = new ConcreteMedia();
    auto colleageA = new ConcreteColleageA(m);
    auto colleageB = new ConcreteColleageB(m);

    m->colleageA = colleageA;
    m->colleageB = colleageB;

    colleageA->send("吃饭没有");
    colleageB->send("吃完了");

    delete m;
    delete colleageA;
    delete colleageB;
    return 0;
}



