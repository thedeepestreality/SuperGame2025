#pragma once

#include <GameState.h>
 
enum class Direction {
	None,
	Up,
	Down,
	Left,
	Right
};

enum class GameStatus {
	StartMenu,
	InGame
};

class GameManager {
private:
	GameState m_state;
	const int m_skip_enemy_steps = 4;
	
	CellPosition move(CellPosition pos, Direction dir);

	bool check_enemy_meet() const;

public:
	GameManager(const GameState& state);

	bool update_world(Direction player_dir);

	GameState get_state() const;
};
