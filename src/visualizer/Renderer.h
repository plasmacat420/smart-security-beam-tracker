#ifndef RENDERER_H
#define RENDERER_H

#include "../core/Grid.h"
#include "../core/SecurityLaser.h"
#include <string>
#include <vector>

enum class RenderMode {
    COMPACT,      // Simple grid
    DETAILED,     // Grid with beam lines
    ANIMATED,     // Step-by-step animation
    STATISTICS    // Grid with stats overlay
};

class Renderer {
private:
    RenderMode mode;
    bool showCoordinates;
    bool showLegend;
    int highlightRow;
    int highlightCol;

public:
    Renderer();

    void setMode(RenderMode m) { mode = m; }
    void setShowCoordinates(bool show) { showCoordinates = show; }
    void setShowLegend(bool show) { showLegend = show; }
    void setHighlight(int row, int col);
    void clearHighlight();

    // Rendering functions
    std::string render(const Grid& grid) const;
    std::string render(const Grid& grid, const SecurityLaser& laser) const;
    std::string renderWithStats(const Grid& grid, const SecurityLaser& laser) const;
    std::string renderStep(const Grid& grid, const SecurityLaser::CalculationStep& step) const;

    // UI elements
    std::string renderHeader(const std::string& title) const;
    std::string renderLegend() const;
    std::string renderStats(const SecurityLaser& laser) const;
    std::string renderControls() const;

    // Screen management
    void clearScreen() const;
    void moveCursor(int row, int col) const;
    void hideCursor() const;
    void showCursor() const;
};

#endif
