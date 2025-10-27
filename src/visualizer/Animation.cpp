#include "Animation.h"
#include "../utils/ColorManager.h"
#include <iostream>
#include <thread>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

Animation::Animation() : delayMs(500), isPaused(false) {
    renderer.setMode(RenderMode::DETAILED);
}

void Animation::animateBeamCalculation(const Grid& grid) {
    SecurityLaser laser(grid);
    auto steps = laser.getCalculationSteps();

    int totalBeams = 0;

    for (size_t i = 0; i < steps.size(); i++) {
        renderer.clearScreen();

        std::cout << renderer.renderHeader("Security Laser Beam Calculator - Animation");
        std::cout << renderer.renderStep(grid, steps[i]);

        totalBeams += steps[i].beamsAdded;

        std::cout << "\n" << ColorManager::info("Progress: ")
                  << ColorManager::success("Row " + std::to_string(i + 1) + "/" + std::to_string(steps.size()))
                  << "\n";
        std::cout << ColorManager::info("Total Beams So Far: ")
                  << ColorManager::success(std::to_string(totalBeams)) << "\n";

        if (i < steps.size() - 1) {
            std::cout << "\n" << ColorManager::empty("Press any key to continue...");
            std::cout.flush();
            waitForKey();
        }
    }

    std::cout << "\n\n" << ColorManager::success("Animation Complete!") << "\n";
    std::cout << ColorManager::info("Final Result: ")
              << ColorManager::success(std::to_string(totalBeams) + " laser beams") << "\n";

    sleep(1000);
}

void Animation::animateGridBuild(Grid& grid, const std::vector<std::pair<int, int>>& positions) {
    renderer.clearScreen();

    for (const auto& pos : positions) {
        grid.setCell(pos.first, pos.second, '1');

        renderer.clearScreen();
        std::cout << renderer.renderHeader("Building Grid...");
        std::cout << renderer.render(grid);

        sleep(delayMs / 2);
    }
}

void Animation::animateBeamFlash(const Grid& grid, const SecurityLaser& laser) {
    for (int flash = 0; flash < 3; flash++) {
        renderer.clearScreen();

        if (flash % 2 == 0) {
            std::cout << renderer.render(grid, laser);
        } else {
            std::cout << renderer.render(grid);
        }

        sleep(200);
    }

    // Final state with beams visible
    renderer.clearScreen();
    std::cout << renderer.renderWithStats(grid, laser);
}

void Animation::sleep(int ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Animation::waitForKey() const {
#ifdef _WIN32
    _getch();
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}
