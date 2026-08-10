#include "stat_controller.hpp"

#include <sys/select.h>
#include <sys/time.h>

#include <cerrno>
#include <cstring>
#include <iomanip>
#include <iostream>

namespace stat_controller {
inline constexpr int TCP_VALUE = 0;
inline constexpr int HEADERS_LENGTH = 9;
inline constexpr int MONTH_SHIFT = 1;
inline constexpr int YEAR_SHIFT = 1900;
inline constexpr int EDGE_CASE = 0;
inline constexpr std::size_t TIME_BUFFER_SIZE = 64;
inline constexpr unsigned MAX_BODY_LENGTH = 246;
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

std::string_view levelToString(unsigned lvl) {
  switch (lvl) {
    case 0:
      return "DEBUG";
    case 1:
      return "INFO";
    case 2:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}

void Controller::work() {
  if (!port_opt.has_value()) {
    std::cerr << "Port not set. Use setParams() before work().\n";
    return;
  }

  if (bindAndListen() != 0) {
    return;
  }
  std::cout << "Waiting for connection...\n";

  fd_set accept_fds;
  FD_ZERO(&accept_fds);
  FD_SET(listen_fd.value(), &accept_fds);

  struct timeval tv;
  tv.tv_sec = TTL;
  tv.tv_usec = 0;

  int sel_result =
      select(listen_fd.value() + 1, &accept_fds, nullptr, nullptr, &tv);

  if (sel_result == 0) {
    std::cerr << "No connection within " << TTL << " seconds. Exiting.\n";
    closeListenSocket();
    return;
  } else if (sel_result < 0) {
    std::cerr << "select error\n";
    closeListenSocket();
    return;
  }

  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  FdType client_fd_raw =
      accept(listen_fd.value(),
             reinterpret_cast<struct sockaddr*>(&client_addr), &addr_len);
  if (client_fd_raw < EDGE_CASE) {
    std::cerr << "Accept failed\n";
    closeListenSocket();
    return;
  }
  client_fd = client_fd_raw;
  std::cout << "Connection accepted.\n";

  fd_set read_fds;
  struct timeval tv_read;

  while (true) {
    FD_ZERO(&read_fds);
    FD_SET(client_fd.value(), &read_fds);

    tv_read.tv_sec = timeout_sec;
    tv_read.tv_usec = 0;

    int select_result =
        select(client_fd.value() + 1, &read_fds, nullptr, nullptr, &tv_read);

    if (select_result < EDGE_CASE) {
      std::cerr << "Select error\n";
      break;
    }

    if (select_result == 0) {
      if (stats_changed) {
        printStats();
        resetChanged();
      }
      continue;
    }

    if (FD_ISSET(client_fd.value(), &read_fds)) {
      std::string msg;
      unsigned level;
      time_t msg_time;
      if (readOneMessage(client_fd.value(), msg, level, msg_time)) {
        const std::tm* tm_info = std::localtime(&msg_time);
        std::array<char, TIME_BUFFER_SIZE> time_buf;
        std::strftime(time_buf.data(), sizeof(time_buf), "%Y-%m-%d %H:%M:%S",
                      tm_info);
        std::cout << "[" << levelToString(level) << "] "
                  << "{" << time_buf.data() << "} " << msg << "\n";
        updateStats(msg, level, msg_time);
        if (total_msgs % msgs_limit == 0 && total_msgs > 0) {
          printStats();
          resetChanged();
        }
      } else {
        std::cerr << "Connection closed or read error.\n";
        break;
      }
    }
  }

  if (client_fd.has_value()) {
    close(client_fd.value());
    client_fd.reset();
  }
  closeListenSocket();
}

ErrorCode Controller::bindAndListen() noexcept {
  listen_fd = socket(AF_INET, SOCK_STREAM, TCP_VALUE);
  if (!listen_fd.has_value() || listen_fd.value() < 0) {
    std::cerr << "Socket creation failed\n";
    return 4;
  }

  int opt = 1;
  setsockopt(listen_fd.value(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_opt.value());
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listen_fd.value(), reinterpret_cast<struct sockaddr*>(&addr),
           sizeof(addr)) < EDGE_CASE) {
    std::cerr << "Bind failed\n";
    closeListenSocket();
    return 6;
  }

  if (listen(listen_fd.value(), 1) < EDGE_CASE) {
    std::cerr << "Listen failed\n";
    closeListenSocket();
    return 7;
  }

  std::cout << "Listening on port " << port_opt.value() << "...\n";
  return 0;
}

void Controller::closeListenSocket() noexcept {
  if (listen_fd.has_value()) {
    close(listen_fd.value());
    listen_fd.reset();
  }
}

bool Controller::readOneMessage(FdType fd, std::string& out_msg,
                                unsigned& out_level,
                                time_t& out_time) const noexcept {
  MsgHeader header;
  ssize_t received = 0;
  while (received < HEADERS_LENGTH) {
    ssize_t n = recv(fd, reinterpret_cast<char*>(&header) + received,
                     HEADERS_LENGTH - received, 0);
    if (n <= 0) return false;
    received += n;
  }
  struct tm tm_info;
  tm_info.tm_sec = header.sec;
  tm_info.tm_min = header.min;
  tm_info.tm_hour = header.hour;
  tm_info.tm_mday = header.day;
  tm_info.tm_mon = header.month - MONTH_SHIFT;
  tm_info.tm_year = ntohs(header.year) - YEAR_SHIFT;
  tm_info.tm_isdst = -1;
  out_time = mktime(&tm_info);
  if (out_time == -1) {
    out_time = std::time(nullptr);
  }
  unsigned msg_len = header.full_msg_size - HEADERS_LENGTH;
  if (msg_len > MAX_BODY_LENGTH) {
    return false;
  }

  std::array<char, MAX_BODY_LENGTH> buffer;
  received = 0;
  while (received < msg_len) {
    ssize_t n = recv(fd, buffer.data() + received, msg_len - received, 0);
    if (n <= 0) return false;
    received += n;
  }

  out_msg.assign(buffer.data(), msg_len);
  out_level = header.log_lvl;
  return true;
}

void Controller::updateStats(std::string_view msg, unsigned level,
                             time_t msg_time) {
  ++total_msgs;
  if (level < LOGLEVEL_FIELDS_COUNT) ++level_counts[level];

  unsigned len = static_cast<unsigned>(msg.length());
  sum_lengths += len;
  if (len < min_length) min_length = len;
  if (len > max_length) max_length = len;

  recent_timestamps.push(msg_time);
  time_t now = std::time(nullptr);
  while (!recent_timestamps.empty() &&
         (now - recent_timestamps.front() > 3600)) {
    recent_timestamps.pop();
  }

  stats_changed = true;
}

void Controller::printStats() const noexcept {
  std::cout << "\n------------ STATISTICS ------------\n";
  std::cout << "Total messages: " << total_msgs << "\n";
  std::cout << "By level:\n";
  std::cout << "  DEBUG: " << level_counts[0] << "\n";
  std::cout << "  INFO : " << level_counts[1] << "\n";
  std::cout << "  ERROR: " << level_counts[2] << "\n";
  std::cout << "Last hour: " << recent_timestamps.size() << "\n";

  if (total_msgs > 0) {
    std::cout << "Length statistics:\n";
    std::cout << "  Min: " << min_length << "\n";
    std::cout << "  Max: " << max_length << "\n";
    double avg = static_cast<double>(sum_lengths) / total_msgs;
    std::cout << "  Avg: " << std::fixed << std::setprecision(2) << avg << "\n";
  } else {
    std::cout << "No messages yet.\n";
  }
  std::cout << "--------------------------------\n";
}

}  // namespace stat_controller