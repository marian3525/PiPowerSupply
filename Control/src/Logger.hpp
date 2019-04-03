#include <utility>

#include <utility>

#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include "Controller.hpp"

/**
 * @brief The Logger class is used to store data for the purpose of plotting on the UI
 * The Logger can log a pair of values at a time, create multiple loggers if you need to log multiple pairs
 * The X and Y scale can be set
 * The logging interval is set from the controller
 */
class Logger
{
public:
    Logger(Controller* controller, float Xscale, std::string Xlabel, float Yscale, std::string Ylabel): Xscale{Xscale}, Xlabel{std::move(Xlabel)},
                                                                            Yscale{Yscale}, Ylabel{std::move(Ylabel)} {};
    void startLog(uint16_t interval);

private:
    friend class Controller;
    // used to get values directly
    Controller* controller = nullptr;

    float Xscale = 0;
    std::string Xlabel;

    float Yscale = 0;
    std::string Ylabel;

    // data storage
    vector<pair<float, float>> points;

    //
    bool stopped;
};

#endif // LOGGER_HPP
