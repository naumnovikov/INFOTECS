#include "log.hpp"

#include <future>
#include <memory>

#include "user_controller.hpp"

namespace menu {
std::pair<msg, logging::LogLevel> Log::parseInput(const Tokens& tokens) {
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
  bool level_specified = false;

  if (last_clean == "DEBUG" || last_clean == "INFO" || last_clean == "ERROR") {
    level_specified = true;
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
  if (!level_specified) {
    std::cout << "No level specified, using DEBUG by default.\n";
  }

  return {message, lvl};
}

// Always log in file
// but log in socket only if availalbe
// (Socket is available if user set [ip:port] on initialization stage)
void Log::execute(Tokens& tokens) {
  auto p = parseInput(tokens);
  if (p.first.empty()) {
    std::cout << "No message to log. Skipping.\n";
    return;
  }

  bool will_be_logged = false;
  if (p.second >= fl_api::getLevel()) {
    will_be_logged = true;
  }
  if (user_controller_api::socketAvailable() &&
      p.second >= sl_api::getLevel()) {
    will_be_logged = true;
  }

  if (!will_be_logged) {
    std::cout << "Message level is below current threshold. Skipped.\n";
    return;
  }

  auto promise = std::make_shared<std::promise<logging::ErrorCode>>();
  auto future = promise->get_future();

  worker.addTask([p, promise]() {
    auto ec = fl_api::log(p.first, p.second);
    if (ec == 0 && user_controller_api::socketAvailable()) {
      ec = sl_api::log(p.first, p.second);
    }
    promise->set_value(ec);
  });

  logging::ErrorCode ec = future.get();
  if (ec == 0) {
    std::cout << "Message logged successfully.\n";
  } else {
    std::cerr << "Logging error (code " << ec << ").\n";
  }
}
}  // namespace menu