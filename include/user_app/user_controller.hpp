#ifndef FA_CONTROLLER_HPP
#define FA_CONTROLLER_HPP

#include <memory>
#include <sstream>
#include <unordered_map>

#include "business_worker.hpp"
#include "change.hpp"
#include "exit.hpp"
#include "file_logger.hpp"
#include "help.hpp"
#include "log.hpp"

namespace user_controller {
using CommandArgument = std::string;

class Controller {
 public:
  static Controller& getInstance() noexcept {
    static Controller instance;
    return instance;
  }
  void interact();
  inline void setSocketAvailable(bool available) noexcept {
    socket_available = available;
  }
  inline bool socketAvailable() const noexcept { return socket_available; }

  Controller(Controller const&) = delete;
  Controller& operator=(Controller const&) = delete;

 private:
  bool socket_available = false;
  std::unordered_map<CommandArgument, std::unique_ptr<menu::Menu>> commands;

  void initCommands(business_worker::BusinessWorker& worker);

  Controller() noexcept = default;
  ~Controller() noexcept = default;
};
inline menu::Tokens collectInputCommand(std::string command_buffer) {
  menu::Tokens tokens;
  std::istringstream iss(std::move(command_buffer));
  std::string token;
  while (iss >> token) {
    if (!token.empty()) {
      tokens.push_back(std::move(token));
    }
  }
  return tokens;
}
}  // namespace user_controller

namespace user_controller_api {
inline void interact() {
  user_controller::Controller::getInstance().interact();
}
inline void setSocketAvailable(bool available) noexcept {
  user_controller::Controller::getInstance().setSocketAvailable(available);
}
inline bool socketAvailable() noexcept {
  return user_controller::Controller::getInstance().socketAvailable();
}
}  // namespace user_controller_api

#endif  // FA_CONTROLLER_HPP