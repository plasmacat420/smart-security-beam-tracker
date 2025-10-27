#include "ColorManager.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

bool ColorManager::colorsEnabled = true;
std::map<Color, std::string> ColorManager::ansiCodes;

void ColorManager::initializeColorCodes() {
    ansiCodes[Color::RESET] = "\033[0m";
    ansiCodes[Color::BLACK] = "\033[30m";
    ansiCodes[Color::RED] = "\033[31m";
    ansiCodes[Color::GREEN] = "\033[32m";
    ansiCodes[Color::YELLOW] = "\033[33m";
    ansiCodes[Color::BLUE] = "\033[34m";
    ansiCodes[Color::MAGENTA] = "\033[35m";
    ansiCodes[Color::CYAN] = "\033[36m";
    ansiCodes[Color::WHITE] = "\033[37m";
    ansiCodes[Color::BRIGHT_BLACK] = "\033[90m";
    ansiCodes[Color::BRIGHT_RED] = "\033[91m";
    ansiCodes[Color::BRIGHT_GREEN] = "\033[92m";
    ansiCodes[Color::BRIGHT_YELLOW] = "\033[93m";
    ansiCodes[Color::BRIGHT_BLUE] = "\033[94m";
    ansiCodes[Color::BRIGHT_MAGENTA] = "\033[95m";
    ansiCodes[Color::BRIGHT_CYAN] = "\033[96m";
    ansiCodes[Color::BRIGHT_WHITE] = "\033[97m";
    ansiCodes[Color::BG_BLACK] = "\033[40m";
    ansiCodes[Color::BG_RED] = "\033[41m";
    ansiCodes[Color::BG_GREEN] = "\033[42m";
    ansiCodes[Color::BG_YELLOW] = "\033[43m";
    ansiCodes[Color::BG_BLUE] = "\033[44m";
    ansiCodes[Color::BG_MAGENTA] = "\033[45m";
    ansiCodes[Color::BG_CYAN] = "\033[46m";
    ansiCodes[Color::BG_WHITE] = "\033[47m";
}

void ColorManager::initialize() {
    initializeColorCodes();

#ifdef _WIN32
    // Enable ANSI escape codes on Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

void ColorManager::enableColors(bool enable) {
    colorsEnabled = enable;
}

bool ColorManager::areColorsEnabled() {
    return colorsEnabled;
}

std::string ColorManager::colorize(const std::string& text, Color color) {
    if (!colorsEnabled) return text;
    return ansiCodes[color] + text + ansiCodes[Color::RESET];
}

std::string ColorManager::colorize(const std::string& text, Color fg, Color bg) {
    if (!colorsEnabled) return text;
    return ansiCodes[bg] + ansiCodes[fg] + text + ansiCodes[Color::RESET];
}

std::string ColorManager::reset() {
    return ansiCodes[Color::RESET];
}

std::string ColorManager::device(const std::string& text) {
    return colorize(text, Color::BRIGHT_RED, Color::BG_RED);
}

std::string ColorManager::beam(const std::string& text) {
    return colorize(text, Color::BRIGHT_YELLOW);
}

std::string ColorManager::empty(const std::string& text) {
    return colorize(text, Color::BRIGHT_BLACK);
}

std::string ColorManager::highlight(const std::string& text) {
    return colorize(text, Color::BLACK, Color::BG_WHITE);
}

std::string ColorManager::info(const std::string& text) {
    return colorize(text, Color::BRIGHT_CYAN);
}

std::string ColorManager::success(const std::string& text) {
    return colorize(text, Color::BRIGHT_GREEN);
}

std::string ColorManager::error(const std::string& text) {
    return colorize(text, Color::BRIGHT_RED);
}
