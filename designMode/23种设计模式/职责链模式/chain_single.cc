/*
* 职责链模式(行为模式)
* 因为采用了c++ 11标准(只处理单一的请求，并不是每个人都需要处理，一次处理之后就结束)
* 编译 g++ template.cc -std=c++11
*/

#include <iostream>
#include <memory>
#include <string>
using namespace std;

namespace ChainOfResponsibility
{
    class ChainHandle;
    typedef std::shared_ptr<ChainHandle> ChainHandlePtr;

    class ChainHandle
    {
    protected:
        ChainHandlePtr handle;
    public:
        ChainHandle(/* args */){}
        virtual ~ChainHandle(){}
        void setChainHandle(ChainHandlePtr handle) {
            this->handle = handle;
        }

        virtual void HandleRequest(string request) {}
    };

    class ConcreatHandleA:public ChainHandle
    {
    private:
        /* data */
    public:
        ConcreatHandleA(/* args */){}
        ~ConcreatHandleA(){}
        void HandleRequest(string request) {
            if (request == "A") {
                //满足条件，处理请求
                cout << "handle request in ConcreatHandleA" << endl;
            } else {
                //不满足条件
                handle->HandleRequest(request);
            }
        }
    };

    class ConcreatHandleB:public ChainHandle
    {
    private:
        /* data */
    public:
        ConcreatHandleB(/* args */){}
        ~ConcreatHandleB(){}
        void HandleRequest(string request) {
            if (request == "B") {
                //满足条件，处理请求
                cout << "handle request in ConcreatHandleB" << endl;
            } else {
                //不满足条件
                handle->HandleRequest(request);
            }
        }
    };

    class ConcreatHandleC:public ChainHandle
    {
    private:
        /* data */
    public:
        ConcreatHandleC(/* args */){}
        ~ConcreatHandleC(){}
        void HandleRequest(string request) {
            if (request == "C") {
                //满足条件，处理请求
                cout << "handle request in ConcreatHandleC" << endl;
            } else {
                //不满足条件
                handle->HandleRequest(request);
            }
        }
    };

    class ClientHandle
    {
    private:
        /* data */
    public:
        ClientHandle(/* args */){}
        ~ClientHandle(){}
        void HandleInterface(string request) {
            auto handleA = std::make_shared<ConcreatHandleA>();
            auto handleB = std::make_shared<ConcreatHandleB>();
            auto handleC = std::make_shared<ConcreatHandleC>();

            handleC->setChainHandle(handleB);
            handleB->setChainHandle(handleA);
            
            handleC->HandleRequest(request);
        }
    };
        
} //namespace ChainOfResponsibility

int main () {
    ChainOfResponsibility::ClientHandle client;
    client.HandleInterface("C");

    return 0;
}