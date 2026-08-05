#ifndef SOCKET_LOGGER_HPP
#define SOCKET_LOGGER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <optional>
#include <cstdint>
#include "i_logger.hpp"

namespace logging{
    using FdType = int;
    using PortType = std::uint16_t;
    using IpType = std::uint32_t;

    class SocketLogger : public ILogger<SocketLogger>{
        public:
            static SocketLogger& getInstance(){
                static SocketLogger instance;
                return instance;
            }
            inline void setLevel(LogLevel lvl) override {
                current_log_lvl = lvl;
            }
            void log(std::string_view msg, LogLevel lvl) override;

            //Don't forget to close socket
            void closeSocket() {
                if (fd_opt.has_value()){
                    close(fd_opt.value());
                    fd_opt.reset();
                }
            }
        private:
            std::atomic<LogLevel> current_log_lvl = LogLevel::DEBUG;
            std::optional<FdType> fd_opt;
            std::mutex mtx;

            SocketLogger(IpType ip, PortType port, LogLevel default_level);

            SocketLogger() = default;
            ~SocketLogger() = default;
    };
}

#endif // SOCKET_LOGGER_HPP