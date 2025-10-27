#ifndef FILE_IO_H
#define FILE_IO_H

#include "../core/Grid.h"
#include <string>
#include <vector>

struct GridConfig {
    std::string name;
    std::string description;
    std::vector<std::string> data;
    int expectedBeams;
};

class FileIO {
public:
    // Grid save/load
    static bool saveGrid(const Grid& grid, const std::string& filename);
    static Grid loadGrid(const std::string& filename);

    // JSON format (for web compatibility)
    static bool saveGridJSON(const Grid& grid, const std::string& filename,
                            const std::string& name = "", const std::string& description = "");
    static Grid loadGridJSON(const std::string& filename);

    // Presets
    static std::vector<GridConfig> loadPresets(const std::string& filename = "presets/examples.json");
    static bool savePresets(const std::vector<GridConfig>& configs, const std::string& filename);

    // Export visualization
    static bool exportVisualization(const std::string& visualization, const std::string& filename);
};

#endif
