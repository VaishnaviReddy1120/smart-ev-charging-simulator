#include "NetworkManager.h"
#include <iostream>
#include <climits>

NetworkManager::NetworkManager(Logger* log) {
    logger = log;
    simulationTime = 0;
}

void NetworkManager::addStation(ChargingStation station) {
    stations.push_back(station);
}

// ─── Smart Routing ────────────────────────────────────────────────────────
void NetworkManager::smartRoute(Vehicle vehicle) {
    if (logger)
        logger->log("Smart routing Vehicle " + std::to_string(vehicle.getVehicleId()) +
                    " [" + vehicle.getVehicleType() + "] score=" +
                    std::to_string(vehicle.getPriorityScore()));

    int bestIndex = -1;
    int minWait = INT_MAX;

    for (int i = 0; i < (int)stations.size(); i++) {
        if (!stations[i].isOnline()) continue;
        int wait = stations[i].getMinimumWaitingTime();
        if (wait < minWait) {
            minWait = wait;
            bestIndex = i;
        }
    }

    if (bestIndex != -1) {
        if (logger)
            logger->log("--> Routed to Station " +
                        std::to_string(stations[bestIndex].getStationId()) +
                        " (wait: " + std::to_string(minWait) + " mins)");
        stations[bestIndex].assignVehicle(vehicle);
    } else {
        std::cout << "  [ERROR] No online stations available!" << std::endl;
        if (logger) logger->log("[ERROR] No online stations available for routing!");
    }
}

// ─── Tick All ─────────────────────────────────────────────────────────────
void NetworkManager::tickAll(int timeUnits) {
    simulationTime += timeUnits;
    if (logger) logger->setTime(simulationTime);

    if (logger)
        logger->log("--- Advancing simulation by " +
                    std::to_string(timeUnits) + " min(s) ---");

    for (auto& station : stations) {
        station.tick(timeUnits);
    }
}

// ─── Fault Tolerance ──────────────────────────────────────────────────────
void NetworkManager::simulateStationFailure(int stationId) {
    for (auto& station : stations) {
        if (station.getStationId() == stationId) {
            station.goOffline();

            // Evacuate all vehicles and reroute them
            std::vector<Vehicle> evacuated = station.evacuateVehicles();

            if (evacuated.empty()) {
                std::cout << "  No vehicles to reroute from Station "
                          << stationId << std::endl;
                return;
            }

            std::cout << "\n  [FAULT TOLERANCE] Rerouting "
                      << evacuated.size()
                      << " vehicle(s) from Station "
                      << stationId << "..." << std::endl;

            for (auto& v : evacuated) {
                smartRoute(v);
            }
            return;
        }
    }
    std::cout << "  Station " << stationId << " not found!" << std::endl;
}

void NetworkManager::restoreStation(int stationId) {
    for (auto& station : stations) {
        if (station.getStationId() == stationId) {
            station.goOnline();
            return;
        }
    }
}

// ─── Analytics ────────────────────────────────────────────────────────────
void NetworkManager::displayNetworkAnalytics() const {
    std::cout << "\n========== NETWORK ANALYTICS REPORT ==========" << std::endl;

    int totalServed = 0;
    double totalRevenue = 0.0;

    for (const auto& station : stations) {
        station.displayAnalytics();
        totalServed += station.getTotalVehiclesServed();
        totalRevenue += station.getTotalRevenue();
    }

    std::cout << "\n  ========== OVERALL SUMMARY ==========" << std::endl;
    std::cout << "  Total Vehicles Served : " << totalServed << std::endl;
    std::cout << "  Total Network Revenue : Rs." << (int)totalRevenue << std::endl;
    std::cout << "  Simulation Time       : " << simulationTime << " mins" << std::endl;
    std::cout << "==============================================" << std::endl;
}

// ─── Display ──────────────────────────────────────────────────────────────
void NetworkManager::displayNetwork() const {
    std::cout << "\n========== NETWORK STATUS [T="
              << simulationTime << " min] ==========" << std::endl;
    for (const auto& station : stations) {
        station.displayStatus();
    }
    std::cout << "=============================================" << std::endl;
}
