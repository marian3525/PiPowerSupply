#include "Logger.hpp"
#include <thread>

/**
 * Start another thread and start logging every interval milliseconds
 * @param interval Sample interval in ms
 */
void Logger::startLog(uint16_t interval) {
    /*
     * TODO: store a pointer to a function that will get the Y value, according to the Y mode from the controller
     *       same for X
     */
    worker = new std::thread([&](){
        float x, y;

        // take a sample
        while(!stopped) {
            //y = getY();
            usleep(static_cast<__useconds_t>(interval * 1000));
        }
    });
}

Logger::~Logger() {
    worker->join();
}
