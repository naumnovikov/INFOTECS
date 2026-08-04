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
            void close() override {
                if (f_out_opt.is_open()) {
                    f_out_opt.close();
                }
            }
        private:
            LogLevel current_log_lvl = LogLevel::DEBUG;
            std::ofstream f_out_opt;

            FileLogger(std::string_view filename);

            FileLogger() = default;
            ~FileLogger() = default;
    };
}

#endif // FILE_LOGGER_HPP