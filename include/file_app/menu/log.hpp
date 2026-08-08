#ifndef LOG_HPP
#define LOG_HPP

#include "menu.hpp"

namespace menu{
    using msg = std::string;
    class Log : public Menu {
        public:
            inline Log(BusinessWorker& worker_param) : worker(worker_param) {}
            void execute(Tokens& tokens) override;
        private:
            BusinessWorker& worker;
            
            std::pair<msg, logging::LogLevel> parseInput(Tokens& tokens);
    };
} // namespace menu

#endif  // LOG_HPP