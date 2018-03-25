#include "EasyThreadPool.h"
#include <iostream>

EasyThreadPool::EasyThreadPool(size_t numThreads) : _isDone(false), _taskEmpty(true)
{
    try
    {
        for (size_t i = 0; i < numThreads; i++)
        {
            _threads.push_back(std::thread(&EasyThreadPool::RunThreadWorker, this));
        }
    }
    catch (...)
    {
        _isDone.store(true);
        throw;
    }
}

EasyThreadPool::~EasyThreadPool()
{
    _isDone.store(true);
    //唤醒所有线程
    _cv.notify_all();
    for (std::thread &thread : _threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void EasyThreadPool::PostTask(const Task &task)
{
    if (_isDone)
    {
        std::cout << "The threadPool has been stop!" << std::endl;
    }
    else
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this] { return !_isDone; });
        _tasks.push(task);
        _taskEmpty.store(false);
        //唤醒一个线程去执行
        _cv.notify_one();
    }
}

void EasyThreadPool::RunThreadWorker()
{
    Task task;
    //确保任务执行完成后再销毁
    while (!_isDone || !_taskEmpty)
    {
        if (GetTask(task))
        {
            task();
        }
        else
        {
            //暂停一段时间，给别的线程机会
            std::this_thread::yield();
        }
    }
}

bool EasyThreadPool::GetTask(Task &task)
{
    std::unique_lock<std::mutex> lock(_mutex);
    //不满足条件会阻塞
    _cv.wait(lock, [this] { return !_taskEmpty || _isDone; });
    if (!_tasks.empty())
    {
        task = std::move(_tasks.front());
        _tasks.pop();
        _taskEmpty.store(_tasks.empty());
        return true;
    }
    return false;
}
