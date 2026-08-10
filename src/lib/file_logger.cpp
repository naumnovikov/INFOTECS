#include "file_logger.hpp"

#include <iomanip>

namespace logging {

// In case init wasn't used before logging,
// it just wouldn't log to file because
// !f_out.is_open() returns true.
ErrorCode FileLogger::init(std::string_view filename, LogLevel default_level) {
  if (filename.empty()) {
    return 3;
  }
  std::string filename_str(filename);
  f_out.open(filename_str, std::ios::out | std::ios::app);
  if (!f_out) {
    return 3;
  }
  setLevel(default_level);
  return 0;
}
ErrorCode FileLogger::log(std::string_view msg, LogLevel lvl) {
  if (!f_out.is_open()) {
    return 1;
  }
  if (lvl < current_log_lvl) {
    return 0;
  }
  std::time_t t = std::time(nullptr);
  std::tm local_time;
  localtime_r(&t, &local_time);
  std::lock_guard<std::mutex> lg(mtx);
  f_out << "[" << turnLogLevelIntoString(lvl) << "] " << "{"
        << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "} " << msg
        << "\n";
  return 0;
}
}  // namespace logging