#include <iostream>
#include <memory>
#include <string>
#include <map>
using namespace std;

class UserInfo
{
private:
    /* data */
    string m_name;
public:
    UserInfo(string name):m_name(name){}
    ~UserInfo(){}
    string getUserName() {
        return m_name;
    }
};
using UserPtr = std::shared_ptr<UserInfo>;

class Flyweight
{
private:
    /* data */
public:
    Flyweight(/* args */){}
    virtual ~Flyweight(){}
    virtual void showWebInfo(UserPtr user) = 0;
};
using FlyweightPtr = std::shared_ptr<Flyweight>;

class ConcreteFlyweight:public Flyweight
{
private:
    /* data */
    string m_name;
public:
    ConcreteFlyweight(string name):m_name(name){}
    ~ConcreteFlyweight(){}
    void showWebInfo(UserPtr user) {
        cout << "网站展示: " << m_name << "  用户: " << user->getUserName()<<endl;
    }
};

class unsharedFlyweight:public Flyweight
{
private:
    string m_name;
public:
    unsharedFlyweight(string name):m_name(name){}
    ~unsharedFlyweight(){}
    void showWebInfo(UserPtr user) {
        cout << "网站展示: " << m_name << "  用户: " << user->getUserName() <<endl;
    }
};

class FlyweightFactory
{
private:
    /* data */
    std::map<std::string, FlyweightPtr> map_factory;
public:
    FlyweightFactory(/* args */){}
    ~FlyweightFactory(){}
    FlyweightPtr getSharedWeb(string key, string mode) {
        if (map_factory.find(key) != map_factory.end()) {
            return map_factory[key];
        }

        if (mode == "shared") {
            map_factory.emplace(key, std::make_shared<ConcreteFlyweight>(key));
        } else if (mode == "unshared") {
            map_factory.emplace(key, std::make_shared<unsharedFlyweight>(key));
        }
        return map_factory[key];
    }

    int getCount() {
        return map_factory.size();
    }
};

int main () {
    auto webFactory = std::make_shared<FlyweightFactory>();
    auto webx = webFactory->getSharedWeb("产品展示", "shared");
    webx->showWebInfo(std::make_shared<UserInfo>("大鸟"));
    auto webY = webFactory->getSharedWeb("产品展示", "shared");
    webY->showWebInfo(std::make_shared<UserInfo>("小菜"));
    auto webp = webFactory->getSharedWeb("产品展示扩展", "unshared");
    webp->showWebInfo(std::make_shared<UserInfo>("小菜"));

    cout << "------------------" <<endl;
    auto webZ = webFactory->getSharedWeb("博客", "shared");
    webZ->showWebInfo(std::make_shared<UserInfo>("大鸟"));
    auto webH = webFactory->getSharedWeb("博客", "shared");
    webH->showWebInfo(std::make_shared<UserInfo>("小菜"));
    auto webl = webFactory->getSharedWeb("博客扩展", "unshared");
    webl->showWebInfo(std::make_shared<UserInfo>("小菜"));

    cout << "------------------" <<endl;
    cout << "web factory size is: " << webFactory->getCount() <<endl;

    return 0;
}