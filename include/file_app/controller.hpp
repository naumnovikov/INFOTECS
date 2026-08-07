#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "business_worker.hpp"

namespace controller_api{
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
            Controller() = default;
            ~Controller() = default;
    };

    inline void interact() {
        Controller::getInstance().interact();
    }
} // namespace controller_api

#endif // CONTROLLER_HPP