#ifndef HELP_HPP
#define HELP_HPP

#include "menu.hpp"

namespace menu{
class Help : public Menu {
 public:
  void execute(Tokens& tokens) override;
};
} // namespace menu

#endif  // HELP_HPP