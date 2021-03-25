#include <iostream>
#include <string>

using namespace std;

class SchoolGirl
{
private:
    /* data */
    string name;
public:
    SchoolGirl(){}
    ~SchoolGirl(){}
    string getGirlName() {
        return this->name;
    }

    void setGirlName(string name) {
        this->name = name;
    }
};


//接口
class Interface
{
private:
    /* data */
public:
    Interface(/* args */){}
    virtual ~Interface(){}
    virtual void GiveGift(){}
};

//追求者
class Persuit:public Interface
{
private:
    /* data */
    SchoolGirl mm;
public:
    Persuit(SchoolGirl girl):mm(girl){}
    ~Persuit(){}
    void GiveGift(){
        cout << "追求者送礼物给" << mm.getGirlName() << endl;
    }
};

//代理
class Proxy:public Interface
{
private:
    /* data */
    Persuit* gg;
public:
    Proxy(SchoolGirl mm){
        gg = new Persuit(mm);
    }
    ~Proxy(){
        if (gg) {
            delete gg;
        }
    }
    void GiveGift(){
        gg->GiveGift();
    }
};


int main () {
    SchoolGirl beauty;
    beauty.setGirlName("jiaojiao");

    Proxy* proxy = new Proxy(beauty);
    proxy->GiveGift();

    delete proxy;

    return 0;
}