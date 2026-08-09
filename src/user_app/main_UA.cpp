#include <arpa/inet.h>

#include <cctype>
#include <cstdlib>
#include <string>

#include "socket_logger.hpp"
#include "user_controller.hpp"

namespace UA_cli {
inline constexpr int MIN_PORT = 1;
inline constexpr int MAX_PORT = 65535;
inline constexpr int MIN_ARGS = 3;
inline constexpr int MAX_ARGS = 4;
}  // namespace UA_cli

int main(int argc, const char* argv[]) {
  if (argc < UA_cli::MIN_ARGS || argc > UA_cli::MAX_ARGS) {
    std::cerr << "Usage: " << argv[0] << " <log_filename> <level> [ip:port]\n";
    return 1;
  }

  std::string filename = argv[1];
  std::string level_str = argv[2];
  bool use_socket = (argc == UA_cli::MAX_ARGS);

  logging::LogLevel default_lvl = logging::LogLevel::DEBUG;
  menu::turnStringIntoUpper(level_str);
  if (level_str == "INFO") {
    default_lvl = logging::LogLevel::INFO;
  } else if (level_str == "ERROR") {
    default_lvl = logging::LogLevel::ERROR;
  } else if (level_str != "DEBUG") {
    std::cerr << "Unknown level, using DEBUG\n";
  }

  logging::ErrorCode ec = fl_api::init(filename, default_lvl);
  if (ec != 0) {
    std::cerr << "Failed to init file logger. Error code: " << ec << "\n";
    return 1;
  }

  if (use_socket) {
    std::string arg = argv[3];
    size_t colon = arg.find(':');
    if (colon == std::string::npos) {
      std::cerr << "Invalid socket format. Use ip:port\n";
      return 1;
    }
    std::string ip = arg.substr(0, colon);
    int port;
    try {
      port = std::stoi(arg.substr(colon + 1));
    } catch (const std::exception&) {
      std::cerr << "Invalid port number.\n";
      return 1;
    }
    if (port < UA_cli::MIN_PORT || port > UA_cli::MAX_PORT) {
      std::cerr << "Port must be between " << UA_cli::MIN_PORT << " and "
                << UA_cli::MAX_PORT << "\n";
      return 1;
    }
    uint32_t ip_int = inet_addr(ip.c_str());
    if (ip_int == INADDR_NONE) {
      std::cerr << "Invalid IP address.\n";
      return 1;
    }
    ec = sl_api::init(ip_int, static_cast<uint16_t>(port), default_lvl);
    if (ec == 0) {
      user_controller_api::setSocketAvailable(true);
    } else {
      std::cerr << "Failed to init socket logger. Error code: " << ec << "\n";
    }
  }

  user_controller_api::interact();
  return 0;
}