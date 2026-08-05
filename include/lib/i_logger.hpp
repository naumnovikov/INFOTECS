#ifndef I_LOGGER_HPP
#define I_LOGGER_HPP

#include <string_view>
#include <iostream>
#include <ctime>
#include <atomic>
#include <mutex>

namespace logging{
    enum class LogLevel{
        DEBUG, 
        INFO, 
        ERROR
    };

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
            virtual void log(std::string_view msg, LogLevel lvl) = 0;

            ILogger(ILogger const&) = delete;
            ILogger& operator= (ILogger const&) = delete;
        protected:
            ILogger() = default;
            ~ILogger() = default;
    };
}

#endif // I_LOGGER_HPP