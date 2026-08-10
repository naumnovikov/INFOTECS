#ifndef EXIT_HPP
#define EXIT_HPP

#include <iostream>

#include "menu.hpp"

namespace menu {
class Exit : public Menu {
 private:
  BusinessWorker& worker;

 public:
  inline explicit Exit(BusinessWorker& worker_param) : worker(worker_param) {}
  void execute(Tokens&) override {
    std::cout << "Exiting...";
    worker.stop();
    worker.manualJoin();
    fl_api::close();
    sl_api::close();
  }
};
}  // namespace menu

#endif  // EXIT_HPP