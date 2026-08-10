#ifndef I_LOGGER_HPP
#define I_LOGGER_HPP

#include <atomic>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <mutex>
#include <string_view>

namespace logging {
using ErrorCode = unsigned;

// std::uint8_t covers all 6 basic
// levels of logging (debug, info,
// warn, error, fatal).
enum class LogLevel : std::uint8_t {
  DEBUG,
  INFO,
  ERROR,

  // COUNT_FIELD is needed for
  // getting quantity of fields in
  // LogLevel.
  COUNT_FIELD
};
inline constexpr std::uint8_t LOGLVL_SIZE =
    static_cast<std::uint8_t>(LogLevel::COUNT_FIELD);

inline const std::string_view turnLogLevelIntoString(LogLevel lvl) {
  switch (lvl) {
    case LogLevel::DEBUG:
      return "DEBUG";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::ERROR:
      return "ERROR";
    default:
      return "Unknown log level";
  }
}

// ILogger is template because
// it's abstract class and derived class
// is undefined in compile time.
template <typename Derived>
class ILogger {
 public:
  static Derived& getInstance() { return Derived::getInstance(); }
  virtual inline void setLevel(LogLevel lvl) = 0;
  virtual ErrorCode log(std::string_view msg, LogLevel lvl) = 0;
  virtual inline LogLevel getLevel() const = 0;

  // Init function is up to child
  // because they differ.
  virtual void close() = 0;

  ILogger(ILogger const&) = delete;
  ILogger& operator=(ILogger const&) = delete;

 protected:
  // Constructor and destructor in derived classes
  // will default because of singleton.
  ILogger() = default;
  ~ILogger() = default;
};
}  // namespace logging

#endif  // I_LOGGER_HPP