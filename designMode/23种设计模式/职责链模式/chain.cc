/*
* 职责链模式(行为模式)
* 因为采用了c++ 11标准
* 编译 g++ template.cc -std=c++11
*/

#include <iostream>
#include <list>
#include <memory>
using namespace std;

typedef int TOPIC;
const TOPIC NO_HELP_TOPIC = -1;

class HelpHandler
{
private:
    HelpHandler* handler_;
    TOPIC topic_;

public:
    virtual bool HasHelp(){
        return topic_ != NO_HELP_TOPIC;
    }

    virtual void SetHandler(HelpHandler* h, TOPIC t){
        handler_ = h;
        topic_ = t;
    }

    virtual void HandleHelp() {
        if (handler_) {
            handler_->HandleHelp();
        }
    }
    //两种表示方式
    //HelpHandler(HelpHandler* = 0, TOPIC = NO_HELP_TOPIC)
    //HelpHandler(HelpHandler* h, TOPIC t):handler_(h), topic_(t){}
    HelpHandler(HelpHandler* h = 0, TOPIC t = NO_HELP_TOPIC){
        handler_ = h;
        topic_ = t;
    }
    ~HelpHandler(){}
};

class Widget:public HelpHandler
{
private:
    Widget* parent_;

protected:
    Widget(Widget* w = 0, TOPIC t = NO_HELP_TOPIC):HelpHandler(w, t){
        parent_ = w;
    }
    ~Widget(){}
};

class Button:public Widget
{
private:
    /* data */
public:
    virtual void HandleHelp(){
        if (HasHelp()) {
            //提供帮助
            cout << "Button doing..." <<endl;
        } else {
            HelpHandler::HandleHelp();
        }
    }
    Button(Widget* w = 0, TOPIC t = NO_HELP_TOPIC):Widget(w, t){}
    ~Button(){}
};

class Dialog:public Widget
{
private:
    /* data */
public:
    virtual void HandleHelp(){
        if (HasHelp()) {
            //提供帮助
            cout << "Dialog doing..." <<endl;
        } else {
            HelpHandler::HandleHelp();
        }
    }
    Dialog(HelpHandler* h, TOPIC t = NO_HELP_TOPIC):Widget(0){
        SetHandler(h, t);
    }
    ~Dialog(){}
};

class Application:public HelpHandler
{
private:
    /* data */
public:
    Application(TOPIC t):HelpHandler(0, t){}
    virtual void HandleHelp(){
        cout << "Application doing..." <<endl;
    }   
    ~Application(){}
};

int main () {
    const TOPIC NO_HANDLE = -1;
    const TOPIC PRINT_TOPIC = 1;
    const TOPIC PAPER_TOPIC = 2;
    const TOPIC APPLICATION_TOPIC = 3;

    Application* application = new Application(APPLICATION_TOPIC);
    Dialog* dialog = new Dialog(application, NO_HANDLE);
    Button* button = new Button(dialog, NO_HANDLE);

    button->HandleHelp();
}





