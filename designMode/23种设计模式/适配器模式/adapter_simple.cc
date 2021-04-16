#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Target
{
private:
    /* data */
public:
    Target(/* args */){}
    virtual ~Target(){}
    virtual void request(){
        std::cout << "target's request" << std::endl;
    }
};

/*需要适配的类型*/
class adaptee
{
private:
    /* data */
public:
    adaptee(/* args */){}
    ~adaptee(){}
    void specificRequest(){
        std::cout << "adaptee's request" << std::endl;
    }
};

class adapter:public Target
{
private:
    /* data */
    adaptee* ad;
public:
    adapter(/* args */){
        ad = new adaptee();
    }
    ~adapter(){
        if (ad) {
            delete ad;
            ad = nullptr;
        }
    }
    void request(){
        ad->specificRequest();
    }
};

int main() {
    Target* target = new adapter();
    target->request();

    delete target;
    return 0;
}