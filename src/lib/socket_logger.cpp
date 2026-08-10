#include "socket_logger.hpp"

namespace logging {
inline constexpr int TCP_VALUE = 0;
inline constexpr int EDGE_CASE = 0;

// In case init wasn't used before logging,
// it just wouldn't log to socket because
// !fd_opt.has_value() returns true.
ErrorCode SocketLogger::init(IpType ip, PortType port, LogLevel default_level) {
  struct sockaddr_in addr;
  fd_opt = socket(AF_INET, SOCK_STREAM, TCP_VALUE);
  if (!fd_opt.has_value() || (fd_opt.value() < EDGE_CASE)) {
    return 4;
  }
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = ip;
  if (connect(fd_opt.value(), reinterpret_cast<struct sockaddr*>(&addr),
              sizeof(addr)) < EDGE_CASE) {
    close();
    return 5;
  }
  sl_api::setLevel(default_level);
  return 0;
}

inline constexpr int HEADERS_LENGTH = 9;
inline constexpr int YEAR_SHIFT = 1900;
inline constexpr int MONTH_SHIFT = 1;
inline constexpr std::size_t MAX_MSG_SIZE = 246;
using Byte = std::uint8_t;
using DoubleByte = std::uint16_t;

#pragma pack(push, 1)
struct MsgHeader {
  Byte full_msg_size;
  Byte day;
  Byte month;
  DoubleByte year;
  Byte hour;
  Byte min;
  Byte sec;
  Byte log_lvl;  // 0 – DEBUG, 1 – INFO, 2 – ERROR
};
#pragma pack(pop)

void setLocalTime(MsgHeader& header) {
  std::time_t t = std::time(nullptr);
  std::tm local_time;
  localtime_r(&t, &local_time);
  header.day = local_time.tm_mday;
  header.month = local_time.tm_mon + MONTH_SHIFT;
  header.year = htons(local_time.tm_year + YEAR_SHIFT);
  header.hour = local_time.tm_hour;
  header.min = local_time.tm_min;
  header.sec = local_time.tm_sec;
}
void fillMsgHeader(MsgHeader& header, std::size_t msg_length, LogLevel lvl) {
  header.full_msg_size = msg_length + HEADERS_LENGTH;
  setLocalTime(header);
  header.log_lvl = static_cast<Byte>(lvl);
}
ErrorCode SocketLogger::log(std::string_view msg, LogLevel lvl) {
  if (!fd_opt.has_value()) {
    return 1;
  }
  if (lvl < current_log_lvl) {
    return 0;
  }
  MsgHeader header;
  std::size_t msg_length = msg.length();

  if (msg_length > MAX_MSG_SIZE) {
    msg_length = MAX_MSG_SIZE;
  }
  fillMsgHeader(header, msg_length, lvl);
  std::lock_guard<std::mutex> lg(mtx);
  const char* header_ptr = reinterpret_cast<const char*>(&header);
  std::size_t total_sent = 0;
  while (total_sent < HEADERS_LENGTH) {
    ssize_t sent = send(fd_opt.value(), header_ptr + total_sent,
                        HEADERS_LENGTH - total_sent, TCP_VALUE);
    if (sent <= 0) {
      return 2;
    }
    total_sent += sent;
  }
  if (msg_length > 0) {
    const char* body_ptr = msg.data();
    total_sent = 0;
    while (total_sent < msg_length) {
      ssize_t sent = send(fd_opt.value(), body_ptr + total_sent,
                          msg_length - total_sent, TCP_VALUE);
      if (sent <= 0) {
        return 2;
      }
      total_sent += sent;
    }
  }
  return 0;
}
}  // namespace logging