#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Reciver
{
private:
    /* data */
public:
    Reciver(/* args */){}
    ~Reciver(){}
    void Xoperaion() {
        std::cout << "大厨烤鸡翅操作" << std::endl;
    }

    void Yoperaion() {
        std::cout << "大厨烤土豆片操作" << std::endl;
    }
};
using ReciverPtr = std::shared_ptr<Reciver>;

class Command
{
private:
    /* data */
protected:
    ReciverPtr m_rptr;
public:
    Command(ReciverPtr ptr):m_rptr(ptr){}
    virtual ~Command(){}
    virtual void ExecuCommand() = 0;
};
using CommandPtr = std::shared_ptr<Command>;

class CommandX:public Command
{
private:
    /* data */
public:
    CommandX(ReciverPtr ptr):Command(ptr){}
    ~CommandX(){}
    void ExecuCommand(){
        m_rptr->Xoperaion();
    }
};

class CommandY:public Command
{
private:
    /* data */
public:
    CommandY(ReciverPtr ptr):Command(ptr){}
    ~CommandY(){}
    void ExecuCommand(){
        m_rptr->Yoperaion();
    }
};

class Waiter
{
private:
    /* data */
protected:
    std::vector<CommandPtr> ptr_vec;
public:
    Waiter(/* args */){}
    ~Waiter(){}
    void addOrder(CommandPtr ptr) {
        ptr_vec.emplace_back(ptr);
    }

    void cancelOrder(CommandPtr ptr) {
        for (vector<CommandPtr>::iterator it = ptr_vec.begin();  it != ptr_vec.end(); it++) {
            if (ptr == *it) {
                ptr_vec.erase(it);
                break;
            }
        }
    }

    void notify() {
        for (auto com:ptr_vec) {
            com->ExecuCommand();
        }
    }
};

int main () {

    auto reciver = std::make_shared<Reciver>();
    auto waiter = std::make_shared<Waiter>();
    auto commandx = std::make_shared<CommandX>(reciver);
    auto commandy = std::make_shared<CommandY>(reciver);

    waiter->addOrder(commandx);
    waiter->addOrder(commandy);
    waiter->notify();

    std::cout << "---------------------" << std::endl;
    waiter->cancelOrder(commandx);
    waiter->notify();
    return 0;
}