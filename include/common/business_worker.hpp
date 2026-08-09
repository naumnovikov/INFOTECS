#ifndef BUSINESS_WORKER_HPP
#define BUSINESS_WORKER_HPP

#include <condition_variable>
#include <functional>
#include <queue>
#include <utility>

#include "join_thread.hpp"

namespace business_worker {
class BusinessWorker {
 public:
  void addTask(const std::function<void()>& job);
  void stop();

  // manualJoin() is needed when
  // stop() was called but workerLoop()
  // already work. manualJoin() synchronizes
  // threads to solve problems like
  // closing file while workerLoop() tries to
  // use it.
  void manualJoin() {
    // join() in join_thread::JoinThread
    // checks if jt is joinable inside.
    jt.join();
  }

  BusinessWorker() : jt(join_thread::JoinThread([this]() { workerLoop(); })) {}
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
}  // namespace business_worker

#endif  // BUSINESS_WORKER_HPP