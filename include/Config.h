#pragma once
#include <GameState.h>
#include <SFML/Graphics.hpp>

struct Config {
    char player_char = 'P';
    char wall_char = 'H';
    char empty_char = '.';
    char enemy_char = 'X';

    sf::Color player_color = sf::Color(100, 100, 250); 
    sf::Color wall_color = sf::Color(100, 100, 100);   
    sf::Color empty_color = sf::Color(100, 250, 50);   
    sf::Color enemy_color = sf::Color(255, 100, 100);  

    float cell_side = 100.0f;
};

void InitGame(GameState& init_state, Config& config);