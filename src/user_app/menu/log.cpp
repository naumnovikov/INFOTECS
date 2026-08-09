#include "log.hpp"

#include "user_controller.hpp"

namespace menu {
std::pair<msg, logging::LogLevel> Log::parseInput(
    const Tokens& tokens) noexcept {
  if (tokens.size() < 2) {
    std::cout << "No message provided.\n";
    return {"", logging::LogLevel::DEBUG};
  }
  std::string last = tokens.back();
  std::string last_clean = last;
  if (!last_clean.empty() && last_clean.front() == '"') {
    last_clean.erase(0, 1);
  }
  if (!last_clean.empty() && last_clean.back() == '"') {
    last_clean.pop_back();
  }
  turnStringIntoUpper(last_clean);

  logging::LogLevel lvl = logging::LogLevel::DEBUG;
  std::size_t end_idx = tokens.size();

  if (last_clean == "DEBUG" || last_clean == "INFO" || last_clean == "ERROR") {
    if (last_clean == "INFO") {
      lvl = logging::LogLevel::INFO;
    }
    if (last_clean == "ERROR") {
      lvl = logging::LogLevel::ERROR;
    }
    end_idx = tokens.size() - 1;
  }

  std::string message;
  for (size_t i = 1; i < end_idx; ++i) {
    if (i > 1) {
      message += ' ';
    }
    message += tokens[i];
  }

  if (!message.empty() && message.front() == '"') {
    message.erase(0, 1);
  }
  if (!message.empty() && message.back() == '"') {
    message.pop_back();
  }

  return {message, lvl};
}

// Always log in file
// but log in socket only if availalbe
// (Socket is available if user set [ip:port] on initialization stage)
void Log::execute(Tokens& tokens) {
  auto p = parseInput(tokens);
  worker.addTask([p]() {
    logging::ErrorCode ec = fl_api::log(p.first, p.second);
    if (ec != 0) {
      std::cerr << "File logger error: " << ec << "\n";
    }
    if (user_controller_api::socketAvailable()) {
      ec = sl_api::log(p.first, p.second);
      if (ec != 0) {
        std::cerr << "Socket logger error: " << ec << "\n";
      }
    }
  });
  std::cout << "Message processed...\n";
}
}  // namespace menu