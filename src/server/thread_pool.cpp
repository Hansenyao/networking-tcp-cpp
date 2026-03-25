#include "thread_pool.h"

ThreadPool::ThreadPool(size_t num_threads)
 : stop_(false)
{
    for (size_t i = 0; i < num_threads && i < MAX_THREADS_NUM; i++) {
        threads_.emplace_back(worker, this);
    }
}

ThreadPool::~ThreadPool()
{
    Stop();
}

void ThreadPool::AddTask(void (*func)(int), int arg)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stop_) return;
        tasks_.emplace(func, arg);
    }

    cond_.notify_one();
}

void ThreadPool::Stop()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stop_) return;
        stop_ = true;
    }
    
    cond_.notify_all();

    for (auto& t : threads_) {
        if (t.joinable()) t.join();
    }
}

void ThreadPool::worker(void* arg)
{
    ThreadPool *pool = (ThreadPool*)arg;

    while (true) {
        std::unique_lock<std::mutex> lock(pool->mutex_);
        pool->cond_.wait(lock, [pool]() {
            return pool->stop_ || !pool->tasks_.empty();
        });

        if (pool->stop_ && pool->tasks_.empty()) break;

        Task task = pool->tasks_.front();
        pool->tasks_.pop();
        lock.unlock();
        
        task.func(task.arg);
    }
}