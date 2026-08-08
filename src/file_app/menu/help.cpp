#include "help.hpp"

namespace menu{
    void Help::execute(Tokens& tokens){
        std::cout << "------------Commands------------\n";
        std::cout << "CHANGE - changle default logging level;\n";
        std::cout << "LOG - log;\n";
        std::cout << "HELP - print commands;\n";
        std::cout << "EXIT - exit.\n";
        std::cout << "--------------------------------\n";
        std::cout << "------------HOW TO USE------------\n";
        std::cout << "Few things you need to know:\n";
        std::cout << "- command parser is case-insensitive;\n";
        std::cout << "- extra arguments will be skiped;\n";
        std::cout << "- extra spaces will be skiped;\n";
        std::cout << "- you can use LOG without level of log.\n";
        std::cout << "Possible variants:\n";
        std::cout << "  log \"Log message\" DEBUG\n";
        std::cout << "  log \"Log message\"\n";
        std::cout << "  LOG \"Log message\" deBug\n";
        std::cout << "  exit\n";
        std::cout << "  help\n";
        std::cout << "  change debug\n";
        std::cout << "--------------------------------\n";
    }
} // namespace menu