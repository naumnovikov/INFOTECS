#include "help.hpp"

#include <iostream>

namespace menu {
void Help::execute(Tokens&) {
  std::cout << "\n------------COMMANDS------------\n";
  std::cout << "CHANGE - change default logging level;\n";
  std::cout << "LOG - log a message;\n";
  std::cout << "HELP - print this help;\n";
  std::cout << "EXIT - exit the program.\n";
  std::cout << "--------------------------------\n";
  std::cout << "------------EXISTING LOGGING LEVELS------------\n";
  std::cout << "0) DEBUG;\n";
  std::cout << "1) INFO;\n";
  std::cout << "2) ERROR.\n";
  std::cout << "--------------------------------\n";
  std::cout << "------------HOW TO USE------------\n";
  std::cout << "Few things you need to know:\n";
  std::cout << "- command parser is case-insensitive;\n";
  std::cout << "- extra arguments will be skipped (accept from LOG command);\n";
  std::cout << "- extra spaces will be skipped;\n";
  std::cout << "- you can use LOG without specifying level (default: DEBUG);\n";
  std::cout << "- if file already exists, program continues writing there;\n";
  std::cout << "- if file doesn't exist, program creates it.\n";
  std::cout << "\nPossible variants:\n";
  std::cout << "  LOG \"Your message here\" DEBUG\n";
  std::cout << "  LOG \"Your message here\"\n";
  std::cout << "  LOG YourMessage INFO\n";
  std::cout << "  LOG YourMessage\n";
  std::cout << "  CHANGE INFO\n";
  std::cout << "  CHANGE debug\n";
  std::cout << "  HELP\n";
  std::cout << "  EXIT\n";
  std::cout << "--------------------------------\n";
}
}  // namespace menu