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
            ErrorCode log(std::string_view msg, LogLevel lvl) override;
            ErrorCode init(std::string_view filename, LogLevel default_level);

            //Don't forget to close file
            void close() override{
                if (f_out.is_open()) {
                    f_out.close();
                }
            }
        private:
            LogLevel current_log_lvl = LogLevel::DEBUG;
            std::ofstream f_out;
            std::mutex mtx;

            FileLogger() = default;
            ~FileLogger() = default;
    };
} // namespace logging

namespace fl_api {
    inline logging::ErrorCode debug(std::string_view msg) {
        return logging::FileLogger::getInstance().log(msg, logging::LogLevel::DEBUG);
    }
    inline logging::ErrorCode info(std::string_view msg) {
        return logging::FileLogger::getInstance().log(msg, logging::LogLevel::INFO);
    }
    inline logging::ErrorCode error(std::string_view msg) {
        return logging::FileLogger::getInstance().log(msg, logging::LogLevel::ERROR);
    }
    inline logging::ErrorCode log(std::string_view msg, logging::LogLevel level) {
        return logging::FileLogger::getInstance().log(msg, level);
    }
    inline void setLevel(logging::LogLevel level) {
        logging::FileLogger::getInstance().setLevel(level);
    }
    inline logging::ErrorCode init(std::string_view filename, logging::LogLevel default_level) {
        return logging::FileLogger::getInstance().init(filename, default_level);
    }
    inline void close() {
        logging::FileLogger::getInstance().close();
    }
} // namespace fl_api

#endif // FILE_LOGGER_HPP