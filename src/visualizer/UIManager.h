#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "../core/Grid.h"
#include "../core/SecurityLaser.h"
#include "Renderer.h"
#include "Animation.h"
#include <string>

enum class UIMode {
    MENU,
    EDIT,
    ANIMATE,
    BENCHMARK,
    VIEW
};

enum class Key {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE,
    ENTER,
    ESC,
    Q,
    S,
    L,
    R,
    A,
    H,
    UNKNOWN
};

class UIManager {
private:
    Grid grid;
    Renderer renderer;
    Animation animation;
    UIMode currentMode;
    int cursorRow;
    int cursorCol;
    bool running;

public:
    UIManager();
    UIManager(const Grid& g);

    void run();
    void stop() { running = false; }

    // Mode handlers
    void handleMenuMode();
    void handleEditMode();
    void handleAnimateMode();
    void handleBenchmarkMode();
    void handleViewMode();

    // Input handling
    Key getKey() const;
    void handleKey(Key key);

    // Menu system
    int showMenu(const std::string& title, const std::vector<std::string>& options);
    bool confirm(const std::string& message);
    std::string getInput(const std::string& prompt);

    // Display
    void refresh();
    void showMessage(const std::string& msg, int durationMs = 2000);
    void showError(const std::string& error);

    // Grid operations
    void loadGrid(const Grid& g);
    void saveGrid(const std::string& filename);
    void loadGridFromFile(const std::string& filename);
    void generateRandomGrid(int rows, int cols, double density);
};

#endif
