#ifndef STAT_CONTROLLER_HPP
#define STAT_CONTROLLER_HPP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <cstdint>
#include <ctime>
#include <limits>
#include <optional>
#include <queue>
#include <string_view>

namespace stat_controller {
using ErrorCode = unsigned;
using PortType = std::uint16_t;
using FdType = int;
inline constexpr unsigned LOGLEVEL_FIELDS_COUNT = 3;

class Controller {
 public:
  static Controller& getInstance() noexcept {
    static Controller instance;
    return instance;
  }
  inline void setParams(PortType p, unsigned N, unsigned T) noexcept {
    port_opt = p;
    msgs_limit = N;
    timeout_sec = T;
  }
  void work();

  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

 private:
  std::optional<PortType> port_opt;
  unsigned msgs_limit = 100;
  unsigned timeout_sec = 120;
  unsigned TTL = 120;

  // In future program can listen more clients
  // and get statistics from them all if needed.
  std::optional<FdType> listen_fd;
  std::optional<FdType> client_fd;

  unsigned total_msgs = 0;

  // Corresponds to the enum LogLevels' fields respectively
  // e.g DEBUG, INFO, ERROR
  std::array<unsigned, LOGLEVEL_FIELDS_COUNT> level_counts = {0, 0, 0};
  unsigned long long sum_lengths = 0;
  unsigned min_length = std::numeric_limits<unsigned>::max();
  unsigned max_length = 0;
  std::queue<std::time_t> recent_timestamps;
  bool stats_changed = false;

  ErrorCode bindAndListen() noexcept;
  void closeListenSocket() noexcept;
  bool readOneMessage(FdType fd, std::string& out_msg, unsigned& out_level,
                      std::time_t& out_time) const noexcept;
  void updateStats(std::string_view msg, unsigned level, std::time_t msg_time);
  void printStats() const noexcept;
  inline void resetChanged() noexcept { stats_changed = false; }

  Controller() noexcept = default;
  ~Controller() noexcept = default;
};

}  // namespace stat_controller

namespace stat_controller_api {
inline void work() { stat_controller::Controller::getInstance().work(); }
inline void setParams(stat_controller::PortType p, unsigned N,
                      unsigned T) noexcept {
  stat_controller::Controller::getInstance().setParams(p, N, T);
}
}  // namespace stat_controller_api

#endif  // STAT_CONTROLLER_HPP