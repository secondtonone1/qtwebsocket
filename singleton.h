#ifndef SINGLETON_H
#define SINGLETON_H
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <iostream>
using namespace std;
//模板单例类
template <typename T>
class CSingleton
{

public:
      static std::shared_ptr<T> getInstance()
       {
           // "double checked lock" 防止虚假唤醒
           if(m_pInstance==nullptr){
               std::lock_guard<std::mutex> lk(m_sgMutex);
               if(m_pInstance == nullptr){
                 m_pInstance = std::shared_ptr<T>(new T);
               }
           }
           return m_pInstance;
       }

       CSingleton(CSingleton&)=delete;
       CSingleton& operator=(const CSingleton&)=delete;

       ~CSingleton(){
           cout << "destruct CSingleton" << endl;
       }

protected:

       CSingleton(){
            cout << "construct CSingleton" << endl;
       }
protected:
       static std::shared_ptr<T> m_pInstance;

       static std::mutex m_sgMutex;

};

template <typename T>
std::shared_ptr<T>  CSingleton<T> :: m_pInstance = nullptr;

template <typename T>
std::mutex CSingleton<T>::m_sgMutex ;

#endif
