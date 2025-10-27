#include "../src/core/Grid.h"
#include "../src/core/SecurityLaser.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>

// Simple test framework
int totalTests = 0;
int passedTests = 0;

#define TEST(name) \
    void test_##name(); \
    void run_test_##name() { \
        totalTests++; \
        std::cout << "Running: " << #name << "... "; \
        try { \
            test_##name(); \
            passedTests++; \
            std::cout << "PASSED\n"; \
        } catch (const std::exception& e) { \
            std::cout << "FAILED: " << e.what() << "\n"; \
        } \
    } \
    void test_##name()

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        throw std::runtime_error(std::string("Expected ") + std::to_string(expected) + \
                                " but got " + std::to_string(actual)); \
    }

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition); \
    }

// Tests
TEST(grid_creation) {
    Grid grid(5, 5);
    ASSERT_EQ(5, grid.getRows());
    ASSERT_EQ(5, grid.getCols());
    ASSERT_TRUE(grid.isValid());
}

TEST(grid_from_data) {
    std::vector<std::string> data = {"011", "001", "110"};
    Grid grid(data);
    ASSERT_EQ(3, grid.getRows());
    ASSERT_EQ(3, grid.getCols());
    ASSERT_EQ('0', grid.getCell(0, 0));
    ASSERT_EQ('1', grid.getCell(0, 1));
}

TEST(grid_toggle) {
    Grid grid(3, 3);
    ASSERT_EQ('0', grid.getCell(1, 1));
    grid.toggleCell(1, 1);
    ASSERT_EQ('1', grid.getCell(1, 1));
    grid.toggleCell(1, 1);
    ASSERT_EQ('0', grid.getCell(1, 1));
}

TEST(grid_count_devices) {
    std::vector<std::string> data = {"011", "001", "110"};
    Grid grid(data);
    ASSERT_EQ(2, grid.countDevicesInRow(0));
    ASSERT_EQ(1, grid.countDevicesInRow(1));
    ASSERT_EQ(2, grid.countDevicesInRow(2));
}

TEST(basic_beam_calculation) {
    std::vector<std::string> data = {"011001", "000000", "010100", "001000"};
    Grid grid(data);
    SecurityLaser laser(grid);

    int beams = laser.calculateBeams();
    ASSERT_EQ(8, beams);
}

TEST(no_beams_same_row) {
    std::vector<std::string> data = {"000", "111", "000"};
    Grid grid(data);
    SecurityLaser laser(grid);

    int beams = laser.calculateBeams();
    ASSERT_EQ(0, beams);
}

TEST(maximum_beams) {
    std::vector<std::string> data = {"111", "000", "111"};
    Grid grid(data);
    SecurityLaser laser(grid);

    int beams = laser.calculateBeams();
    ASSERT_EQ(9, beams);
}

TEST(sparse_grid) {
    std::vector<std::string> data = {"10000", "00000", "00001", "00000", "01000"};
    Grid grid(data);
    SecurityLaser laser(grid);

    int beams = laser.calculateBeams();
    ASSERT_EQ(2, beams);
}

TEST(empty_grid) {
    std::vector<std::string> data = {"000", "000", "000"};
    Grid grid(data);
    SecurityLaser laser(grid);

    int beams = laser.calculateBeams();
    ASSERT_EQ(0, beams);
}

TEST(single_row) {
    std::vector<std::string> data = {"111"};
    Grid grid(data);
    SecurityLaser laser(grid);

    int beams = laser.calculateBeams();
    ASSERT_EQ(0, beams);
}

TEST(single_device_per_row) {
    std::vector<std::string> data = {"100", "010", "001"};
    Grid grid(data);
    SecurityLaser laser(grid);

    int beams = laser.calculateBeams();
    ASSERT_EQ(2, beams);
}

TEST(alternating_rows) {
    std::vector<std::string> data = {"11", "00", "11", "00", "11"};
    Grid grid(data);
    SecurityLaser laser(grid);

    int beams = laser.calculateBeams();
    ASSERT_EQ(8, beams); // 2*2 + 2*2 = 8
}

TEST(beam_tracking) {
    std::vector<std::string> data = {"11", "00", "11"};
    Grid grid(data);
    SecurityLaser laser(grid);

    laser.calculateBeamsWithTracking();
    const auto& beams = laser.getBeams();

    ASSERT_EQ(4, static_cast<int>(beams.size()));
    ASSERT_EQ(4, laser.getTotalBeams());
}

TEST(calculation_steps) {
    std::vector<std::string> data = {"11", "00", "11"};
    Grid grid(data);
    SecurityLaser laser(grid);

    auto steps = laser.getCalculationSteps();
    ASSERT_EQ(3, static_cast<int>(steps.size()));

    // Row 0: 2 devices, prev=0, beams=0
    ASSERT_EQ(2, steps[0].devicesInRow);
    ASSERT_EQ(0, steps[0].beamsAdded);

    // Row 1: 0 devices, prev=2, beams=0
    ASSERT_EQ(0, steps[1].devicesInRow);
    ASSERT_EQ(0, steps[1].beamsAdded);

    // Row 2: 2 devices, prev=2, beams=4
    ASSERT_EQ(2, steps[2].devicesInRow);
    ASSERT_EQ(4, steps[2].beamsAdded);
}

TEST(large_grid) {
    Grid grid(100, 100);
    grid.randomize(0.3);

    SecurityLaser laser(grid);
    int beams = laser.calculateBeams();

    // Should complete without error
    ASSERT_TRUE(beams >= 0);
}

TEST(total_devices) {
    std::vector<std::string> data = {"111", "000", "101"};
    Grid grid(data);
    SecurityLaser laser(grid);

    ASSERT_EQ(5, laser.getTotalDevices());
}

int main() {
    std::cout << "=== Security Laser Beam Test Suite ===\n\n";

    run_test_grid_creation();
    run_test_grid_from_data();
    run_test_grid_toggle();
    run_test_grid_count_devices();
    run_test_basic_beam_calculation();
    run_test_no_beams_same_row();
    run_test_maximum_beams();
    run_test_sparse_grid();
    run_test_empty_grid();
    run_test_single_row();
    run_test_single_device_per_row();
    run_test_alternating_rows();
    run_test_beam_tracking();
    run_test_calculation_steps();
    run_test_large_grid();
    run_test_total_devices();

    std::cout << "\n=== Test Results ===\n";
    std::cout << "Total: " << totalTests << "\n";
    std::cout << "Passed: " << passedTests << "\n";
    std::cout << "Failed: " << (totalTests - passedTests) << "\n";

    if (passedTests == totalTests) {
        std::cout << "\n✓ All tests passed!\n";
        return 0;
    } else {
        std::cout << "\n✗ Some tests failed!\n";
        return 1;
    }
}
