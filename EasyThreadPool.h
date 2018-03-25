#ifndef _EASY_THREAD_POOL_H_
#define _EASY_THREAD_POOL_H_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <queue>
#include <functional>

class EasyThreadPool
{
  private:
    using Task = std::function<void()>;

  public:
    EasyThreadPool(size_t numThreads = std::thread::hardware_concurrency());
    ~EasyThreadPool();

    //提交任务
    void PostTask(const Task &);

  private:
    std::vector<std::thread> _threads;
    std::queue<Task> _tasks;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::atomic<bool> _isDone;  //是否已停止加入新任务
    std::atomic<bool> _taskEmpty; //任务队列是否已空

    void RunThreadWorker();
    bool GetTask(Task&);
};

#endif // _EASY_THREAD_POOL_H_
