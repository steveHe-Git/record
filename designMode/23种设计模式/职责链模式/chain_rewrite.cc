/*
* 职责链模式(行为模式)
* 因为采用了c++ 11标准
* 编译 g++ template.cc -std=c++11
*/
#include <iostream>
#include <memory>
#include <string>
using namespace std;

typedef enum {
    NOT_HELP_TPOIC,
    GIVE_HELP_TPOIC,
}TOPIC_T;

//QT帮助类
class HandleHelp
{
private:
    HandleHelp* handle_;
    TOPIC_T topic;
public:
    HandleHelp(HandleHelp* h, TOPIC_T t){
        this->handle_ = h;
        this->topic = t;
    }

    void setHandle(HandleHelp* h, TOPIC_T t) {
        this->handle_ = h;
        this->topic = t;
    }

    virtual bool isGiveHelp() {
        return topic != NOT_HELP_TPOIC;
    }

    virtual ~HandleHelp(){}

    virtual void operateHelp() {
        if (handle_) {
            handle_->operateHelp();
        }
    }
};

//QT窗体
class Widget:public HandleHelp
{
private:
    Widget* cur;
public:
    Widget(Widget* w, TOPIC_T t):HandleHelp(w, t){
        this->cur = w;
    }
    virtual ~Widget(){}
};

class Dialog:public Widget
{
private:
    /* data */
public:
    void operateHelp() {
        if (isGiveHelp()) {
            cout << "在Dialog处提供帮助" << endl;
        } else {
            //返回给上一级处理
            //需要指明作用域，否则就是调用自己的HandleHelp，出现死循环
            HandleHelp::operateHelp();
        }
    }

    Dialog(HandleHelp* h, TOPIC_T t = NOT_HELP_TPOIC):Widget(0, t){
        setHandle(h, t);
    }
    ~Dialog(){}
};

class Button:public Widget
{
private:
    /* data */
public:
    Button(Widget* w, TOPIC_T t):Widget(w, t){}
    ~Button(){}
    void operateHelp() {
        if (isGiveHelp()) {
            cout << "在Button处提供帮助" << endl;
        } else {
            //返回给上一级处理
            //需要指明作用域，否则就是调用自己的HandleHelp，出现死循环
            HandleHelp::operateHelp();
        }
    }
};

class Application:public HandleHelp
{
private:
    /* data */
public:
    Application(TOPIC_T t):HandleHelp(nullptr, t){}
    ~Application(){}
    void operateHelp() {
        cout << "在Application处提供帮助" << endl;
    }
};

int main() {
    auto application = new Application(GIVE_HELP_TPOIC);
    auto dialog = new Dialog(application, NOT_HELP_TPOIC);
    auto button = new Button(dialog, NOT_HELP_TPOIC);

    button->operateHelp();
    delete button;
    delete dialog;
    delete application;
    return 0;
}












