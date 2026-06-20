#ifndef CHARGINGSTATION_H
#define CHARGINGSTATION_H

#include <vector>
#include <queue>
#include <string>
#include "Vehicle.h"
#include "ChargingSlot.h"
#include "Logger.h"

class ChargingStation {
private:
    int stationId;
    bool online;                  // Fault tolerance: is station active?
    double maxPowerCapacity;      // Energy load balancing: total kW
    std::vector<ChargingSlot> slots;
    std::priority_queue<Vehicle> waitingQueue;

    // Analytics
    int totalVehiclesServed;
    int totalWaitTime;
    double totalRevenue;

    // Pricing
    double calculateCurrentRate() const;

    // Load balancing
    void rebalancePower();

    Logger* logger;

public:
    ChargingStation(int id, int numberOfSlots, double maxPower, Logger* log);

    void assignVehicle(Vehicle vehicle);
    void tick(int timeUnits = 1);

    // Fault tolerance
    void goOffline();
    void goOnline();
    bool isOnline() const;

    // Getters
    int getStationId() const;
    int getMinimumWaitingTime() const;
    int getWaitingQueueSize() const;
    double getCurrentRate() const;
    double getTotalRevenue() const;
    int getTotalVehiclesServed() const;
    double getAverageWaitTime() const;

    // Extract all vehicles (for fault tolerance rerouting)
    std::vector<Vehicle> evacuateVehicles();

    void displayStatus() const;
    void displayAnalytics() const;
};

#endif
