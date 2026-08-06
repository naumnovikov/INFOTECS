#include "controller.hpp"
#include <string>

namespace cli_utils {
    logging::ErrorCode checkLvl(std::int8_t lvl){
        if (lvl >= logging::LOGLVL_SIZE || lvl < 0){
            std::cerr << "Wrong log level.\n";
            return 3;
        }
        return 0;
    }
} // namespace cli_utils

void Controller::interact(){
    std::cout << "------------Init a file logger------------\n";
    std::int8_t lvl_input;
    do{
        std::cout << "Input default log level: ";
        std::cin >> lvl_input;
    }while (cli_utils::checkLvl(lvl_input) != 0);
    logging::LogLevel lvl = static_cast<logging::LogLevel>(static_cast<std::uint8_t>(lvl_input));
    std::string filename;
    do{
        std::cout << "Input filename for logging: ";
        std::cin >> filename;
    }while (fl_api::init(filename, lvl) != 0);

    // обработчик лога (Уровень важности может отсутствовать)
    // при выходе закрываем дескриптор 
}