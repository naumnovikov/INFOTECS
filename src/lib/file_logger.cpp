#include "file_logger.hpp"
#include <iomanip>

namespace logging{
    FileLogger::FileLogger(std::string_view filename, LogLevel default_level){
        f_out.open(filename, std::ios::out);
        if (!f_out){
            std::cerr << "Can't open a file.\n";
            return;
        }
        setLevel(default_level);
    }
    void FileLogger::log(std::string_view msg, LogLevel lvl) {
        if (!f_out.is_open()) {
            std::cerr << "Can't log to file.\n";
            return;
        }
        if (lvl < current_log_lvl) {
            return;
        }
        std::time_t t = std::time(nullptr);
        std::tm local_time;
        localtime_r(&t, &local_time); 
        std::lock_guard<std::mutex> lg(mtx);
        f_out << "[" << turnLogLevelIntoString(lvl) << "] " << "{" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "} " << msg << "\n";
    }
}