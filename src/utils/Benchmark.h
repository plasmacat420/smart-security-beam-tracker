#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "../core/Grid.h"
#include "../core/SecurityLaser.h"
#include <chrono>
#include <string>
#include <vector>

struct BenchmarkResult {
    int gridSize;
    int deviceCount;
    int beamCount;
    long long executionTimeUs;
    size_t memoryBytes;
};

class Benchmark {
private:
    std::vector<BenchmarkResult> results;

public:
    // Run benchmarks
    void runSingleBenchmark(const Grid& grid);
    void runScalabilityTest(int minSize, int maxSize, int step, double density);
    void runDensityTest(int gridSize, double minDensity, double maxDensity, double step);

    // Results
    const std::vector<BenchmarkResult>& getResults() const { return results; }
    void clearResults() { results.clear(); }

    // Display
    std::string generateReport() const;
    std::string generateASCIIGraph() const;

    // Utility
    static long long measureExecutionTime(const Grid& grid);
    static size_t estimateMemoryUsage(const Grid& grid);
};

#endif
