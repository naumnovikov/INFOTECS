#ifndef LOG_HPP
#define LOG_HPP

#include "menu.hpp"

namespace menu {
using msg = std::string;
class Log : public Menu {
 public:
  explicit Log(BusinessWorker& worker_param) : worker(worker_param) {}
  void execute(Tokens& tokens) override;

 private:
  BusinessWorker& worker;

  static std::pair<msg, logging::LogLevel> parseInput(
      const Tokens& tokens);
};
}  // namespace menu

#endif  // LOG_HPP