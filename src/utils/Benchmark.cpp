#include "Benchmark.h"
#include "ColorManager.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

void Benchmark::runSingleBenchmark(const Grid& grid) {
    BenchmarkResult result;
    result.gridSize = grid.getRows() * grid.getCols();

    SecurityLaser laser(grid);

    // Count devices
    result.deviceCount = laser.getTotalDevices();

    // Measure execution time
    auto start = std::chrono::high_resolution_clock::now();
    result.beamCount = laser.calculateBeams();
    auto end = std::chrono::high_resolution_clock::now();

    result.executionTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    result.memoryBytes = estimateMemoryUsage(grid);

    results.push_back(result);
}

void Benchmark::runScalabilityTest(int minSize, int maxSize, int step, double density) {
    for (int size = minSize; size <= maxSize; size += step) {
        Grid grid(size, size);
        grid.randomize(density);
        runSingleBenchmark(grid);
    }
}

void Benchmark::runDensityTest(int gridSize, double minDensity, double maxDensity, double step) {
    for (double density = minDensity; density <= maxDensity; density += step) {
        Grid grid(gridSize, gridSize);
        grid.randomize(density);
        runSingleBenchmark(grid);
    }
}

std::string Benchmark::generateReport() const {
    std::ostringstream oss;

    oss << ColorManager::info("Benchmark Results:") << "\n";
    oss << std::string(80, '-') << "\n";
    oss << std::setw(12) << "Grid Size"
        << std::setw(12) << "Devices"
        << std::setw(12) << "Beams"
        << std::setw(20) << "Time (μs)"
        << std::setw(20) << "Memory (bytes)" << "\n";
    oss << std::string(80, '-') << "\n";

    for (const auto& result : results) {
        oss << std::setw(12) << result.gridSize
            << std::setw(12) << result.deviceCount
            << std::setw(12) << result.beamCount
            << std::setw(20) << result.executionTimeUs
            << std::setw(20) << result.memoryBytes << "\n";
    }

    oss << std::string(80, '-') << "\n";

    // Statistics
    if (!results.empty()) {
        long long totalTime = 0;
        long long maxTime = 0;
        long long minTime = results[0].executionTimeUs;

        for (const auto& result : results) {
            totalTime += result.executionTimeUs;
            maxTime = std::max(maxTime, result.executionTimeUs);
            minTime = std::min(minTime, result.executionTimeUs);
        }

        double avgTime = static_cast<double>(totalTime) / results.size();

        oss << "\n" << ColorManager::info("Statistics:") << "\n";
        oss << "  Average Time: " << std::fixed << std::setprecision(2) << avgTime << " μs\n";
        oss << "  Min Time: " << minTime << " μs\n";
        oss << "  Max Time: " << maxTime << " μs\n";
        oss << "  Total Tests: " << results.size() << "\n";
    }

    return oss.str();
}

std::string Benchmark::generateASCIIGraph() const {
    if (results.empty()) return "";

    std::ostringstream oss;
    const int graphHeight = 15;
    const int graphWidth = 60;

    oss << "\n" << ColorManager::info("Performance Graph (Time vs Grid Size):") << "\n\n";

    // Find max time for scaling
    long long maxTime = 0;
    for (const auto& result : results) {
        maxTime = std::max(maxTime, result.executionTimeUs);
    }

    // Create graph
    for (int row = graphHeight; row >= 0; row--) {
        long long threshold = (maxTime * row) / graphHeight;

        // Y-axis label
        oss << std::setw(8) << threshold << " |";

        // Plot points
        for (size_t i = 0; i < results.size() && i < static_cast<size_t>(graphWidth); i++) {
            long long time = results[i].executionTimeUs;
            long long level = (time * graphHeight) / maxTime;

            if (level >= row) {
                oss << ColorManager::beam("█");
            } else {
                oss << " ";
            }
        }
        oss << "\n";
    }

    // X-axis
    oss << "         +" << std::string(std::min(results.size(), static_cast<size_t>(graphWidth)), '-') << "\n";
    oss << "          Grid Size →\n";

    return oss.str();
}

long long Benchmark::measureExecutionTime(const Grid& grid) {
    SecurityLaser laser(grid);

    auto start = std::chrono::high_resolution_clock::now();
    laser.calculateBeams();
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

size_t Benchmark::estimateMemoryUsage(const Grid& grid) {
    // Estimate memory for grid storage
    size_t gridMemory = grid.getRows() * grid.getCols() * sizeof(char);

    // Add overhead for vectors and objects
    size_t overhead = sizeof(Grid) + sizeof(SecurityLaser);

    return gridMemory + overhead;
}
