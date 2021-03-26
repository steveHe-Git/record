#include <iostream>
#include <memory>

using namespace std;

typedef enum {
    FEMALE,
    MALE,
}SEX_TYPE;

class EmployeeBaseInfo
{
private:
    string      EmployeeName;
    SEX_TYPE    EmployeeSex;
    uint32_t    EmployeeAge;
    uint32_t    EmployeeId;
public:
    EmployeeBaseInfo(/* args */):
        EmployeeName(""),
        EmployeeSex(FEMALE),
        EmployeeAge(0),
        EmployeeId(0){}

    ~EmployeeBaseInfo(){}
};

class EmployeeSalary
{
private:
    float           EmployeeBaseSalary;
    float           EmployeeBonus;
    float           EmployeeFullBonus;
    uint32_t        EmployeeId;
public:
    EmployeeSalary(/* args */):
        EmployeeBaseSalary(0),
        EmployeeBonus(0),
        EmployeeFullBonus(0),
        EmployeeId(0){}

    ~EmployeeSalary(){}
};

class EmployeeInfo
{
private:
    EmployeeBaseInfo baseInfo;
    EmployeeSalary   salary;
public:
    EmployeeInfo(/* args */){}
    ~EmployeeInfo(){}
};

class EmployeeSubSystermInfo
{
private:
    EmployeeInfo employeeInfo;
public:
    EmployeeSubSystermInfo(/* args */){}
    ~EmployeeSubSystermInfo(){}
    void InsertBaseInfo(EmployeeInfo& emSalary){
        cout << "Insert BaseInfo to EmployeeInfo" <<endl;
    }
    EmployeeInfo getBaseInfo(){
        cout << "get BaseInfo from EmployeeInfo" <<endl;
        return employeeInfo;
    }
};

class EmployeeSubSystermSalary
{
private:
    EmployeeInfo employeeInfo;
public:
    EmployeeSubSystermSalary(/* args */){}
    ~EmployeeSubSystermSalary(){}
    void InsertSalary(EmployeeInfo& emSalary){
        cout << "insert salary to EmployeeInfo" <<endl;
    
    }
    EmployeeInfo getSalary(){
        cout << "get salary from EmployeeInfo" <<endl;
        return employeeInfo;
    }
};

//作为外观接口
class FacadeInterfaceEmployee
{
private:
    EmployeeSubSystermSalary    interfaceSalary;
    EmployeeSubSystermInfo      interfaceInfo;
public:
    FacadeInterfaceEmployee(/* args */){}
    ~FacadeInterfaceEmployee(){}

    void InsertEmployeeInfo(EmployeeInfo& ef) {
        cout << "----InsertEmployeeInfo----"<<endl;
        interfaceInfo.InsertBaseInfo(ef);
        interfaceSalary.InsertSalary(ef);
    }

    void getEmployeeInfo(EmployeeInfo& ef) {
        cout << "----UpdateEmployeeInfo----"<<endl;
        interfaceInfo.getBaseInfo();
        interfaceSalary.getSalary();
    }
};


int main() {
    EmployeeInfo ef;
    FacadeInterfaceEmployee facade;
    facade.InsertEmployeeInfo(ef);
    facade.getEmployeeInfo(ef);
    return 0;
}