#ifndef CHANGE_HPP
#define CHANGE_HPP

#include "menu.hpp"

namespace menu{
class Change : public Menu {
    private:
        logging::LogLevel parseInput(Tokens& tokens);
 public:
  void execute(Tokens& tokens) override;
};
} // namespace menu

#endif  // CHANGE_HPP