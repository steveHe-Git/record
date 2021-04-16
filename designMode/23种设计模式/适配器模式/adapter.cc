#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Player
{
protected:
    /* data */
    std::string m_name;
public:
    Player(std::string name):m_name(name){}
    virtual ~Player(){}
    virtual void defense() {}
    virtual void offence() {}
};
typedef std::shared_ptr<Player> Player_Ptr;

class Coach
{
private:
    std::vector<Player_Ptr>playList;
public:
    Coach(/* args */){}
    ~Coach(){}
    void addPlayer(Player_Ptr player){
        playList.push_back(player);
    }

    void weWillOffence(){
        std::cout << "############# we should offence ###############" << std::endl;
        for(auto man:playList){
            man->offence();
        }
    }

    void weWillDefence(){
        std::cout << "############# we should Defence ###############" << std::endl;
        for(auto man:playList){
            man->defense();
        }
    }
};

class NbaPlayer:public Player
{
private:
    /* data */
public:
    NbaPlayer(std::string name):Player(name){}
    ~NbaPlayer(){}
    virtual void defense() {
        std::cout << m_name << " defense" <<std::endl;
    }
    virtual void offence() {
        std::cout << m_name << " offence" <<std::endl;
    }
};

class ForeignPlayer
{
private:
    std::string m_name;
public:
    ForeignPlayer(std::string name):m_name(name){}
    ~ForeignPlayer(){}
    void defense() {
        std::cout << m_name << " defense" <<std::endl;
    }
    void offence() {
        std::cout << m_name << " offence" <<std::endl;
    }
};

class Translator:public Player
{
private:
    std::shared_ptr<ForeignPlayer>ym;
public:
    Translator(std::string name):Player(name){
        ym = std::make_shared<ForeignPlayer>(name);
    }
    ~Translator(){}
    virtual void defense() {
        ym->defense();
    }
    virtual void offence() {
        ym->offence();
    }
};


int main ()
{
    Coach Bobo;
    Player_Ptr maiDi = std::make_shared<NbaPlayer>("MaiDi");
    Player_Ptr kobe = std::make_shared<NbaPlayer>("kobe");
    Player_Ptr translator = std::make_shared<Translator>("yaoMing");
    Bobo.addPlayer(maiDi);
    Bobo.addPlayer(kobe);
    Bobo.addPlayer(translator);

    Bobo.weWillDefence();
    Bobo.weWillOffence();
    return 0;
}