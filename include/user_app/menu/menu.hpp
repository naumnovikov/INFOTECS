#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <vector>

#include "business_worker.hpp"
#include "file_logger.hpp"
#include "socket_logger.hpp"

namespace menu {
using Tokens = std::vector<std::string>;
using BusinessWorker = business_worker::BusinessWorker;

class Menu {
 public:
  virtual ~Menu() = default;
  virtual void execute(Tokens& tokens) = 0;
};

inline void turnStringIntoUpper(std::string& str) noexcept {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::toupper(c); });
}
}  // namespace menu

#endif  // MENU_HPP