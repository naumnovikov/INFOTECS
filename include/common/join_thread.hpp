#ifndef WORKER_HPP
#define WORKER_HPP

#include <thread>

namespace join_thread {
class JoinThread {
 public:
  void join() {
    if (t.joinable()) {
      t.join();
    }
  }

  template <typename Function, typename... Args>
  explicit JoinThread(Function&& f, Args&&... args)
      : t(std::forward<Function>(f), std::forward<Args>(args)...) {}
  ~JoinThread() { join(); }
  JoinThread(const JoinThread&) = delete;
  JoinThread& operator=(const JoinThread&) = delete;

 private:
  std::thread t;
};
}  // namespace join_thread

#endif  // WORKER_HPP