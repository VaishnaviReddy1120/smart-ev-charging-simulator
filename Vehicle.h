#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

class Vehicle {
private:
    int vehicleId;
    std::string vehicleType;
    int batteryLevel;
    int priorityScore;
    int estimatedChargingTime;
    bool critical; // true if battery < 20%

public:
    Vehicle();
    Vehicle(int id, std::string type, int battery);
    void calculatePriority();
    void calculateChargingTime();

    int getVehicleId() const;
    int getBatteryLevel() const;
    std::string getVehicleType() const;
    int getPriorityScore() const;
    int getEstimatedChargingTime() const;
    bool isCritical() const;

    void setChargingTime(int time);

    void displayVehicle() const;

    bool operator<(const Vehicle& other) const {
        return priorityScore < other.priorityScore;
    }
};

#endif
