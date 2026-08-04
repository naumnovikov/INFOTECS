#include "file_logger.hpp"

#include <time.h>

namespace logging{
    FileLogger::FileLogger(std::string_view filename){
        f_out_opt.open(filename, std::ios::out);
    }
    void FileLogger::log(std::string_view msg, LogLevel lvl) {
        if (!f_out_opt.is_open() || lvl < current_log_lvl){
            return;
        }
        time_t my_time = time(NULL);
        f_out_opt << "[" << turnLogLevelIntoString(current_log_lvl) << "] " << "{" << ctime(&my_time) << "} " << msg;
    }
}