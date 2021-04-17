#include <iostream>
#include <memory>
#include <string>
using namespace std;

class work;
class state
{
private:
    /* data */
public:
    state(/* args */){}
    virtual ~state(){}
    virtual void function(work*){}
};
typedef std::shared_ptr<state> state_ptr;

class work
{
private:
    state_ptr m_state;
    int time;
    bool taskFinish;
public:
    int& getTime(){
        return time;
    }

    bool& isTaskFinish(){
        return taskFinish;
    }

    work(/* args */){
        time = 8;
        taskFinish = false;
    }

    ~work(){}

    void function(){
        if (m_state) {
            m_state->function(this);
        }
    }

    void setHandleState(state_ptr state) {
        m_state = state;
    }
};

class offWorkState:public state
{
private:
    /* data */
public:
    offWorkState(/* args */){}
    ~offWorkState(){}
    void function(work* ptr) { 
        std::cout << "now is time to off work! yeah" << "  time:" << ptr->getTime() <<std::endl;
    }
};

class workingState:public state
{
private:
    /* data */
public:
    workingState(/* args */){}
    ~workingState(){}
    void function(work* ptr) { 
        if (ptr->getTime() >= 9 && ptr->getTime() < 20) {
            std::cout << "coding & debuging !!!!" << "  time:" << ptr->getTime()<<std::endl;
        } else if (ptr->getTime() >= 20) {
            ptr->setHandleState(std::make_shared<offWorkState>());
            ptr->function();
        }
    }
};

class goToWorkState:public state
{
private:
    /* data */
public:
    goToWorkState(/* args */){}
    ~goToWorkState(){}
    void function(work* ptr) {
        if (ptr->getTime() >= 8 && ptr->getTime() < 9) {
            std::cout << " prepared go to working " << "  time:" << ptr->getTime()<<std::endl;
        } else {
            ptr->setHandleState(std::make_shared<workingState>());
            ptr->function();
        }
    }
};

int main()
{
    auto m_work = new work();
    m_work->setHandleState(std::make_shared<goToWorkState>());
    m_work->getTime() = 8;
    m_work->function();
    m_work->getTime() = 12;
    m_work->function();
    m_work->getTime() = 21;
    m_work->function();

    return 0;
}