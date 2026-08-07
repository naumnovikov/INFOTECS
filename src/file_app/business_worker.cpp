#include "business_worker.hpp"

void business_worker::BusinessWorker::workerLoop(){
    while (true){
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> ul(mtx);
            cv.wait(ul, [this](){return !jobs.empty() || should_terminate;});
            if (should_terminate){
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}
void business_worker::BusinessWorker::addTask(const std::function<void()>& job){
    std::unique_lock<std::mutex> ul(mtx);
    jobs.push(job);
    cv.notify_one();
}
void business_worker::BusinessWorker::stop(){
    should_terminate = true;
    cv.notify_one();
}  