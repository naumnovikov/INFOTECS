#include "controller.hpp"
#include <string>

void controller_api::Controller::interact(){
    // TO DO:
    // - рефакторинг 
    std::cout << "------------Init a file logger------------\n";
    std::int8_t lvl_input;
    std::cout << "Input default log level: ";
    std::cin >> lvl_input;
    while (logging::checkLvl(lvl_input) != 0){
        std::cerr << "Wrong input.\n";
        std::cin >> lvl_input;
    }
    logging::LogLevel lvl = static_cast<logging::LogLevel>(static_cast<std::uint8_t>(lvl_input));
    std::string filename;
    std::cout << "Input filename for logging: ";
    while (fl_api::init(filename, lvl) != 0){
        std::cerr << "Wrong input.\n";
        std::cin >> filename;
    }
    business_worker::BusinessWorker worker;
    while (true){
        std::cout << "------------Commands------------\n";
        std::cout << "1) changle default logging level;\n";
        std::cout << "2) log;\n";
        std::cout << "3) exit.\n";
        std::cout << "--------------------------------\n";
        std::cout << "Input type of command: ";
        std::int8_t command_input;
        std::cin >> command_input;
        while (!std::cin || std::cin.peek() != '\n' || command_input < 1 || command_input > 3) {
            std::cerr << "Wrong input.\n";
            std::cin >> command_input;
        }
        switch (command_input){
            case 1:{
                std::int8_t lvl_input;
                std::cout << "Input new default log level: ";
                std::cin >> lvl_input;
                while (logging::checkLvl(lvl_input) != 0){
                    std::cerr << "Wrong input.\n";
                    std::cin >> lvl_input;
                }
                fl_api::setLevel(static_cast<logging::LogLevel>(static_cast<std::uint8_t>(lvl_input)));
            }
            case 2:{
                // TO DO:
                // - сделать возможным логирование без лвла
                std::int8_t lvl_input;
                std::cout << "Input log level: ";
                std::cin >> lvl_input;
                while (logging::checkLvl(lvl_input) != 0){
                    std::cerr << "Wrong input.\n";
                    std::cin >> lvl_input;
                }
                std::cout << "Input log message: ";
                std::string msg;
                std::getline(std::cin, msg);
                worker.addTask([msg, lvl_input](){fl_api::log(msg, static_cast<logging::LogLevel>(static_cast<std::uint8_t>(lvl_input)));});
            }
            case 3:
                std::cout << "Exiting...";
                worker.stop();
                fl_api::close();
                break; 
        }
    }
}