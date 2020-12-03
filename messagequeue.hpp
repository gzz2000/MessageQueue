#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

typedef std::chrono::time_point timer_index;

template<typename T>
class MessageQueue {
    std::queue<T> q;
    mutable std::mutex mutex;
    std::condition_variable cond;
    std::map<std::chrono::time_point, T> timers;

public:
    inline T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        while(q.empty()) {
            if(timers.empty()) cond.wait(lock);
            else {
                auto status = cond.wait_until(lock, timers.begin()->first);
                if(status == std::cv_status::timeout && !timers.empty()) {
                    T ret = std::move(timers.begin()->second);
                    timers.erase(timers.begin());
                    return ret;
                }
            }
        }
        T ret = std::move(q.front());
        q.pop();
        return ret;
    }

    inline void push(const T &msg) {
        std::lock_guard<std::mutex> lock(mutex);
        q.push(msg);
        cond.notify_one();
    }

    inline std::chrono::time_point setTimeout(const T &msg, int delay /* ms */) {
        std::lock_guard<std::mutex> lock(mutex);
        std::chrono::time_point index = std::chrono::steady_clock::now() + std::chrono::milliseconds(delay);
        while(timers.find(index) != timers.end()) index += std::chrono::microseconds(1);
        timers[index] = msg;
        cond.notify_one();   // most to reset the timer
        return index;
    }

    inline void clearTimeout(std::chrono::time_point index) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = timers.find(index);
        if(it != timers.end()) {
            timers.erase(it);
            cond.notify_one();
        }
    }

    // inline size_t size() const {
    //     std::lock_guard<std::mutex> lock(mutex);
    //     return q.size(); // + timers.size();
    // }

    // inline bool empty() const
};

