#include <iostream>
#include <memory>

using namespace std;

class Phone
{
private:
    /* data */
public:
    Phone(/* args */){}
    virtual ~Phone(){}
    virtual void ShowAdvantage(){}
};
typedef std::shared_ptr<Phone> PhonePtr;

class XiaoMi:public Phone
{
private:
    /* data */
public:
    XiaoMi(/* args */){}
    ~XiaoMi(){}
    void ShowAdvantage(){
        cout << "XiaoMi 性价比高" <<endl;
    }
};

class HuaWei:public Phone
{
private:
    /* data */
public:
    HuaWei(/* args */){}
    ~HuaWei(){}
    void ShowAdvantage(){
        cout << "HuaWei 像素高" <<endl;
    }
};

//模板实现策略模式
// template<class T>
// class Transform
// {
// private:
//     /* data */
//     T phone;
// public:
//     Transform(){}
//     ~Transform(){}
//     void show() {
//         phone.ShowAdvantage();
//     }
// };

//结合工厂模式
class Transform
{
private:
    Phone* phone = nullptr;
public:
    Transform(string type){
        if (type == "xiaomi") {
            phone = new XiaoMi();
        }  else if (type == "huawei") { 
            phone = new HuaWei();
        }
    }
    void InfoShow() {
        phone->ShowAdvantage();
    }
    ~Transform(){
        if (phone) {
            delete phone;
        }
    }
};




int main () {
    //模板
    // Transform<XiaoMi> transFormMi;
    // transFormMi.show();
    // Transform<HuaWei> transFormHua;
    // transFormHua.show();

    //工厂
    Transform transformMi("xiaomi");
    transformMi.InfoShow();
    Transform transformHua("huawei");
    transformHua.InfoShow();

    return 0;
}