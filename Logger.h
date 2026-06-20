#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger {
private:
    std::ofstream logFile;
    int simulationTime; // track current sim time

public:
    Logger(const std::string& filename);
    ~Logger();
    void log(const std::string& message);
    void setTime(int time);
};

#endif
