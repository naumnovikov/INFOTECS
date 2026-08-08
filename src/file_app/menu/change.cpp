#include "change.hpp"

namespace menu{
    logging::LogLevel Change::parseInput(Tokens& tokens){
        logging::LogLevel lvl = logging::LogLevel::DEBUG;
        std::string raw_lvl;
        turnStringIntoUpper(raw_lvl);
        if (raw_lvl == "DEBUG"){
            lvl = logging::LogLevel::DEBUG;
        }else if (raw_lvl == "INFO"){
            lvl = logging::LogLevel::INFO;
        }else if (raw_lvl == "ERROR"){
            lvl = logging::LogLevel::ERROR;
        }else{
            std::cout << "Unknown logging level. Log level will be set to debug.\n";
        }
        return lvl;
    }
    void Change::execute(Tokens& tokens){
        logging::LogLevel lvl = parseInput(tokens);
        fl_api::setLevel(lvl);
        std::cout << "Logging level was changed.";
    }
} // namespace menu