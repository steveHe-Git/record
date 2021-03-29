/*
* 功能链模式(行为模式)(请求逐次传递处理，直到最后一个处理者)
* 根据员工的级别不同工资的计算方式也不同
* 前提是所有人基本工资，补助，奖金一样
* 领导 基本工资+ 项目奖 + 补助 + 奖金
* 组长 基本工资 + 补助 + 奖金
* 领导 基本工资 + 补助
* 因为采用了c++ 11标准
* 编译 g++ template.cc -std=c++11
*/
#include <iostream>
#include <memory>
using namespace std;

typedef enum{
    EMPLOYEE_ORIGIN,
    EMPLOYEE_TEAM_LEADER,
    EMPLOYEE_GROUP_LEADER
}EmployeeType;

typedef int Money; 
Money BaseMoney     = 3000;
Money SupplyMoney   = 2000;
Money BonusMoney    = 5000;
Money ProjectMoney  = 8000;

string NameString[] = {"普通员工", "组长", "领导"};

class Handle;
typedef std::shared_ptr<Handle>HandlePtr;

class Handle
{
protected:
    HandlePtr handle;
public:
    Handle(/* args */){}
    virtual ~Handle(){}
    void setHandle (HandlePtr handle) {
        this->handle = handle;
    }
    virtual int CalculateSalary(EmployeeType type) {}
};

//基本工资
class BaseHandle:public Handle
{
private:
    /* data */
public:
    BaseHandle(/* args */){}
    ~BaseHandle(){}
    int CalculateSalary(EmployeeType type) {
        if (type == EMPLOYEE_ORIGIN) {
            return BaseMoney + SupplyMoney;
        } else {
            return handle->CalculateSalary(type);
        }
    }
};

//项目奖励
class ProjectAwardHandle:public Handle
{
private:
    /* data */
public:
    ProjectAwardHandle(/* args */){}
    ~ProjectAwardHandle(){}
    int CalculateSalary(EmployeeType type) {
        if (EMPLOYEE_GROUP_LEADER == type) {
            return BaseMoney + SupplyMoney + BonusMoney + ProjectMoney;
        } else {
            return handle->CalculateSalary(type);
        }
    }
};

//奖金
class BonusHandle:public Handle
{
private:
    /* data */
public:
    BonusHandle(/* args */){}
    ~BonusHandle(){}
    int CalculateSalary(EmployeeType type) {
        if (EMPLOYEE_TEAM_LEADER == type) {
            return BaseMoney + SupplyMoney + BonusMoney;
        } else {
            return handle->CalculateSalary(type);
        }
    }
};

class ClientSalary
{
private:
    /* data */
public:
    ClientSalary(/* args */){}
    ~ClientSalary(){}
    int getSalay(EmployeeType type) {
        auto baseHandle = std::make_shared<BaseHandle>();
        auto bonusHandle = std::make_shared<BonusHandle>();
        auto projectAwardHandle = std::make_shared<ProjectAwardHandle>();

        baseHandle->setHandle(bonusHandle);
        bonusHandle->setHandle(projectAwardHandle);

        cout << NameString[(int)type];
        return baseHandle->CalculateSalary(type);
    }
};

int main () {
    ClientSalary client;
    cout << "获得的工资为: " << client.getSalay(EMPLOYEE_TEAM_LEADER) <<endl;
    return 0;
}





