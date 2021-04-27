#ifndef ANY_HPP_
#define ANY_HPP_

#include <iostream>
#include <memory>
#include <typeindex>

using namespace std;

class Any
{
private:
    /* data */
    struct Base;
    using BasePtr = std::unique_ptr<Base>;
    
    struct Base
    {
        /* data */
        virtual ~Base() {}
        virtual BasePtr Clone() const = 0;
    };

    template<typename T>
    struct Derived:public Base
    {
        /* data */
        template<typename U>
        Derived(U&&value):m_value(typename std::decay<U>::type(value)){}

        BasePtr Clone() const {
            return BasePtr(new Derived<typename std::decay<T>::type>(m_value));
        }

        T m_value;
    };
    
    BasePtr m_base;
    std::type_index m_typeIndex;

public:
    Any(/* args */):m_typeIndex(type_index(typeid(void))){}
    ~Any(){}
    //拷贝构造函数需要深拷贝，所以需要AnyClone
    Any(const Any& that):m_base(that.AnyClone()), m_typeIndex(that.m_typeIndex){}
    Any& operator=(const Any& that){
        if (m_base == that.m_base) {
            return *this;
        }
        m_base = that.AnyClone();
        m_typeIndex = that.m_typeIndex;
    }
    //移动构造函数通过移动语义即可解决
    Any(Any&& that):m_base(std::move(that.m_base)), m_typeIndex(that.m_typeIndex){}
    
    //类型是U不是Any的拷贝构造函数 U a; Any b = a;
    template<typename U, class = typename std::enable_if<!is_same<typename std::decay<U>::type, Any>::value, U>::type> Any(U&& value)
        :m_base(new Derived<typename std::decay<U>::type>(forward<U>(value))),
         m_typeIndex(type_index(typeid(typename std::decay<U>::type))) {}

    bool IsNull() const{
        return !bool(m_base);
    }

    template<typename U>
    bool IsSameTypeIndex() const{
        return bool(m_typeIndex == type_index(typeid(typename std::decay<U>::type)));
    }

    template<typename U>
    U& AnyCast() {
        if (!IsSameTypeIndex<U>()) {
            cout << "can not cast" << m_typeIndex.name() <<"to " << typeid(typename std::decay<U>::type).name() <<endl;
            throw bad_cast();
        }

        auto derived = dynamic_cast<Derived<U>*>(m_base.get());
        return derived->m_value;
    }

    BasePtr AnyClone() const {
        if (m_base) {
            return m_base->Clone();
        }
        return nullptr;
    }
};

#endif