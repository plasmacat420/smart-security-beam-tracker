#ifndef SECURITY_LASER_H
#define SECURITY_LASER_H

#include "Grid.h"
#include <vector>
#include <utility>

struct BeamInfo {
    int row1, col1;  // First device
    int row2, col2;  // Second device
};

class SecurityLaser {
private:
    Grid grid;
    std::vector<BeamInfo> beams;
    int totalBeams;

public:
    SecurityLaser(const Grid& g);

    // Core algorithm
    int calculateBeams();

    // Detailed analysis
    void calculateBeamsWithTracking();
    const std::vector<BeamInfo>& getBeams() const { return beams; }
    int getTotalBeams() const { return totalBeams; }

    // Step-by-step for animation
    struct CalculationStep {
        int currentRow;
        int devicesInRow;
        int previousDevices;
        int beamsAdded;
        std::vector<BeamInfo> newBeams;
    };

    std::vector<CalculationStep> getCalculationSteps() const;

    // Statistics
    int getTotalDevices() const;
    double getAverageBeamsPerDevice() const;
};

#endif
