#include "ChargingStation.h"
#include <iostream>
#include <climits>
#include <cmath>

ChargingStation::ChargingStation(int id, int numberOfSlots, double maxPower, Logger* log) {
    stationId = id;
    online = true;
    maxPowerCapacity = maxPower;
    logger = log;
    totalVehiclesServed = 0;
    totalWaitTime = 0;
    totalRevenue = 0.0;

    for (int i = 0; i < numberOfSlots; i++) {
        slots.push_back(ChargingSlot());
    }
}

// ─── Dynamic Pricing ───────────────────────────────────────────────────────
double ChargingStation::calculateCurrentRate() const {
    int occupied = 0;
    for (const auto& slot : slots) {
        if (!slot.isFree()) occupied++;
    }

    double rate = 10.0; // base rate ₹10/min

    if (!waitingQueue.empty())
        rate = 20.0; // surge: queue exists
    else if (occupied == (int)slots.size())
        rate = 15.0; // moderate: all slots full
    else if (occupied > 0)
        rate = 10.0; // normal

    return rate;
}

double ChargingStation::getCurrentRate() const {
    return calculateCurrentRate();
}

// ─── Energy Load Balancing ────────────────────────────────────────────────
void ChargingStation::rebalancePower() {
    int activeSlots = 0;
    for (auto& slot : slots) {
        if (!slot.isFree()) activeSlots++;
    }
    if (activeSlots == 0) return;

    double powerPerSlot = maxPowerCapacity / activeSlots;

    for (auto& slot : slots) {
        if (!slot.isFree()) {
            slot.setPowerAllocated(powerPerSlot);
            // Charging time scales inversely with power
            // Full power (maxPowerCapacity) = base time
            // Half power = double the time
            double speedFactor = powerPerSlot / maxPowerCapacity;
            int adjustedTime = (int)ceil(
                slot.getAssignedVehicle().getEstimatedChargingTime() / speedFactor
            );
            // We don't reset remaining time here to avoid jumps;
            // new vehicles get adjusted time at assignment
        }
    }

    if (logger) {
        logger->log("Station " + std::to_string(stationId) +
                    " rebalanced power: " + std::to_string(powerPerSlot) +
                    " kW per slot (" + std::to_string(activeSlots) + " active)");
    }
}

// ─── Assign Vehicle ───────────────────────────────────────────────────────
void ChargingStation::assignVehicle(Vehicle vehicle) {
    if (!online) {
        if (logger)
            logger->log("Station " + std::to_string(stationId) +
                        " is OFFLINE. Cannot accept Vehicle " +
                        std::to_string(vehicle.getVehicleId()));
        return;
    }

    // Range anxiety check
    if (vehicle.isCritical()) {
        if (logger)
            logger->log("[CRITICAL] Vehicle " + std::to_string(vehicle.getVehicleId()) +
                        " has low battery (<20%) - treating as high priority!");
    }

    for (auto& slot : slots) {
        if (slot.isFree()) {
            // Adjust charging time based on current load
            int activeSlots = 0;
            for (auto& s : slots) if (!s.isFree()) activeSlots++;
            double powerPerSlot = maxPowerCapacity / (activeSlots + 1);
            double speedFactor = powerPerSlot / maxPowerCapacity;
            int adjustedTime = (int)ceil(vehicle.getEstimatedChargingTime() / speedFactor);
            vehicle.setChargingTime(adjustedTime);

            slot.assignVehicle(vehicle);
            rebalancePower();

            double rate = calculateCurrentRate();
            double cost = rate * adjustedTime;
            totalRevenue += cost;

            if (logger)
                logger->log("Vehicle " + std::to_string(vehicle.getVehicleId()) +
                            " [" + vehicle.getVehicleType() + "] assigned to Station " +
                            std::to_string(stationId) +
                            " | Rate: Rs." + std::to_string((int)rate) + "/min" +
                            " | Est. Cost: Rs." + std::to_string((int)cost) +
                            " | Charging Time: " + std::to_string(adjustedTime) + " mins");
            return;
        }
    }

    // No free slot
    waitingQueue.push(vehicle);
    if (logger)
        logger->log("All slots full at Station " + std::to_string(stationId) +
                    ". Vehicle " + std::to_string(vehicle.getVehicleId()) +
                    " [" + vehicle.getVehicleType() + "] added to priority queue" +
                    " (score: " + std::to_string(vehicle.getPriorityScore()) + ")" +
                    " | SURGE Rate: Rs.20/min");
}

// ─── Tick ─────────────────────────────────────────────────────────────────
void ChargingStation::tick(int timeUnits) {
    if (!online) return;

    for (auto& slot : slots) {
        bool wasBusy = !slot.isFree();
        slot.tick(timeUnits);
        if (wasBusy && slot.isFree()) {
            totalVehiclesServed++;
        }
    }

    // Assign queued vehicles to freed slots
    for (auto& slot : slots) {
        if (slot.isFree() && !waitingQueue.empty()) {
            Vehicle next = waitingQueue.top();
            waitingQueue.pop();
            assignVehicle(next);
        }
    }

    rebalancePower();
}

// ─── Fault Tolerance ──────────────────────────────────────────────────────
void ChargingStation::goOffline() {
    online = false;
    if (logger)
        logger->log("[FAULT] Station " + std::to_string(stationId) +
                    " went OFFLINE!");
}

void ChargingStation::goOnline() {
    online = true;
    if (logger)
        logger->log("[RECOVERY] Station " + std::to_string(stationId) +
                    " is back ONLINE!");
}

bool ChargingStation::isOnline() const { return online; }

std::vector<Vehicle> ChargingStation::evacuateVehicles() {
    std::vector<Vehicle> evacuated;

    for (auto& slot : slots) {
        if (!slot.isFree()) {
            evacuated.push_back(slot.getAssignedVehicle());
            slot.freeSlot();
        }
    }

    while (!waitingQueue.empty()) {
        evacuated.push_back(waitingQueue.top());
        waitingQueue.pop();
    }

    return evacuated;
}

// ─── Getters ──────────────────────────────────────────────────────────────
int ChargingStation::getStationId() const { return stationId; }
int ChargingStation::getWaitingQueueSize() const { return (int)waitingQueue.size(); }
double ChargingStation::getTotalRevenue() const { return totalRevenue; }
int ChargingStation::getTotalVehiclesServed() const { return totalVehiclesServed; }
double ChargingStation::getAverageWaitTime() const {
    if (totalVehiclesServed == 0) return 0;
    return (double)totalWaitTime / totalVehiclesServed;
}

int ChargingStation::getMinimumWaitingTime() const {
    if (!online) return INT_MAX;
    int minTime = INT_MAX;
    for (const auto& slot : slots) {
        int t = slot.getRemainingTime();
        if (t < minTime) minTime = t;
    }
    return (minTime == INT_MAX) ? 0 : minTime;
}

// ─── Display ──────────────────────────────────────────────────────────────
void ChargingStation::displayStatus() const {
    std::cout << "\n===== Charging Station " << stationId;
    if (!online) std::cout << " [OFFLINE]";
    std::cout << " =====" << std::endl;
    std::cout << "  Max Power Capacity : " << maxPowerCapacity << " kW" << std::endl;
    std::cout << "  Current Rate       : Rs." << calculateCurrentRate() << "/min" << std::endl;

    for (int i = 0; i < (int)slots.size(); i++) {
        std::cout << "  Slot " << i + 1 << ": ";
        slots[i].displaySlot();
    }

    std::cout << "  Waiting Queue Size : " << waitingQueue.size() << std::endl;
    std::cout << "  Min Waiting Time   : " << getMinimumWaitingTime() << " mins" << std::endl;
}

void ChargingStation::displayAnalytics() const {
    std::cout << "\n  --- Station " << stationId << " Analytics ---" << std::endl;
    std::cout << "  Total Vehicles Served : " << totalVehiclesServed << std::endl;
    std::cout << "  Total Revenue         : Rs." << (int)totalRevenue << std::endl;
    std::cout << "  Avg Wait Time         : " << getAverageWaitTime() << " mins" << std::endl;
}
