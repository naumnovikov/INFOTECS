#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <unordered_map>
#include <memory>
#include <sstream>
#include "business_worker.hpp"
#include "change.hpp"
#include "exit.hpp"
#include "help.hpp"
#include "log.hpp"

namespace controller_api{
    using CommandArgument = std::string;

    class Controller{
        public:
            static Controller& getInstance(){
                static Controller instance;
                return instance; 
            }
            void interact();

            Controller(Controller const&) = delete;
            Controller& operator= (Controller const&) = delete;
        private:
            std::unordered_map<CommandArgument, std::unique_ptr<menu::Menu>> commands;

            void initCommands(business_worker::BusinessWorker& worker);
            Controller() = default;
            ~Controller() = default;
    };

    inline void interact() {
        Controller::getInstance().interact();
    }
    menu::Tokens collectInputCommand(std::string command_buffer) {
        menu::Tokens tokens;
        std::istringstream iss(std::move(command_buffer));
        std::string token;
        while (iss >> token) {
            if (!token.empty()){
            tokens.push_back(std::move(token));
            }
        }
        return tokens;
    }
    void initFl(){
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
    }
} // namespace controller_api

#endif // CONTROLLER_HPP