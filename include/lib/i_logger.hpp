#ifndef I_LOGGER_HPP
#define I_LOGGER_HPP

#include <string_view>
#include <iostream>
#include <ctime>
#include <atomic>
#include <mutex>
#include <cstdint>

namespace logging{
    using ErrorCode = unsigned;

    // std::uint8_t covers all 6 basic
    // levels of logging (debug, info,
    // warn, error, fatal)
    enum class LogLevel : std::uint8_t{
        DEBUG, 
        INFO, 
        ERROR,
        COUNT_FIELD
    };
    inline constexpr std::uint8_t LOGLVL_SIZE = COUNT_FIELD;

    inline const std::string_view turnLogLevelIntoString(LogLevel lvl){
        switch (lvl){
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

    template <typename Derived>
    class ILogger{
        public:
            static Derived& getInstance(){
                return Derived::getInstance();
            }
            virtual inline void setLevel(LogLevel lvl) = 0;
            virtual ErrorCode log(std::string_view msg, LogLevel lvl) = 0;

            // Init function is up to child 
            // because they differ
            virtual void close() = 0;

            ILogger(ILogger const&) = delete;
            ILogger& operator= (ILogger const&) = delete;
        protected:
            ILogger() = default;
            ~ILogger() = default;
    };
} // namespace logging

#endif // I_LOGGER_HPP