#include "user_controller.hpp"

void user_controller::Controller::initCommands(
    business_worker::BusinessWorker& worker) {
  commands["CHANGE"] = std::make_unique<menu::Change>();
  commands["LOG"] = std::make_unique<menu::Log>(worker);
  commands["HELP"] = std::make_unique<menu::Help>();
  commands["EXIT"] = std::make_unique<menu::Exit>(worker);
}
void user_controller::Controller::interact() {
  business_worker::BusinessWorker worker;
  initCommands(worker);
  if (commands.empty()) {
    std::cerr << "No commands initialized. Exiting.\n";
    return;
  }

  // stub used here because HELP
  // doesn't need any arguments.
  // It just sticks to menu interface.
  std::vector<std::string> stub;
  commands["HELP"]->execute(stub);
  while (true) {
    std::cout << ">> ";
    std::string command_input;
    std::getline(std::cin, command_input);
    while (command_input.empty()) {
      std::cerr << "Empty input.\n>> ";
      std::getline(std::cin, command_input);
    }
    menu::Tokens tokens = collectInputCommand(std::move(command_input));
    if (tokens.empty()) {
      std::cerr << "Spaces input.\n";
      continue;
    }
    std::string command_argument = tokens[0];
    menu::turnStringIntoUpper(command_argument);
    if (command_argument == "CHANGE") {
      if (tokens.size() == 1) {
        std::cout << "Too few arguments.\n";
      } else {
        commands["CHANGE"]->execute(tokens);
      }
    } else if (command_argument == "LOG") {
      if (tokens.size() == 1) {
        std::cout << "Too few arguments.\n";
      } else {
        commands["LOG"]->execute(tokens);
      }
    } else if (command_argument == "HELP") {
      commands["HELP"]->execute(stub);
    } else if (command_argument == "EXIT") {
      commands["EXIT"]->execute(stub);
      break;
    } else {
      std::cerr << "Unknown command.\n";
    }
  }
}