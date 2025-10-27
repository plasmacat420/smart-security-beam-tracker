#include "core/Grid.h"
#include "core/SecurityLaser.h"
#include "visualizer/UIManager.h"
#include "utils/ColorManager.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    try {
        ColorManager::initialize();

        // Check for command-line arguments
        if (argc > 1) {
            if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
                std::cout << ColorManager::info("Security Laser Beam Visualizer\n");
                std::cout << "\nUsage:\n";
                std::cout << "  " << argv[0] << " [options]\n\n";
                std::cout << "Options:\n";
                std::cout << "  --help, -h        Show this help message\n";
                std::cout << "  --version, -v     Show version information\n";
                std::cout << "  --file <path>     Load grid from file\n";
                std::cout << "  --demo            Run quick demo\n\n";
                std::cout << "Interactive Mode:\n";
                std::cout << "  Run without arguments to enter interactive mode\n\n";
                return 0;
            } else if (std::string(argv[1]) == "--version" || std::string(argv[1]) == "-v") {
                std::cout << ColorManager::info("Security Laser Beam Visualizer v1.0\n");
                std::cout << "Created as an enhanced portfolio project\n";
                std::cout << "Based on LeetCode: Number of Laser Beams in a Bank\n\n";
                return 0;
            } else if (std::string(argv[1]) == "--demo") {
                // Quick demo
                std::cout << ColorManager::info("Running Demo...\n\n");

                std::vector<std::string> demoGrid = {"011001", "000000", "010100", "001000"};
                Grid grid(demoGrid);
                SecurityLaser laser(grid);
                laser.calculateBeamsWithTracking();

                Renderer renderer;
                std::cout << renderer.renderHeader("Demo: Security Laser Beam Analysis");
                std::cout << renderer.renderWithStats(grid, laser);
                std::cout << renderer.renderLegend();

                std::cout << "\n" << ColorManager::success("Demo complete!");
                std::cout << " Run without --demo for interactive mode.\n\n";
                return 0;
            } else if (std::string(argv[1]) == "--file" && argc > 2) {
                Grid grid = FileIO::loadGrid(argv[2]);
                UIManager ui(grid);
                ui.run();
                return 0;
            }
        }

        // Default: Interactive mode
        UIManager ui;
        ui.run();

    } catch (const std::exception& e) {
        std::cerr << ColorManager::error("Fatal error: ") << e.what() << std::endl;
        return 1;
    }

    return 0;
}
