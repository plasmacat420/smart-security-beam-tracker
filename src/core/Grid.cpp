#include "Grid.h"
#include <algorithm>
#include <random>
#include <sstream>

Grid::Grid() : rows(0), cols(0) {}

Grid::Grid(const std::vector<std::string>& bank) : data(bank) {
    if (bank.empty()) {
        rows = 0;
        cols = 0;
    } else {
        rows = bank.size();
        cols = bank[0].length();
    }
}

Grid::Grid(int r, int c) : rows(r), cols(c) {
    data.resize(rows, std::string(cols, '0'));
}

char Grid::getCell(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("Cell coordinates out of bounds");
    }
    return data[row][col];
}

int Grid::countDevicesInRow(int row) const {
    if (row < 0 || row >= rows) return 0;

    int count = 0;
    for (char c : data[row]) {
        if (c == '1') count++;
    }
    return count;
}

void Grid::setCell(int row, int col, char value) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw std::out_of_range("Cell coordinates out of bounds");
    }
    if (value != '0' && value != '1') {
        throw std::invalid_argument("Cell value must be '0' or '1'");
    }
    data[row][col] = value;
}

void Grid::toggleCell(int row, int col) {
    char current = getCell(row, col);
    setCell(row, col, current == '0' ? '1' : '0');
}

bool Grid::isValid() const {
    if (rows == 0 || cols == 0) return false;
    if (data.size() != static_cast<size_t>(rows)) return false;

    for (const auto& row : data) {
        if (row.length() != static_cast<size_t>(cols)) return false;
        for (char c : row) {
            if (c != '0' && c != '1') return false;
        }
    }
    return true;
}

void Grid::clear() {
    for (auto& row : data) {
        std::fill(row.begin(), row.end(), '0');
    }
}

void Grid::randomize(double deviceDensity) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data[i][j] = (dis(gen) < deviceDensity) ? '1' : '0';
        }
    }
}

std::string Grid::toString() const {
    std::ostringstream oss;
    for (const auto& row : data) {
        oss << row << "\n";
    }
    return oss.str();
}
