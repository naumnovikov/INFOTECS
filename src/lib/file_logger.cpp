#include "file_logger.hpp"

namespace logging{
    FileLogger::FileLogger(std::string_view filename, LogLevel default_level){
        f_out_opt.open(filename, std::ios::out);
        if (!f_out_opt){
            std::cerr << "Can't open a file.\n";
            return;
        }
        setLevel(default_level);
    }
    void FileLogger::log(std::string_view msg, LogLevel lvl) {
        if (!f_out_opt.is_open()){
            std::cerr << "Can't log to file.\n";
            return;
        }
        if (lvl < current_log_lvl){
            return;
        }
        time_t my_time = time(NULL);
        f_out_opt << "[" << turnLogLevelIntoString(lvl) << "] " << "{" << ctime(&my_time) << "} " << msg;
    }
}