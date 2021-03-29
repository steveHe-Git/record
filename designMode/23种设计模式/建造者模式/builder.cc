/* 建造者模式， 属于创建形模式*/
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Product;
typedef std::shared_ptr<Product> ProductPtr;

class Product
{
private:
    vector<string> vecString;
public:
    Product(/* args */){}
    ~Product(){}

    void add(string str) {
        vecString.push_back(str);
    }

    void show() {
        cout << "产品的构造顺序是:" <<endl;
        for (auto x:vecString) {
            cout << x << endl;
        }
        cout << "产品构造完成" <<endl;
    }
};

class Builder;
typedef std::shared_ptr<Builder> BuilderPtr;

class Builder
{
private:
    /* data */
public:
    Builder(/* args */){}
    virtual ~Builder(){}
    virtual void BuildPartA(){}
    virtual void BuildPartB(){}
    virtual ProductPtr GetResult(){}
};

class ConcreatPart1:public Builder
{
private:
    ProductPtr product;
public:
    ConcreatPart1(/* args */){
        product = std::make_shared<Product>();
    }
    ~ConcreatPart1(){}

    void BuildPartA () {
        product->add("构建A部分");
    }

    void BuildPartB () {
        product->add("构建B部分");
    }

    ProductPtr GetResult(){
        return product;
    }
};

class ConcreatPart2:public Builder
{
private:
    ProductPtr product;
public:
    ConcreatPart2(/* args */){
        product = std::make_shared<Product>();
    }
    ~ConcreatPart2(){}
    void BuildPartA () {
        product->add("构建X部分");
    }

    void BuildPartB () {
        product->add("构建Y部分");
    }

    ProductPtr GetResult(){
        return product;
    }
};

class ClientDirector
{
private:
    /* data */
public:
    ClientDirector(/* args */){}
    ~ClientDirector(){}
    void creat(BuilderPtr build) {
        build->BuildPartA();
        build->BuildPartB();
    }
};

int main () {
    ClientDirector director;
    auto concreat1 = std::make_shared<ConcreatPart1>();
    director.creat(concreat1);
    concreat1->GetResult()->show();
    cout << "---------------------" <<endl;
    auto concreat2 = std::make_shared<ConcreatPart2>();
    director.creat(concreat2);
    concreat2->GetResult()->show();

    return 0;
}