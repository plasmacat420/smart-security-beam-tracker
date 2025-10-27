#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>
#include <stdexcept>

class Grid {
private:
    std::vector<std::string> data;
    int rows;
    int cols;

public:
    Grid();
    Grid(const std::vector<std::string>& bank);
    Grid(int rows, int cols);

    // Getters
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    const std::vector<std::string>& getData() const { return data; }
    char getCell(int row, int col) const;
    int countDevicesInRow(int row) const;

    // Setters
    void setCell(int row, int col, char value);
    void toggleCell(int row, int col);

    // Utility
    bool isValid() const;
    void clear();
    void randomize(double deviceDensity = 0.3);
    std::string toString() const;
};

#endif
