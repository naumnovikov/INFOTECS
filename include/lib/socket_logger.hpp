#ifndef SOCKET_LOGGER_HPP
#define SOCKET_LOGGER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <optional>
#include "i_logger.hpp"

namespace logging{
    using FdType = int;
    using PortType = uint16_t;

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
            void close() override {
                // закрытие fd
            }
        private:
            LogLevel current_log_lvl = LogLevel::DEBUG;
            std::optional<FdType> fd_opt;

            SocketLogger(PortType port);

            SocketLogger() = default;
            ~SocketLogger() = default;
    };
}

#endif // SOCKET_LOGGER_HPP