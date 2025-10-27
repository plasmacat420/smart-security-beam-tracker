#ifndef ANIMATION_H
#define ANIMATION_H

#include "../core/Grid.h"
#include "../core/SecurityLaser.h"
#include "Renderer.h"
#include <chrono>
#include <thread>

class Animation {
private:
    Renderer renderer;
    int delayMs;
    bool isPaused;

public:
    Animation();

    void setDelay(int ms) { delayMs = ms; }
    int getDelay() const { return delayMs; }

    // Animation sequences
    void animateBeamCalculation(const Grid& grid);
    void animateGridBuild(Grid& grid, const std::vector<std::pair<int, int>>& positions);
    void animateBeamFlash(const Grid& grid, const SecurityLaser& laser);

    // Control
    void pause() { isPaused = true; }
    void resume() { isPaused = false; }
    bool paused() const { return isPaused; }

    // Utility
    void sleep(int ms) const;
    void waitForKey() const;
};

#endif
