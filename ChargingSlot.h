#ifndef CHARGINGSLOT_H
#define CHARGINGSLOT_H

#include "Vehicle.h"

class ChargingSlot {
private:
    bool occupied;
    Vehicle assignedVehicle;
    int remainingTime;
    double powerAllocated; // kW allocated to this slot

public:
    ChargingSlot();
    bool isFree() const;
    void assignVehicle(Vehicle vehicle);
    void tick(int timeUnits = 1);
    void freeSlot();
    int getRemainingTime() const;
    Vehicle getAssignedVehicle() const;
    void setPowerAllocated(double power);
    double getPowerAllocated() const;
    void displaySlot() const;
};

#endif
