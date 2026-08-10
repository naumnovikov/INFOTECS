#include "business_worker.hpp"

namespace business_worker {
void BusinessWorker::workerLoop() {
  while (true) {
    std::function<void()> job;
    {
      std::unique_lock<std::mutex> ul(mtx);
      cv.wait(ul, [this]() { return !jobs.empty() || should_terminate; });
      if (should_terminate) {
        return;
      }
      job = jobs.front();
      jobs.pop();
    }
    job();
  }
}
void BusinessWorker::addTask(const std::function<void()>& job) {
  std::unique_lock<std::mutex> ul(mtx);
  jobs.push(job);
  cv.notify_one();
}
void BusinessWorker::stop() {
  should_terminate = true;
  cv.notify_one();
}
}  // namespace business_worker