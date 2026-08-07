#ifndef BUSINESS_WORKER_HPP
#define BUSINESS_WORKER_HPP

#include <queue>
#include <utility>
#include <condition_variable>
#include <functional>
#include "join_thread.hpp"
#include "file_logger.hpp"

namespace business_worker{
class BusinessWorker{
    public:
        void addTask(const std::function<void()>& job);
        void stop();

        BusinessWorker() : jt(join_thread::JoinThread(&workerLoop)){}
        BusinessWorker(const BusinessWorker&) = delete;
        BusinessWorker& operator=(const BusinessWorker&) = delete;
    private:
        join_thread::JoinThread jt;
        std::mutex mtx;
        std::condition_variable cv;
        std::atomic<bool> should_terminate = false;
        std::queue<std::function<void()>> jobs;

        void workerLoop();
};
} // namespace business_worker

#endif // BUSINESS_WORKER_HPP