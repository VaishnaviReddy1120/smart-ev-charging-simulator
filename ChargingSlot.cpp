#include "ChargingSlot.h"
#include <iostream>
#include <cmath>

ChargingSlot::ChargingSlot() {
    occupied = false;
    remainingTime = 0;
    powerAllocated = 0.0;
}

bool ChargingSlot::isFree() const {
    return !occupied;
}

void ChargingSlot::assignVehicle(Vehicle vehicle) {
    assignedVehicle = vehicle;
    occupied = true;
    remainingTime = vehicle.getEstimatedChargingTime();
}

void ChargingSlot::tick(int timeUnits) {
    if (occupied) {
        remainingTime -= timeUnits;
        if (remainingTime <= 0) {
            remainingTime = 0;
            occupied = false;
            std::cout << "  [DONE] Vehicle " << assignedVehicle.getVehicleId()
                      << " [" << assignedVehicle.getVehicleType()
                      << "] finished charging!" << std::endl;
        }
    }
}

void ChargingSlot::freeSlot() {
    occupied = false;
    remainingTime = 0;
    powerAllocated = 0.0;
}

int ChargingSlot::getRemainingTime() const { return remainingTime; }
Vehicle ChargingSlot::getAssignedVehicle() const { return assignedVehicle; }
void ChargingSlot::setPowerAllocated(double power) { powerAllocated = power; }
double ChargingSlot::getPowerAllocated() const { return powerAllocated; }

void ChargingSlot::displaySlot() const {
    if (occupied) {
        std::cout << "Occupied | Vehicle "
                  << assignedVehicle.getVehicleId()
                  << " [" << assignedVehicle.getVehicleType() << "]"
                  << " | Remaining: " << remainingTime << " mins"
                  << " | Power: " << powerAllocated << " kW" << std::endl;
    } else {
        std::cout << "Free" << std::endl;
    }
}
