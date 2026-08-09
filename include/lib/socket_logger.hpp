#ifndef SOCKET_LOGGER_HPP
#define SOCKET_LOGGER_HPP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <optional>

#include "i_logger.hpp"

namespace logging {
using FdType = int;
using PortType = std::uint16_t;
using IpType = std::uint32_t;

class SocketLogger : public ILogger<SocketLogger> {
 public:
  static SocketLogger& getInstance() {
    static SocketLogger instance;
    return instance;
  }
  inline void setLevel(LogLevel lvl) override { current_log_lvl = lvl; }
  ErrorCode log(std::string_view msg, LogLevel lvl) override;
  ErrorCode init(IpType ip, PortType port, LogLevel default_level);

  // Don't forget to close socket.
  void close() override {
    if (fd_opt.has_value()) {
      ::close(fd_opt.value());
      fd_opt.reset();
    }
  }

 private:
  std::atomic<LogLevel> current_log_lvl = LogLevel::DEBUG;
  std::optional<FdType> fd_opt;
  std::mutex mtx;

  SocketLogger() = default;
  ~SocketLogger() = default;
};
}  // namespace logging

// Just shorter way to call Fl functions.
namespace sl_api {
inline logging::ErrorCode log(std::string_view msg, logging::LogLevel level) {
  return logging::SocketLogger::getInstance().log(msg, level);
}
inline void setLevel(logging::LogLevel level) {
  logging::SocketLogger::getInstance().setLevel(level);
}
inline logging::ErrorCode init(logging::IpType ip, logging::PortType port,
                               logging::LogLevel default_level) {
  return logging::SocketLogger::getInstance().init(ip, port, default_level);
}
inline void close() { logging::SocketLogger::getInstance().close(); }
}  // namespace sl_api

#endif  // SOCKET_LOGGER_HPP