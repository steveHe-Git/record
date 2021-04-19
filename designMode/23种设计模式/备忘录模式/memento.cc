#include <iostream>
#include <vector>
#include <memory>

typedef struct state
{
    int attrackValue;
    int defenceValue;
    int blood;

    state() {
        attrackValue = 10;
        defenceValue = 20;
        blood = 100;
    }
}value_state;

class memento
{
private:
    /* data */
    value_state m_state;
public:
    memento(value_state state):m_state(state){}
    ~memento(){}
    value_state getState() {
        return m_state;
    }
};
typedef std::shared_ptr<memento> memento_ptr;

class mementoOriginal
{
private:
    value_state m_state;
public:
    mementoOriginal(/* args */){}
    ~mementoOriginal(){}

    memento_ptr creatMemento() {
        return std::make_shared<memento>(m_state);
    }

    void fight() {
        m_state.blood -= 10;
    }

    void setMemento(memento_ptr mem) {
        m_state = mem->getState();
    }

    void show() {
        std::cout <<"------------------------------------------------------" << std::endl;
        std::cout << "attrackValue: " << m_state.attrackValue << " defence_value: " << m_state.defenceValue << " blood: " << m_state.blood<< std::endl;
    }
};

class mementoCareTaker
{
private:
    memento_ptr m_memento;
public:
    mementoCareTaker(memento_ptr mem):m_memento(mem){}
    ~mementoCareTaker(){}
    memento_ptr getMemento(){
        return m_memento;
    }
};

int main (){

    auto original = std::make_shared<mementoOriginal>();
    memento_ptr memento = original->creatMemento();
    original->show();
    auto careTaker = std::make_shared<mementoCareTaker>(memento);
    original->fight();
    original->show();

    original->setMemento(careTaker->getMemento());
    original->show();
    return 0;
}