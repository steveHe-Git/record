/* boost 程序库函，完全开发指南
在程序中编写自己的工厂类或工厂函数时通常需要在堆上使用new
动态分配一个对象，然后返回对象的指针，因为容易忘记使用delete，存在资源泄漏的隐患；
使用shaerd_ptr可以解决这个问题，只需要修改工厂方法的接口，不再返回原始指针，而是返回一个被share_ptr包装过的指针
*/


#include <iostream>

class abstract
{
private:
    /* data */
public:
    virtual void f() = 0;
    virtual void g() = 0;
protected:
    virtual ~abstract() = default; //注意： abstract析够函数定位为保护的，这意味着除了它自己和它的子类，任何其他对象都无权使用delete删除
};

class impl:public abstract
{
private:
    /* data */
public:
    virtual void f() {
        cout << "class impl f";
    }
    virtual void g() {
        cout << "class impl g";
    }
    impl(/* args */);
    ~impl();
};

impl::impl(/* args */)
{
}

impl::~impl()
{
}

shared_ptr<abstract> creat()
{
    return make_shared<impl>();
}

int main() 
{
    //使用智能指针，自动管理析够
    auto p = creat();
    p->f();
    p->g();

    //错误的
    abstract* q = p.get();  //因为abstract析够是保护的，外部对象无法访问
    delete q;               //这个编译不能通过，abstract析够是保护的，delete相当于一次外部对象调用析够函数
	
    //错误的
    //(#永远不要这样做， 因为shared_ptr在析够时删除可能已经不存在的指针，引发未定义的行为)
    impl* q = (impl*)p.get();  //impl是公开的，可以调用impl的析够函数，之类析够之后(之类指向父类的指针)，会调用父类的析够函数；
    delete q;               //这个能编译通过，delete相当于一次外部对象调用析够函数，然后智能指针在析够时删除delete已经删除的指针，引发未定义行为；
}
}

