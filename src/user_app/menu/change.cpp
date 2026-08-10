#include "change.hpp"

#include "user_controller.hpp"

namespace menu {

logging::LogLevel Change::parseInput(const Tokens& tokens) {
  if (tokens.size() < 2) {
    std::cout << "No level specified. Keeping current level (DEBUG).\n";
    return logging::LogLevel::DEBUG;
  }
  std::string raw_lvl = tokens[1];
  turnStringIntoUpper(raw_lvl);
  if (raw_lvl == "DEBUG") {
    return logging::LogLevel::DEBUG;
  } else if (raw_lvl == "INFO") {
    return logging::LogLevel::INFO;
  } else if (raw_lvl == "ERROR") {
    return logging::LogLevel::ERROR;
  } else {
    std::cout << "Unknown logging level. Log level will be set to DEBUG.\n";
  }
  return logging::LogLevel::DEBUG;
}

void Change::execute(Tokens& tokens) {
  logging::LogLevel lvl = parseInput(tokens);
  fl_api::setLevel(lvl);
  if (user_controller_api::socketAvailable()) {
    sl_api::setLevel(lvl);
  }
  std::cout << "Logging level was changed.\n";
}

}  // namespace menu