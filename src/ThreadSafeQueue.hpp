#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>

template <typename T>
class ThreadSafeQueue{
public:

    size_t size() const{
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }
    void push(T o){
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(o));
        cv_.notify_one();
    }
    T pop(){
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [&](){return !queue_.empty() || stopping_;});
        if(stopping_ && queue_.empty()){
            throw std::runtime_error("Queue Stopped");
        }
        T o = std::move(queue_.front());
        queue_.pop();
        return o;
    }
    void stop(){
        std::lock_guard<std::mutex> lock(mutex_);
        stopping_ = true;
        cv_.notify_all();
    }
    void forceStop(){
        std::lock_guard<std::mutex> lock(mutex_);
        stopping_ = true;
        while (!queue_.empty()) queue_.pop();
        cv_.notify_all();
    }

private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cv_;
    bool stopping_ = false;
};