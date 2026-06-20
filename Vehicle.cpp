#include "Vehicle.h"
#include <iostream>

Vehicle::Vehicle() {
    vehicleId = 0;
    vehicleType = "None";
    batteryLevel = 0;
    priorityScore = 0;
    estimatedChargingTime = 0;
    critical = false;
}

Vehicle::Vehicle(int id, std::string type, int battery) {
    vehicleId = id;
    vehicleType = type;
    batteryLevel = battery;
    critical = (battery < 20);
    calculatePriority();
    calculateChargingTime();
}

void Vehicle::calculatePriority() {
    int typeWeight = 0;
    if (vehicleType == "Emergency")
        typeWeight = 50;
    else if (vehicleType == "Commercial")
        typeWeight = 30;
    else
        typeWeight = 20;

    // Critical battery gives extra priority boost
    int criticalBoost = critical ? 20 : 0;
    priorityScore = typeWeight + (100 - batteryLevel) + criticalBoost;
}

void Vehicle::calculateChargingTime() {
    estimatedChargingTime = 100 - batteryLevel;
}

int Vehicle::getVehicleId() const { return vehicleId; }
std::string Vehicle::getVehicleType() const { return vehicleType; }
int Vehicle::getBatteryLevel() const { return batteryLevel; }
int Vehicle::getPriorityScore() const { return priorityScore; }
int Vehicle::getEstimatedChargingTime() const { return estimatedChargingTime; }
bool Vehicle::isCritical() const { return critical; }

void Vehicle::setChargingTime(int time) {
    estimatedChargingTime = time;
}

void Vehicle::displayVehicle() const {
    std::cout << "  Vehicle ID      : " << vehicleId << std::endl;
    std::cout << "  Type            : " << vehicleType << std::endl;
    std::cout << "  Battery Level   : " << batteryLevel << "%" << std::endl;
    std::cout << "  Priority Score  : " << priorityScore << std::endl;
    std::cout << "  Charging Time   : " << estimatedChargingTime << " mins" << std::endl;
    if (critical)
        std::cout << "  [!] CRITICAL BATTERY - High Priority!" << std::endl;
}
