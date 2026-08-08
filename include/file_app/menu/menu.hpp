#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>
#include "business_worker.hpp"

namespace menu{
    using Tokens = std::vector<std::string>;  
    using BusinessWorker = business_worker::BusinessWorker;  

    class Menu {
    public:
    virtual void execute(Tokens& tokens) = 0;
    };

    void turnStringIntoUpper(std::string& str) {
        for (char& c : str) {
            c = std::toupper(c);
        }
    }
} // namespace menu

#endif  // MENU_HPP