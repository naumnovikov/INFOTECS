#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "worker.hpp"
#include "file_logger.hpp"

class Controller{
    public:
        Controller& getInstance(){
            static Controller instance;
            return instance; 
        }
        void interact();

        Controller(Controller const&) = delete;
        Controller& operator= (Controller const&) = delete;
    private:
        Worker worker;

        Controller() = default;
        ~Controller() = default;
};

#endif // CONTROLLER_HPP