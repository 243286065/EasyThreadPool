# EasyThreadPool
ThreadPool base on C++11

使用实例：
```
// test.cpp

#include "EasyThreadPool.h"
#include <iostream>

void PrintInfo()
{
    std::cout << std::this_thread::get_id() << ": Test" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

int main()
{
    EasyThreadPool pool(10);
    int i = 0;
    while (i++ < 100)
    {
        pool.PostTask(PrintInfo);
    }
    return 0;
}
```

编译命令：
```
g++ test.cpp EasyThreadPool.h EasyThreadPool.cpp -o test --std=c++11 -lpthread
```
