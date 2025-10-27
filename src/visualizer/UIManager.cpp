#include "UIManager.h"
#include "../utils/ColorManager.h"
#include "../utils/FileIO.h"
#include "../utils/Benchmark.h"
#include <iostream>
#include <limits>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

UIManager::UIManager() : grid(8, 8), currentMode(UIMode::MENU),
                         cursorRow(0), cursorCol(0), running(false) {}

UIManager::UIManager(const Grid& g) : grid(g), currentMode(UIMode::MENU),
                                       cursorRow(0), cursorCol(0), running(false) {}

void UIManager::run() {
    running = true;
    ColorManager::initialize();

    renderer.clearScreen();
    renderer.hideCursor();

    while (running) {
        try {
            switch (currentMode) {
                case UIMode::MENU:
                    handleMenuMode();
                    break;
                case UIMode::EDIT:
                    handleEditMode();
                    break;
                case UIMode::ANIMATE:
                    handleAnimateMode();
                    break;
                case UIMode::BENCHMARK:
                    handleBenchmarkMode();
                    break;
                case UIMode::VIEW:
                    handleViewMode();
                    break;
            }
        } catch (const std::exception& e) {
            showError(std::string("Error: ") + e.what());
        }
    }

    renderer.showCursor();
}

void UIManager::handleMenuMode() {
    std::vector<std::string> options = {
        "Interactive Grid Editor",
        "Animate Beam Calculation",
        "View Current Grid",
        "Load Preset Example",
        "Random Grid Generator",
        "Performance Benchmark",
        "Save Grid",
        "Load Grid from File",
        "Exit"
    };

    int choice = showMenu("Security Laser Beam Visualizer", options);

    switch (choice) {
        case 0:
            currentMode = UIMode::EDIT;
            break;
        case 1:
            currentMode = UIMode::ANIMATE;
            break;
        case 2:
            currentMode = UIMode::VIEW;
            break;
        case 3: {
            auto presets = FileIO::loadPresets();
            if (!presets.empty()) {
                std::vector<std::string> presetNames;
                for (const auto& preset : presets) {
                    presetNames.push_back(preset.name + " - " + preset.description);
                }
                int presetChoice = showMenu("Select Preset", presetNames);
                if (presetChoice >= 0 && presetChoice < static_cast<int>(presets.size())) {
                    grid = Grid(presets[presetChoice].data);
                    showMessage("Preset loaded successfully!");
                }
            } else {
                showMessage("No presets available. Using default examples.", 2000);
                // Default example
                grid = Grid({"011001", "000000", "010100", "001000"});
            }
            break;
        }
        case 4: {
            std::string input = getInput("Enter grid size (rows,cols) e.g., 10,10: ");
            int rows = 10, cols = 10;
            if (sscanf(input.c_str(), "%d,%d", &rows, &cols) == 2) {
                std::string densityStr = getInput("Enter device density (0.0-1.0) e.g., 0.3: ");
                double density = 0.3;
                sscanf(densityStr.c_str(), "%lf", &density);
                generateRandomGrid(rows, cols, density);
                showMessage("Random grid generated!");
            }
            break;
        }
        case 5:
            currentMode = UIMode::BENCHMARK;
            break;
        case 6: {
            std::string filename = getInput("Enter filename to save (e.g., grid.txt): ");
            if (!filename.empty()) {
                saveGrid(filename);
            }
            break;
        }
        case 7: {
            std::string filename = getInput("Enter filename to load: ");
            if (!filename.empty()) {
                loadGridFromFile(filename);
            }
            break;
        }
        case 8:
            running = false;
            break;
    }
}

void UIManager::handleEditMode() {
    refresh();

    Key key = getKey();
    handleKey(key);

    if (key == Key::ESC || key == Key::Q) {
        currentMode = UIMode::MENU;
    }
}

void UIManager::handleAnimateMode() {
    animation.animateBeamCalculation(grid);
    currentMode = UIMode::MENU;
}

void UIManager::handleBenchmarkMode() {
    renderer.clearScreen();
    std::cout << renderer.renderHeader("Performance Benchmark");

    Benchmark benchmark;

    std::cout << ColorManager::info("Running scalability test...\n");
    benchmark.runScalabilityTest(10, 100, 20, 0.3);

    std::cout << "\n" << benchmark.generateReport();
    std::cout << "\n" << benchmark.generateASCIIGraph();

    std::cout << "\n" << ColorManager::empty("Press any key to return to menu...");
    std::cout.flush();
    animation.waitForKey();

    currentMode = UIMode::MENU;
}

void UIManager::handleViewMode() {
    renderer.clearScreen();

    SecurityLaser laser(grid);
    laser.calculateBeamsWithTracking();

    std::cout << renderer.renderHeader("Grid View - Beam Analysis");
    std::cout << renderer.renderWithStats(grid, laser);
    std::cout << renderer.renderLegend();

    std::cout << "\n" << ColorManager::empty("Press any key to return to menu...");
    std::cout.flush();
    animation.waitForKey();

    currentMode = UIMode::MENU;
}

Key UIManager::getKey() const {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        ch = _getch();
        switch (ch) {
            case 72: return Key::UP;
            case 80: return Key::DOWN;
            case 75: return Key::LEFT;
            case 77: return Key::RIGHT;
        }
    }
    switch (ch) {
        case 32: return Key::SPACE;
        case 13: return Key::ENTER;
        case 27: return Key::ESC;
        case 'q': case 'Q': return Key::Q;
        case 's': case 'S': return Key::S;
        case 'l': case 'L': return Key::L;
        case 'r': case 'R': return Key::R;
        case 'a': case 'A': return Key::A;
        case 'h': case 'H': return Key::H;
    }
#else
    char ch = getchar();
    if (ch == 27) {
        char next = getchar();
        if (next == '[') {
            char arrow = getchar();
            switch (arrow) {
                case 'A': return Key::UP;
                case 'B': return Key::DOWN;
                case 'C': return Key::RIGHT;
                case 'D': return Key::LEFT;
            }
        }
        return Key::ESC;
    }
    switch (ch) {
        case ' ': return Key::SPACE;
        case '\n': return Key::ENTER;
        case 'q': case 'Q': return Key::Q;
        case 's': case 'S': return Key::S;
        case 'l': case 'L': return Key::L;
        case 'r': case 'R': return Key::R;
        case 'a': case 'A': return Key::A;
        case 'h': case 'H': return Key::H;
    }
#endif
    return Key::UNKNOWN;
}

void UIManager::handleKey(Key key) {
    switch (key) {
        case Key::UP:
            if (cursorRow > 0) cursorRow--;
            break;
        case Key::DOWN:
            if (cursorRow < grid.getRows() - 1) cursorRow++;
            break;
        case Key::LEFT:
            if (cursorCol > 0) cursorCol--;
            break;
        case Key::RIGHT:
            if (cursorCol < grid.getCols() - 1) cursorCol++;
            break;
        case Key::SPACE:
            grid.toggleCell(cursorRow, cursorCol);
            break;
        case Key::ENTER:
            currentMode = UIMode::VIEW;
            break;
        case Key::A:
            currentMode = UIMode::ANIMATE;
            break;
        case Key::R:
            generateRandomGrid(grid.getRows(), grid.getCols(), 0.3);
            break;
        case Key::H:
            renderer.clearScreen();
            std::cout << renderer.renderHeader("Help");
            std::cout << renderer.renderControls();
            animation.waitForKey();
            break;
        default:
            break;
    }
}

int UIManager::showMenu(const std::string& title, const std::vector<std::string>& options) {
    int selected = 0;

    while (true) {
        renderer.clearScreen();
        std::cout << renderer.renderHeader(title);

        for (size_t i = 0; i < options.size(); i++) {
            if (static_cast<int>(i) == selected) {
                std::cout << "  " << ColorManager::highlight("> " + options[i]) << "\n";
            } else {
                std::cout << "    " << options[i] << "\n";
            }
        }

        std::cout << "\n" << ColorManager::empty("Use UP/DOWN arrows to navigate, ENTER to select\n");

        Key key = getKey();
        if (key == Key::UP && selected > 0) {
            selected--;
        } else if (key == Key::DOWN && selected < static_cast<int>(options.size()) - 1) {
            selected++;
        } else if (key == Key::ENTER) {
            return selected;
        } else if (key == Key::ESC || key == Key::Q) {
            return -1;
        }
    }
}

bool UIManager::confirm(const std::string& message) {
    std::cout << "\n" << ColorManager::info(message) << " (y/n): ";
    Key key = getKey();
    return (key == Key::ENTER);  // Simplified for now
}

std::string UIManager::getInput(const std::string& prompt) {
    renderer.showCursor();
    std::cout << "\n" << ColorManager::info(prompt);
    std::string input;
    std::getline(std::cin, input);
    renderer.hideCursor();
    return input;
}

void UIManager::refresh() {
    renderer.clearScreen();
    renderer.setHighlight(cursorRow, cursorCol);

    std::cout << renderer.renderHeader("Interactive Grid Editor");
    std::cout << renderer.render(grid);
    std::cout << renderer.renderControls();

    renderer.clearHighlight();
}

void UIManager::showMessage(const std::string& msg, int durationMs) {
    std::cout << "\n" << ColorManager::success(msg) << "\n";
    animation.sleep(durationMs);
}

void UIManager::showError(const std::string& error) {
    std::cout << "\n" << ColorManager::error(error) << "\n";
    animation.sleep(3000);
}

void UIManager::loadGrid(const Grid& g) {
    grid = g;
    cursorRow = 0;
    cursorCol = 0;
}

void UIManager::saveGrid(const std::string& filename) {
    try {
        if (FileIO::saveGrid(grid, filename)) {
            showMessage("Grid saved successfully to " + filename);
        } else {
            showError("Failed to save grid");
        }
    } catch (const std::exception& e) {
        showError(std::string("Error saving: ") + e.what());
    }
}

void UIManager::loadGridFromFile(const std::string& filename) {
    try {
        Grid loaded = FileIO::loadGrid(filename);
        if (loaded.isValid()) {
            loadGrid(loaded);
            showMessage("Grid loaded successfully from " + filename);
        } else {
            showError("Invalid grid in file");
        }
    } catch (const std::exception& e) {
        showError(std::string("Error loading: ") + e.what());
    }
}

void UIManager::generateRandomGrid(int rows, int cols, double density) {
    grid = Grid(rows, cols);
    grid.randomize(density);
    cursorRow = 0;
    cursorCol = 0;
}
