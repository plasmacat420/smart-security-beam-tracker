#include "Renderer.h"
#include "../utils/ColorManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

Renderer::Renderer() : mode(RenderMode::DETAILED), showCoordinates(true),
                       showLegend(true), highlightRow(-1), highlightCol(-1) {}

void Renderer::setHighlight(int row, int col) {
    highlightRow = row;
    highlightCol = col;
}

void Renderer::clearHighlight() {
    highlightRow = -1;
    highlightCol = -1;
}

std::string Renderer::render(const Grid& grid) const {
    std::ostringstream oss;
    int rows = grid.getRows();
    int cols = grid.getCols();

    // Column headers
    if (showCoordinates) {
        oss << "    ";
        for (int j = 0; j < cols; j++) {
            oss << j % 10 << " ";
        }
        oss << "\n";
        oss << "   " << std::string(cols * 2 + 1, '-') << "\n";
    }

    // Grid rows
    for (int i = 0; i < rows; i++) {
        if (showCoordinates) {
            oss << std::setw(2) << i << " | ";
        }

        for (int j = 0; j < cols; j++) {
            char cell = grid.getCell(i, j);
            bool isHighlighted = (i == highlightRow && j == highlightCol);

            std::string cellStr;
            if (cell == '1') {
                cellStr = "█";
            } else {
                cellStr = "·";
            }

            if (isHighlighted) {
                oss << ColorManager::highlight(cellStr);
            } else if (cell == '1') {
                oss << ColorManager::device(cellStr);
            } else {
                oss << ColorManager::empty(cellStr);
            }
            oss << " ";
        }

        if (showCoordinates) {
            oss << "| " << i;
        }
        oss << "\n";
    }

    if (showCoordinates) {
        oss << "   " << std::string(cols * 2 + 1, '-') << "\n";
    }

    return oss.str();
}

std::string Renderer::render(const Grid& grid, const SecurityLaser& laser) const {
    std::ostringstream oss;
    int rows = grid.getRows();
    int cols = grid.getCols();

    // Create a 2D array to track beam paths
    std::vector<std::vector<bool>> hasBeam(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<int>> beamCount(rows, std::vector<int>(cols, 0));

    const auto& beams = laser.getBeams();
    for (const auto& beam : beams) {
        // Mark all cells between the two devices as having a beam
        for (int r = beam.row1 + 1; r < beam.row2; r++) {
            // Beams connect vertically through intermediate rows
            hasBeam[r][beam.col1] = true;
            hasBeam[r][beam.col2] = true;
            beamCount[r][beam.col1]++;
            beamCount[r][beam.col2]++;
        }
    }

    // Column headers
    if (showCoordinates) {
        oss << "    ";
        for (int j = 0; j < cols; j++) {
            oss << j % 10 << " ";
        }
        oss << "\n";
        oss << "   " << std::string(cols * 2 + 1, '-') << "\n";
    }

    // Grid rows with beams
    for (int i = 0; i < rows; i++) {
        if (showCoordinates) {
            oss << std::setw(2) << i << " | ";
        }

        for (int j = 0; j < cols; j++) {
            char cell = grid.getCell(i, j);
            bool isHighlighted = (i == highlightRow && j == highlightCol);

            std::string cellStr;
            if (cell == '1') {
                cellStr = "█";
            } else if (hasBeam[i][j]) {
                if (beamCount[i][j] > 1) {
                    cellStr = "║";  // Multiple beams
                } else {
                    cellStr = "|";  // Single beam
                }
            } else {
                cellStr = "·";
            }

            if (isHighlighted) {
                oss << ColorManager::highlight(cellStr);
            } else if (cell == '1') {
                oss << ColorManager::device(cellStr);
            } else if (hasBeam[i][j]) {
                oss << ColorManager::beam(cellStr);
            } else {
                oss << ColorManager::empty(cellStr);
            }
            oss << " ";
        }

        if (showCoordinates) {
            oss << "| " << i;
        }
        oss << "\n";
    }

    if (showCoordinates) {
        oss << "   " << std::string(cols * 2 + 1, '-') << "\n";
    }

    return oss.str();
}

std::string Renderer::renderWithStats(const Grid& grid, const SecurityLaser& laser) const {
    std::ostringstream oss;

    oss << render(grid, laser);
    oss << "\n" << renderStats(laser);

    return oss.str();
}

std::string Renderer::renderStep(const Grid& grid, const SecurityLaser::CalculationStep& step) const {
    std::ostringstream oss;

    // Highlight the current row being processed
    int oldRow = highlightRow;
    const_cast<Renderer*>(this)->highlightRow = step.currentRow;

    oss << render(grid);

    const_cast<Renderer*>(this)->highlightRow = oldRow;

    // Show step info
    oss << "\n" << ColorManager::info("Step Information:") << "\n";
    oss << "  Current Row: " << step.currentRow << "\n";
    oss << "  Devices in Row: " << step.devicesInRow << "\n";
    oss << "  Previous Devices: " << step.previousDevices << "\n";
    oss << "  Beams Added: " << ColorManager::success(std::to_string(step.beamsAdded)) << "\n";

    return oss.str();
}

std::string Renderer::renderHeader(const std::string& title) const {
    std::ostringstream oss;
    int width = 60;

    oss << "\n";
    oss << ColorManager::info(std::string(width, '=')) << "\n";
    oss << ColorManager::info("  " + title) << "\n";
    oss << ColorManager::info(std::string(width, '=')) << "\n\n";

    return oss.str();
}

std::string Renderer::renderLegend() const {
    std::ostringstream oss;

    oss << "\n" << ColorManager::info("Legend:") << "\n";
    oss << "  " << ColorManager::device("█") << " Security Device\n";
    oss << "  " << ColorManager::beam("|") << " Laser Beam\n";
    oss << "  " << ColorManager::empty("·") << " Empty Cell\n";
    oss << "  " << ColorManager::highlight("█") << " Selected Cell\n";

    return oss.str();
}

std::string Renderer::renderStats(const SecurityLaser& laser) const {
    std::ostringstream oss;

    oss << ColorManager::info("Statistics:") << "\n";
    oss << "  Total Devices: " << laser.getTotalDevices() << "\n";
    oss << "  Total Beams: " << ColorManager::success(std::to_string(laser.getTotalBeams())) << "\n";
    oss << "  Avg Beams/Device: " << std::fixed << std::setprecision(2)
        << laser.getAverageBeamsPerDevice() << "\n";

    return oss.str();
}

std::string Renderer::renderControls() const {
    std::ostringstream oss;

    oss << "\n" << ColorManager::info("Controls:") << "\n";
    oss << "  " << ColorManager::highlight("Arrow Keys") << " - Navigate grid\n";
    oss << "  " << ColorManager::highlight("SPACE") << "      - Toggle device\n";
    oss << "  " << ColorManager::highlight("ENTER") << "      - Calculate beams\n";
    oss << "  " << ColorManager::highlight("A") << "          - Animate\n";
    oss << "  " << ColorManager::highlight("R") << "          - Random grid\n";
    oss << "  " << ColorManager::highlight("S") << "          - Save\n";
    oss << "  " << ColorManager::highlight("L") << "          - Load\n";
    oss << "  " << ColorManager::highlight("H") << "          - Help\n";
    oss << "  " << ColorManager::highlight("Q/ESC") << "      - Quit\n";

    return oss.str();
}

void Renderer::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

void Renderer::moveCursor(int row, int col) const {
    std::cout << "\033[" << row << ";" << col << "H";
}

void Renderer::hideCursor() const {
    std::cout << "\033[?25l";
}

void Renderer::showCursor() const {
    std::cout << "\033[?25h";
}
