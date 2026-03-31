#include "job_queue.h"
#include <chrono>

namespace vanish {
namespace core {

JobQueue::JobQueue(size_t num_threads, unsigned int rate_limit_ms)
    : _stop(false), _rate_limit_ms(rate_limit_ms) {
    for (size_t i = 0; i < num_threads; ++i) {
        _workers.emplace_back(&JobQueue::worker_loop, this);
    }
}

JobQueue::~JobQueue() {
    stop();
}

void JobQueue::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _tasks.push(std::move(task));
    }
    _cv.notify_one();
}

void JobQueue::stop() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stop = true;
    }
    _cv.notify_all();
    for (auto& worker : _workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void JobQueue::worker_loop() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _cv.wait(lock, [this]() { return _stop || !_tasks.empty(); });
            if (_stop && _tasks.empty()) {
                return;
            }
            task = std::move(_tasks.front());
            _tasks.pop();
        }
        
        task();
        
        // slap a delay on the next execution if we have a rate limit.
        // using the condition variable instead of sleep so we can bail out immediately if the queue gets killed
        if (_rate_limit_ms > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(_rate_limit_ms));
        }
    }
}

} // namespace core
} // namespace vanish
