#include "Logger.h"
#include <iostream>
#include <ctime>

Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::out | std::ios::trunc);
    simulationTime = 0;
    if (logFile.is_open()) {
        logFile << "========================================" << std::endl;
        logFile << "   Smart EV Network Simulator - Log    " << std::endl;
        logFile << "========================================" << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open())
        logFile.close();
}

void Logger::setTime(int time) {
    simulationTime = time;
}

void Logger::log(const std::string& message) {
    std::string entry = "[T=" + std::to_string(simulationTime) + " min] " + message;
    if (logFile.is_open())
        logFile << entry << std::endl;
    // Also print to console
    std::cout << "  " << entry << std::endl;
}
