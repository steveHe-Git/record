/*想必很多人都玩过魔兽争霸3。里面有一个非常厉害的英雄叫做剑圣。
*这个英雄攻击力算是最高的，而且有个听起来很叼的魔法，“剑刃风暴”，这个魔法其实效果一般，不大实用。
*剑圣有一个分身的功能。这个分身各个方面和剑圣都一模一样，但是没有攻击力，也没有魔法。
使用原型模式理由有三：
1 这个分身是游戏过程中动态生成的。剑圣使用了这个功能就动态生成分身。
2 这个分身几乎就是剑圣的拷贝。如果重新new一个剑圣不符合游戏规则。玩过War3的人都知道一个种族只能有一个剑圣吧，如果可以有3个剑圣，让其它族怎么玩。
3 重新创建一个剑圣很麻烦，要初始化各种参数，如等级啊，攻击力啊，等等各个参数，而且这些参数要和原型一致。直接通过原型模式实现则简单。
4 便于扩展。如果分身有30%原剑圣的攻击力，那么我们直接修改Clone方法即可。对外的接口不用变更。
*/
#include <iostream>
#include <string>
using namespace std;

//英雄类
class Hero
{
private:
    int Damage;
    string Magic;
public:
    Hero(int Damage, string Magic){
        SetDamage(Damage);
        SetMagic(Magic);
    }

    Hero(const Hero& hero) {
        cout << "#hero construct" <<endl;
        this->Damage = hero.Damage;
        this->Magic = hero.Magic;
    }
    virtual ~Hero() {
        
    }

    void SetDamage(int Damage) {
        this->Damage = Damage;
    }

    void SetMagic(string Magic) {
        this->Magic = Magic;
    }

    //进攻
    void Attach(){
		cout << Magic <<" "<< Damage << endl;
	}
    
    virtual Hero* clone(){return nullptr;}
};

//剑圣
class Bm:public Hero
{
private:
    /* data */
public:
    Bm(int Damage, string Magic):Hero(Damage, Magic){}
    Bm(const Bm& bm):Hero(bm){}//拷贝构造函数
    ~Bm(){}

    Hero* clone() {
        Bm* bm = new Bm(*this);//调用拷贝构造函数
        bm->SetDamage(0);
        bm->SetMagic("no magic");
        return bm;
    }
};

int main () {
    Hero* bm = new Bm(98, "剑刃风暴");
    bm->Attach();

    Hero* bm_clone = bm->clone();
    bm_clone->Attach();

    delete bm;
    delete bm_clone;

    return 0;
}


