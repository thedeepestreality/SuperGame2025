#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <GameState.h>
#include <Config.h>

int InitGame(GameState& init_state, Config& config, const std::string& config_path) {
    std::ifstream configFile(config_path);
    if (!configFile) {
        std::cerr << "Cannot open config file" << std::endl;
        return 1;
    }

    std::string line;
    bool readColors = false;
    bool readWorld = false;
    bool readWindow = false;
    std::vector<std::vector<char>> temp_world;

    while (std::getline(configFile, line)) {
        if (line.empty()) continue;

        while (!line.empty() && line[0] == ' ') line.erase(0, 1);
        while (!line.empty() && line.back() == ' ') line.pop_back();

        if (line == "color:") {
            readColors = true;
            readWorld = false;
            readWindow = false;
            continue;
        }
        else if (line == "world:") {
            readColors = false;
            readWorld = true;
            readWindow = false;
            continue;
        }
        else if (line == "window:") {
            readColors = false;
            readWorld = false;
            readWindow = true;
            continue;
        }
        else if (line.find("cell_side:") == 0) {
            std::string value = line.substr(line.find(":") + 1);
            while (!value.empty() && value[0] == ' ') value.erase(0, 1);
            try {
                config.cell_side = std::stof(value);
            }
            catch (...) {
                std::cerr << "Invalid cell_side value" << std::endl;
                return 1;
            }
            continue;
        }


        if (readColors) {
            size_t colon_pos = line.find(":");
            if (colon_pos == std::string::npos) continue;

            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            while (!value.empty() && value[0] == ' ') value.erase(0, 1);
            while (!value.empty() && value.back() == ' ') value.pop_back();

            std::vector<int> rgb;
            size_t start = 0;
            while (start < value.length()) {
                size_t next = value.find(' ', start);
                if (next == std::string::npos) next = value.length();
                try {
                    rgb.push_back(std::stoi(value.substr(start, next - start)));
                }
                catch (...) {
                    std::cerr << "Invalid color value for " << key << std::endl;
                    return 1;
                }
                start = next + 1;
            }
            if (rgb.size() != 3) {
                std::cerr << "Invalid color format for " << key << std::endl;
                return 1;
            }

            if (key == "player") {
                config.player_color = sf::Color(rgb[0], rgb[1], rgb[2]);
                config.player_char = 'P';
            }
            else if (key == "wall") {
                config.wall_color = sf::Color(rgb[0], rgb[1], rgb[2]);
                config.wall_char = 'H';
            }
            else if (key == "empty") {
                config.empty_color = sf::Color(rgb[0], rgb[1], rgb[2]);
                config.empty_char = '.';
            }
            else if (key == "enemy") {
                config.enemy_color = sf::Color(rgb[0], rgb[1], rgb[2]);
                config.enemy_char = 'X';
            }
        }
        else if (readWindow) {
            size_t colon_pos = line.find(":");
            if (colon_pos == std::string::npos) continue;

            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            while (!value.empty() && value[0] == ' ') value.erase(0, 1);
            while (!value.empty() && value.back() == ' ') value.pop_back();

            try {
                if (key == "width") {
                    config.window_width = std::stoi(value);
                }
                else if (key == "height") {
                    config.window_height = std::stoi(value);
                }
            }
            catch (...) {
                std::cerr << "invalid window arg: " << key << std::endl;
                return 1;
            }
        }
        else if (readWorld) {
            std::vector<char> row(line.begin(), line.end());
            temp_world.push_back(row);
        }
    }

    if (!temp_world.empty()) {
        size_t rows = temp_world.size();
        size_t cols = temp_world[0].size();
        init_state.m_field.resize(rows, std::vector<CellType>(cols, CellType::Empty));

        for (size_t row = 0; row < rows; ++row) {
            for (size_t col = 0; col < cols; ++col) {
                char c = temp_world[row][col];
                if (c == config.player_char) {
                    init_state.m_player_pos = { static_cast<int>(row), static_cast<int>(col) };
                    init_state.m_field[row][col] = CellType::Empty;
                }
                else if (c == config.enemy_char) {
                    init_state.m_enemy_pos.push_back({ static_cast<int>(row), static_cast<int>(col) });
                    init_state.m_field[row][col] = CellType::Empty;
                }
                else if (c == config.wall_char) {
                    init_state.m_field[row][col] = CellType::Wall;
                }
                else {
                    init_state.m_field[row][col] = CellType::Empty;
                }
            }
        }
    }
    configFile.close();
    return 0;
}