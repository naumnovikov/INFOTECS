#include "log.hpp"

namespace menu{
    std::pair<msg, logging::LogLevel> Log::parseInput(Tokens& tokens){
        std::size_t tokens_size = tokens.size();
        if (tokens[1][0] != '"'){
            std::cout << "Missing starting \".\n";
            std::cout << "Program will parse as there is \"\n";
            tokens[1] = '"' + tokens[1];
        }
        msg message;
        logging::LogLevel lvl = logging::LogLevel::DEBUG;
        std::string raw_lvl;
        for (std::size_t i = 1; i < tokens_size; ++i){ 
            message += tokens[i] + ' ';
            if (tokens[i][tokens_size-1] == '"' && i < tokens_size-2){
                raw_lvl = tokens[i+1];
            }
        }
        message.pop_back();
        turnStringIntoUpper(raw_lvl);
        if (raw_lvl == "INFO"){
            lvl = logging::LogLevel::INFO;
        }else if (raw_lvl == "ERROR"){
            lvl = logging::LogLevel::ERROR;
        }else{
            std::cout << "Unknown logging lvl. Message will be logged on DEBUG.\n";
        }
        return {message, lvl};
    }
    void Log::execute(Tokens& tokens){
        std::pair<msg, logging::LogLevel> p = parseInput(tokens);
        worker.addTask([p](){fl_api::log(p.first, p.second);});
        std::cout << "Message logged.\n";
    }
} // namespace menu