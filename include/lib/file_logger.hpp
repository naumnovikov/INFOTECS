#ifndef FILE_LOGGER_HPP
#define FILE_LOGGER_HPP

#include <fstream>
#include "i_logger.hpp"

namespace logging{
    class FileLogger : public ILogger<FileLogger>{
        public:
            static FileLogger& getInstance(){
                static FileLogger instance;
                return instance;
            }
            inline void setLevel(LogLevel lvl) override {
                current_log_lvl = lvl;
            }
            void log(std::string_view msg, LogLevel lvl) override;

            //Don't forget to close file
            void closeFile() {
                if (f_out.is_open()) {
                    f_out.close();
                }
            }
        private:
            std::atomic<LogLevel> current_log_lvl = LogLevel::DEBUG;
            std::ofstream f_out;
            std::mutex mtx;

            FileLogger(std::string_view filename, LogLevel default_level);

            FileLogger() = default;
            ~FileLogger() = default;
    };
}

#endif // FILE_LOGGER_HPP