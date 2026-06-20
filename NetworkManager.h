#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <vector>
#include "ChargingStation.h"
#include "Vehicle.h"
#include "Logger.h"

class NetworkManager {
private:
    std::vector<ChargingStation> stations;
    Logger* logger;
    int simulationTime;

public:
    NetworkManager(Logger* log);

    void addStation(ChargingStation station);
    void smartRoute(Vehicle vehicle);
    void tickAll(int timeUnits = 1);

    // Fault tolerance
    void simulateStationFailure(int stationId);
    void restoreStation(int stationId);

    // Analytics
    void displayNetworkAnalytics() const;
    void displayNetwork() const;
};

#endif
