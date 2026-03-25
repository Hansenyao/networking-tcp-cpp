#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

// Server's thread task data structure
struct Task {
    void (*func)(int);      // The pointer of the task function
    int arg;                // The argument for the task function

    Task(void (*f)(int), int p) : func(f), arg(p) {};
};

// The thread pool with a fixed threads number for TCP connection handling
// Using a queue to store TCP connection
class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();
public:
    void AddTask(void (*func)(int), int arg);
    void Stop();
private:
    static void worker(void* arg);
private:
    std::vector<std::thread> threads_;
    std::queue<Task> tasks_;
    std::mutex mutex_;
    std::condition_variable cond_;
    bool stop_;
};

#endif //_THREAD_POOL_H_
