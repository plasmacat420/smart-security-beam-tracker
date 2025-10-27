#ifndef COLOR_MANAGER_H
#define COLOR_MANAGER_H

#include <string>
#include <map>

enum class Color {
    RESET,
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE,
    BG_BLACK,
    BG_RED,
    BG_GREEN,
    BG_YELLOW,
    BG_BLUE,
    BG_MAGENTA,
    BG_CYAN,
    BG_WHITE
};

class ColorManager {
private:
    static bool colorsEnabled;
    static std::map<Color, std::string> ansiCodes;
    static void initializeColorCodes();

public:
    static void initialize();
    static void enableColors(bool enable);
    static bool areColorsEnabled();

    static std::string colorize(const std::string& text, Color color);
    static std::string colorize(const std::string& text, Color fg, Color bg);
    static std::string reset();

    // Convenience functions
    static std::string device(const std::string& text);
    static std::string beam(const std::string& text);
    static std::string empty(const std::string& text);
    static std::string highlight(const std::string& text);
    static std::string info(const std::string& text);
    static std::string success(const std::string& text);
    static std::string error(const std::string& text);
};

#endif
