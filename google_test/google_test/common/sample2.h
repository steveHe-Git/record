#ifndef sample2_H_
#define sample2_H_

#include <string.h>
#include <iostream>
using namespace std;

class MyString
{
private:
    /* data */
    const char* c_string;
    const MyString& operator=(const MyString& rhs);
public:
    //MyString s 时调用
    MyString(/* args */):c_string(nullptr) {

    };

    //MyString s2 =s1
    MyString(const MyString& str):c_string(nullptr)  {
        set(str.c_string);
    }

    //s1(s)调用
    explicit MyString(const char* a_c_string):c_string(nullptr) {
        set(a_c_string);
    }

    ~MyString() {
        if (c_string) {
            delete[] c_string;
            c_string = nullptr;
        }
    };

    static const char* CloneCString(const char* a_c_string){
        //cout << "a_c_string: " << a_c_string << endl;
        if (a_c_string == nullptr) return nullptr;
        const size_t len = strlen(a_c_string);

        char* const clone = new char[len + 1];
        memset(clone, 0, len+1);
        memcpy(clone, a_c_string, len+1);

        return clone;
    }

    const char* getCString() const{
        return c_string;
    }

    size_t Length() const {
        return c_string == nullptr? 0: strlen(c_string);
    }

    void set(const char* a_c_string){
        const char* temp = CloneCString(a_c_string);
        if (c_string) {
            delete[] c_string;
            c_string = nullptr;
        }
        c_string = temp;
    }

    void print() {
        //cout << "str: " << c_string << endl;
        //printf("str: %s  P: %p\n ", c_string, c_string);
    }
};


#endif