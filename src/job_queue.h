#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <atomic>
#include <vector>

namespace vanish {
namespace core {

class JobQueue {
public:
    JobQueue(size_t num_threads = 2, unsigned int rate_limit_ms = 1000);
    ~JobQueue();

    void enqueue(std::function<void()> task);
    void stop();

private:
    void worker_loop();

    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;

    std::mutex _mutex;
    std::condition_variable _cv;
    std::atomic<bool> _stop;
    
    unsigned int _rate_limit_ms;
};

} // namespace core
} // namespace vanish
