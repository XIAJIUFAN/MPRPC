#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

// 异步写日志的日志队列
template<typename T>
class LockQueue
{
public:
    // 多个线程都会取写日志queue
    void Push(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
        m_condVariable.notify_one(); // 通知写日志线程去从日志队列中取数据
    }

    // 一个线程读日志queue，写日志文件
    T Pop() // 在写日志线程中执行
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty()) 
        {
            // 日志队列为空，线程进入wait状态，并且释放锁
            m_condVariable.wait(lock);
        }

        T data = m_queue.front();
        m_queue.pop();
        return data;
    }  
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condVariable;
};