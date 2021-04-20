/*
* c++单例模式
*/
#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <iostream>
#include <memory>
#include <vector>
#include <mutex>

template<typename T>
class Singleton
{
private:
    /* data */
    static std::mutex m_Mutex;
    static std::shared_ptr<T> m_Instance;
    Singleton(const Singleton&){}
    Singleton& operator=(const Singleton&){}
    Singleton(/* args */);
    ~Singleton();
public:
    /*饱汉式*/
    static std::shared_ptr<T> GetInstance() {
        if (!m_Instance) {
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (!m_Instance) {
                m_Instance = std::make_shared<T>();
            }
        }
        return m_Instance;
    }

    /*饱汉式多参数版本*/
    template<typename... Args>
    static std::shared_ptr<T> GetInstanceMultiParam(Args&& ...args){
        if (!m_Instance) {
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (!m_Instance) {
                m_Instance = std::make_shared<T>();
            }
        }
        return m_Instance;
    }

    /*饿汉式*/   
    static std::shared_ptr<T> GetInstanceWithoutMutex() {
        static std::shared_ptr<T> Instance;
        if (!Instance) {
            Instance = std::make_shared<T>();
            //Instance = std::shared_ptr<T>(new T())
        }
        return Instance;
    }
};

template<typename T>
std::mutex Singleton<T>::m_Mutex;

template<typename T>
std::shared_ptr<T> Singleton<T>::m_Instance = nullptr;

int main () {
    return 0;
}

#endif