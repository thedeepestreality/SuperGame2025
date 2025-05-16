#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <GameState.h>
#include <Config.h>

void InitGame(GameState& init_state, Config& config) {
    std::ifstream configFile("../config.txt");
    if (!configFile) {
        std::cerr << "No config file" << std::endl;
        return;
    }

    std::string line;
    bool readColors = false;
    bool readWorld = false;
    std::vector<std::vector<char>> temp_world;

    while (std::getline(configFile, line)) {
        if (line.empty()) continue;

        while (!line.empty() && line[0] == ' ') line.erase(0, 1);
        while (!line.empty() && line.back() == ' ') line.pop_back();

        if (line == "color:") {
            readColors = true;
            readWorld = false;
            continue;
        }
        else if (line == "world:") {
            readColors = false;
            readWorld = true;
            continue;
        }

        if (readColors) {
            size_t colon_pos = line.find(":");
            if (colon_pos == std::string::npos) continue;

            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            
            while (!value.empty() && value[0] == ' ') value.erase(0, 1);
            while (!value.empty() && value.back() == ' ') value.pop_back();

            if (key == "player") {
                if (value == "blue") config.player_color = sf::Color(100, 100, 250);
                else if (value == "red") config.player_color = sf::Color(255, 100, 100);
                else if (value == "green") config.player_color = sf::Color(100, 250, 50);
                config.player_char = 'P';
            }
            else if (key == "wall") {
                if (value == "gray") config.wall_color = sf::Color(100, 100, 100);
                else if (value == "blue") config.wall_color = sf::Color(100, 100, 250);
                config.wall_char = 'H';
            }
            else if (key == "empty") {
                if (value == "green") config.empty_color = sf::Color(100, 250, 50);
                else if (value == "gray") config.empty_color = sf::Color(100, 100, 100);
                config.empty_char = '.';
            }

            else if (key == "enemy") {
                if (value == "red") config.enemy_color = sf::Color(255, 100, 100);
                else if (value == "blue") config.enemy_color = sf::Color(100, 100, 250);
                config.enemy_char = 'X';
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
                } else if (c == config.enemy_char) {
                    init_state.m_enemy_pos.push_back({ static_cast<int>(row), static_cast<int>(col) });
                    init_state.m_field[row][col] = CellType::Empty;
                } else if (c == config.wall_char) {
                    init_state.m_field[row][col] = CellType::Wall;
                } else {
                    init_state.m_field[row][col] = CellType::Empty;
                }
            }
        }
    }




    configFile.close();
}