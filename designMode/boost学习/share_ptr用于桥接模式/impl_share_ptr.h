/* boost程序库完全开发指南
    实现的桥接模式
*/

#include <iostream>

class sample
{
private:
    class impl;
    shared_ptr<impl> pm;
public:
    sample(/* args */);
    void print();
    ~sample();
};

class sample::impl
{
private:
    /* data */
public:
    void print(){
        std::cout << "print impl";
    }
    impl(/* args */);
    ~impl();
};

sample::sample(/* args */):pm(make_shared<impl>())
{
    
}

sample::~sample()
{
}

void sample::print()
{
    pm->print();
}


int main() 
{
    sample s;
    s.print(); //结果是 print impl;
}
