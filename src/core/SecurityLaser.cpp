#include "SecurityLaser.h"
#include <algorithm>

SecurityLaser::SecurityLaser(const Grid& g) : grid(g), totalBeams(0) {}

int SecurityLaser::calculateBeams() {
    int prev = 0;
    int ans = 0;

    const auto& data = grid.getData();
    for (const std::string& s : data) {
        int count = 0;
        for (char c : s) {
            if (c == '1') {
                count++;
            }
        }
        if (count != 0) {
            ans += (prev * count);
            prev = count;
        }
    }

    totalBeams = ans;
    return ans;
}

void SecurityLaser::calculateBeamsWithTracking() {
    beams.clear();
    totalBeams = 0;

    const auto& data = grid.getData();
    int rows = grid.getRows();
    int cols = grid.getCols();

    // Find the previous row with devices
    int prevRow = -1;
    std::vector<int> prevDevicePositions;

    for (int i = 0; i < rows; i++) {
        std::vector<int> currentDevicePositions;

        // Find all devices in current row
        for (int j = 0; j < cols; j++) {
            if (data[i][j] == '1') {
                currentDevicePositions.push_back(j);
            }
        }

        // If current row has devices and there's a previous row with devices
        if (!currentDevicePositions.empty() && prevRow != -1) {
            // Create beams between all devices in prev row and current row
            for (int prevCol : prevDevicePositions) {
                for (int currCol : currentDevicePositions) {
                    BeamInfo beam;
                    beam.row1 = prevRow;
                    beam.col1 = prevCol;
                    beam.row2 = i;
                    beam.col2 = currCol;
                    beams.push_back(beam);
                    totalBeams++;
                }
            }
        }

        // Update previous row info if current row has devices
        if (!currentDevicePositions.empty()) {
            prevRow = i;
            prevDevicePositions = currentDevicePositions;
        }
    }
}

std::vector<SecurityLaser::CalculationStep> SecurityLaser::getCalculationSteps() const {
    std::vector<CalculationStep> steps;

    const auto& data = grid.getData();
    int rows = grid.getRows();
    int cols = grid.getCols();

    int prevRow = -1;
    int prevDeviceCount = 0;
    std::vector<int> prevDevicePositions;

    for (int i = 0; i < rows; i++) {
        CalculationStep step;
        step.currentRow = i;
        step.devicesInRow = 0;
        step.previousDevices = prevDeviceCount;
        step.beamsAdded = 0;

        // Count devices and find positions in current row
        std::vector<int> currentDevicePositions;
        for (int j = 0; j < cols; j++) {
            if (data[i][j] == '1') {
                step.devicesInRow++;
                currentDevicePositions.push_back(j);
            }
        }

        // Calculate beams added
        if (step.devicesInRow > 0 && prevRow != -1) {
            step.beamsAdded = prevDeviceCount * step.devicesInRow;

            // Track new beams
            for (int prevCol : prevDevicePositions) {
                for (int currCol : currentDevicePositions) {
                    BeamInfo beam;
                    beam.row1 = prevRow;
                    beam.col1 = prevCol;
                    beam.row2 = i;
                    beam.col2 = currCol;
                    step.newBeams.push_back(beam);
                }
            }
        }

        steps.push_back(step);

        // Update previous row info
        if (step.devicesInRow > 0) {
            prevRow = i;
            prevDeviceCount = step.devicesInRow;
            prevDevicePositions = currentDevicePositions;
        }
    }

    return steps;
}

int SecurityLaser::getTotalDevices() const {
    int count = 0;
    const auto& data = grid.getData();
    for (const auto& row : data) {
        for (char c : row) {
            if (c == '1') count++;
        }
    }
    return count;
}

double SecurityLaser::getAverageBeamsPerDevice() const {
    int devices = getTotalDevices();
    if (devices == 0) return 0.0;
    return static_cast<double>(totalBeams) / devices;
}
