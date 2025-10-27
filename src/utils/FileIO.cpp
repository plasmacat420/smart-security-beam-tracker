#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool FileIO::saveGrid(const Grid& grid, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    const auto& data = grid.getData();
    for (const auto& row : data) {
        file << row << "\n";
    }

    file.close();
    return true;
}

Grid FileIO::loadGrid(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::vector<std::string> data;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            data.push_back(line);
        }
    }

    file.close();
    return Grid(data);
}

bool FileIO::saveGridJSON(const Grid& grid, const std::string& filename,
                         const std::string& name, const std::string& description) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    const auto& data = grid.getData();

    file << "{\n";
    file << "  \"name\": \"" << name << "\",\n";
    file << "  \"description\": \"" << description << "\",\n";
    file << "  \"rows\": " << grid.getRows() << ",\n";
    file << "  \"cols\": " << grid.getCols() << ",\n";
    file << "  \"data\": [\n";

    for (size_t i = 0; i < data.size(); i++) {
        file << "    \"" << data[i] << "\"";
        if (i < data.size() - 1) file << ",";
        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    file.close();
    return true;
}

Grid FileIO::loadGridJSON(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    // Simple JSON parsing (for basic format)
    std::vector<std::string> data;
    std::string line;
    bool inDataArray = false;

    while (std::getline(file, line)) {
        if (line.find("\"data\":") != std::string::npos) {
            inDataArray = true;
            continue;
        }

        if (inDataArray) {
            // Extract string between quotes
            size_t first = line.find('"');
            size_t last = line.rfind('"');

            if (first != std::string::npos && last != std::string::npos && first != last) {
                std::string gridRow = line.substr(first + 1, last - first - 1);
                if (!gridRow.empty()) {
                    data.push_back(gridRow);
                }
            }

            if (line.find(']') != std::string::npos) {
                break;
            }
        }
    }

    file.close();
    return Grid(data);
}

std::vector<GridConfig> FileIO::loadPresets(const std::string& filename) {
    std::vector<GridConfig> configs;

    std::ifstream file(filename);
    if (!file.is_open()) {
        // Return default examples if file doesn't exist
        GridConfig ex1;
        ex1.name = "Example 1";
        ex1.description = "Basic beam configuration";
        ex1.data = {"011001", "000000", "010100", "001000"};
        ex1.expectedBeams = 8;
        configs.push_back(ex1);

        GridConfig ex2;
        ex2.name = "Example 2";
        ex2.description = "No beams";
        ex2.data = {"000", "111", "000"};
        ex2.expectedBeams = 0;
        configs.push_back(ex2);

        GridConfig ex3;
        ex3.name = "Maximum Beams";
        ex3.description = "Dense device placement";
        ex3.data = {"111", "000", "111"};
        ex3.expectedBeams = 9;
        configs.push_back(ex3);

        GridConfig ex4;
        ex4.name = "Sparse";
        ex4.description = "Minimal devices";
        ex4.data = {"10000", "00000", "00001", "00000", "01000"};
        ex4.expectedBeams = 2;
        configs.push_back(ex4);

        return configs;
    }

    // Simple JSON array parsing
    std::string line;
    GridConfig current;
    bool inConfig = false;
    bool inDataArray = false;

    while (std::getline(file, line)) {
        if (line.find('{') != std::string::npos && line.find("\"name\"") == std::string::npos) {
            inConfig = true;
            current = GridConfig();
            continue;
        }

        if (inConfig) {
            if (line.find("\"name\":") != std::string::npos) {
                size_t start = line.find('"', line.find(':')) + 1;
                size_t end = line.rfind('"');
                current.name = line.substr(start, end - start);
            } else if (line.find("\"description\":") != std::string::npos) {
                size_t start = line.find('"', line.find(':')) + 1;
                size_t end = line.rfind('"');
                current.description = line.substr(start, end - start);
            } else if (line.find("\"expectedBeams\":") != std::string::npos) {
                size_t start = line.find(':') + 1;
                std::string numStr = line.substr(start);
                current.expectedBeams = std::stoi(numStr);
            } else if (line.find("\"data\":") != std::string::npos) {
                inDataArray = true;
            } else if (inDataArray) {
                if (line.find(']') != std::string::npos) {
                    inDataArray = false;
                } else {
                    size_t first = line.find('"');
                    size_t last = line.rfind('"');
                    if (first != std::string::npos && last != std::string::npos && first != last) {
                        current.data.push_back(line.substr(first + 1, last - first - 1));
                    }
                }
            }

            if (line.find('}') != std::string::npos && !inDataArray) {
                configs.push_back(current);
                inConfig = false;
            }
        }
    }

    file.close();
    return configs;
}

bool FileIO::savePresets(const std::vector<GridConfig>& configs, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "[\n";

    for (size_t i = 0; i < configs.size(); i++) {
        const auto& config = configs[i];

        file << "  {\n";
        file << "    \"name\": \"" << config.name << "\",\n";
        file << "    \"description\": \"" << config.description << "\",\n";
        file << "    \"expectedBeams\": " << config.expectedBeams << ",\n";
        file << "    \"data\": [\n";

        for (size_t j = 0; j < config.data.size(); j++) {
            file << "      \"" << config.data[j] << "\"";
            if (j < config.data.size() - 1) file << ",";
            file << "\n";
        }

        file << "    ]\n";
        file << "  }";
        if (i < configs.size() - 1) file << ",";
        file << "\n";
    }

    file << "]\n";

    file.close();
    return true;
}

bool FileIO::exportVisualization(const std::string& visualization, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << visualization;

    file.close();
    return true;
}
