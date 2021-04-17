/*
* 组合模式
*/

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
using namespace std;

class company;
typedef std::shared_ptr<company>company_ptr;
class company
{
protected:
    /* data */
    std::string m_name;
public:
    company(std::string name):m_name(name){}
    virtual ~company(){}
    virtual void add(company_ptr company){}
    virtual void remove(company_ptr company){}
    virtual void function(){}
    virtual std::string getName() const = 0;
    virtual void show(int index){}
};

class concreatCompany:public company
{
private:
    std::map<std::string, company_ptr> companyMap;
public:
    concreatCompany(std::string name):company(name){}
    ~concreatCompany(){}
    virtual void add(company_ptr company){
        //companyMap.insert(std::pair<string, company_ptr>(m_name, company));
        companyMap.emplace(company->getName(), company);
    }
    virtual void remove(std::string company_name){
        auto it = companyMap.find(company_name);
        if(it != companyMap.end()){
            companyMap.erase(it);
        }
    }
    virtual void function(){
        std::cout << "size: " << companyMap.size() <<std::endl;
        for (auto company:companyMap) {
            company.second->function();
        }
    }
    virtual std::string getName() const{
        return m_name;
    }
    virtual void show(int index){
        for(int i =0; i< index; i++) 
            std::cout <<"-";
        std::cout << m_name << std::endl;
        for (auto company:companyMap) {
            company.second->show(index + 2);
        }
    }
};

class hrDepartment:public company
{
private:
    /* data */
public:
    hrDepartment(std::string name):company(name){}
    ~hrDepartment(){}
    virtual void add(){}
    virtual void function(){
        std::cout << m_name<< "  hr招聘人才"<< std::endl;
    }
    virtual void remove(company_ptr company){}
    virtual std::string getName() const{
        return m_name;
    }
    virtual void show(int index){
        for(int i =0; i< index; i++) 
            std::cout <<"-";
        std::cout << m_name << std::endl;
    }
    
};

class financeDepartment:public company
{
private:
    /* data */
public:
    financeDepartment(std::string name):company(name){}
    ~financeDepartment(){}
    virtual void add(){}
    virtual void function(){
        std::cout << m_name <<"  负责公司的财务支出"<< std::endl;
    }
    virtual void remove(company_ptr company){}
    virtual std::string getName() const{
        return m_name;
    }
    virtual void show(int index){
        for(int i =0; i< index; i++) 
            std::cout <<"-";
        std::cout << m_name << std::endl;
    }
};

int main()
{
    company_ptr root = make_shared<concreatCompany>("总公司");
    company_ptr leafcomp1 = make_shared<hrDepartment>("总公司人事处");
    company_ptr leafcomp2 = make_shared<financeDepartment>("总公司财务处");
    company_ptr root1 = make_shared<concreatCompany>("分公司1");
    company_ptr leafcomp3 = make_shared<hrDepartment>("分公司1人事处");
    company_ptr leafcomp4 = make_shared<financeDepartment>("分公司1财务处");
    root->add(leafcomp1);
    root->add(leafcomp2);
    root->add(root1);
    root1->add(leafcomp3);
    root1->add(leafcomp4);

    root->show(0);
    root->function();
    return 0;
}