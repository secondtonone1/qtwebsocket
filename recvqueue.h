#ifndef RECV_QUEUE_H
#define RECV_QUEUE_H
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <iostream>
#include <singleton.h>
using namespace std;


//投递队列单例类
template <typename T>
class RecvQueue :public CSingleton<RecvQueue<T> >{
    friend class  CSingleton<RecvQueue<T> >;
private:
    RecvQueue() { }

    RecvQueue(RecvQueue&)=delete;
    RecvQueue& operator=(const RecvQueue&)=delete;

public:
    ~RecvQueue() { }

    void push(T new_value) {
        mu_.lock();
        queue_.push(std::move(new_value));
        mu_.unlock();
        cond_.notify_all();
    }
    int32_t wait_and_pop(T& value, int64_t timeout_ms = -1) {
        std::unique_lock<std::mutex> lk(mu_);
        if (timeout_ms <= -1) {
            cond_.wait(lk, [this]{return !queue_.empty();});
            value = std::move(queue_.front());
            queue_.pop();
            return 0;
        } else {
            if (!cond_.wait_for(lk, std::chrono::milliseconds(timeout_ms), [this]{return !queue_.empty();})) {
                return -1;
            }
            value = std::move(queue_.front());
            queue_.pop();
            return 0;
        }
    }
private:
    mutable std::mutex mu_;
    std::queue<T> queue_;
    std::condition_variable cond_;

};



#endif
