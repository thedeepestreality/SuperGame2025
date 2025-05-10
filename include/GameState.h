#pragma once
#include <vector>

enum class CellType {
	Empty,
	Wall
};

struct CellPosition {
	int row;
	int col;
};

struct GameState {
	std::vector<std::vector<CellType>> m_field;
	CellPosition m_player_pos;
	std::vector<CellPosition> m_enemy_pos;
	unsigned long long epoch = 0;
};