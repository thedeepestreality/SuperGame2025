#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

#include <GameState.h>
#include <GameManager.h>

void render_world(const GameState& state) {
	system("cls");
	std::cout << "Epoch " << state.epoch << '\n';
	for (int row = 0; row < state.m_field.size(); ++row) {
		for (int col = 0; col < state.m_field[0].size(); ++col) {
			if (state.m_player_pos.row == row && state.m_player_pos.col == col)
				std::cout << 'P';
			else
				switch (state.m_field[row][col]) {
					case CellType::Empty:
						std::cout << '.';
						break;
					case CellType::Wall:
						std::cout << 'H';
						break;
				}
		}
		std::cout << '\n';
	}
	std::cout << '\n';
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

struct RenderParams {
	float cell_side = 100.0f;
	sf::Color empty_color = sf::Color(100, 250, 50);
	sf::Color wall_color = sf::Color(100, 100, 100);
	sf::Color player_color = sf::Color(100, 100, 250);
	sf::Color enemy_color = sf::Color(255, 100, 100);
};

void render_world(const GameState& state, sf::RenderWindow& window, const RenderParams& params = RenderParams()) {
	const float cell_side = params.cell_side;
	sf::RectangleShape cell({ cell_side, cell_side });
	// Fill field cells
	for (int row = 0; row < state.m_field.size(); ++row) {
		for (int col = 0; col < state.m_field[0].size(); ++col) {
			cell.setPosition(col*cell_side, row*cell_side);
			switch (state.m_field[row][col]) {
				case CellType::Empty:
					cell.setFillColor(params.empty_color);
					break;
				case CellType::Wall:
					cell.setFillColor(params.wall_color);
					break;
			}
			window.draw(cell);
		}
	}

	// Fill player cell
	cell.setPosition(state.m_player_pos.col * cell_side, state.m_player_pos.row * cell_side);
	cell.setFillColor(params.player_color);
	window.draw(cell);

	// Fill enemy cells
	for (CellPosition enemy_pos : state.m_enemy_pos) {
		cell.setPosition(enemy_pos.col * cell_side, enemy_pos.row * cell_side);
		cell.setFillColor(params.enemy_color);
		window.draw(cell);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

int main() {
	GameState init_state;
	init_state.m_field = {
		{ CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty},
		{ CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty },
		{ CellType::Empty, CellType::Empty, CellType::Wall,  CellType::Empty, CellType::Empty, CellType::Wall,  CellType::Empty, CellType::Empty },
		{ CellType::Empty, CellType::Empty, CellType::Wall,  CellType::Wall,  CellType::Wall,  CellType::Wall,  CellType::Empty, CellType::Empty },
		{ CellType::Empty, CellType::Empty, CellType::Wall,  CellType::Empty, CellType::Empty, CellType::Wall,  CellType::Empty, CellType::Empty },
		{ CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty },
		{ CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty },
	};
	init_state.m_player_pos.row = 0;
	init_state.m_player_pos.col = 0;
	init_state.m_enemy_pos = { {0,5}, {5,0} };

	GameManager manager(init_state);
	const unsigned int win_width = 640u;
	const unsigned int win_height = 480u;

	sf::RenderWindow window(sf::VideoMode({ win_width, win_height }), "CMake SFML Project");
    window.setFramerateLimit(144);

	const int field_rows = init_state.m_field.size();
	const int field_cols = init_state.m_field[0].size();

	const float width_ratio = (float)win_width / field_cols;
	const float height_ratio = (float)win_height / field_rows;

	const float cell_side = std::min(width_ratio, height_ratio);

	RenderParams render_params;
	render_params.cell_side = cell_side;

	sf::Font font;
	if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.TTF"))
		return EXIT_FAILURE;

	GameStatus status = GameStatus::StartMenu;
	sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
				window.close();
        }
		
        window.clear();

		if (status == GameStatus::StartMenu) {

			// create button
			const float button_width = 200.0f;
			const float button_height = 100.0f;
			sf::RectangleShape button({ button_width, button_height });
			float button_left = (win_width - button_width) / 2.0f;
			float button_top = (win_height - button_height) / 2.0f;
			float button_right = button_left + button_width;
			float button_bottom = button_top + button_height;
			button.setPosition(button_left, button_top);
			button.setFillColor(sf::Color(100, 150, 100));
			button.setOutlineColor(sf::Color(100, 100, 200));
			button.setOutlineThickness(3.0f);

			// check if mouse is above the button
			sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
			if (mouse_pos.x >= button_left &&
				mouse_pos.x <= button_right &&
				mouse_pos.y >= button_top &&
				mouse_pos.y <= button_bottom) {
				button.setOutlineColor(sf::Color(200, 100, 100));

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					status = GameStatus::InGame;
			}

			// create text
			sf::Text text("Hello", font, 24); // a font is required to make a text object!
			text.setFillColor(sf::Color::Black);
			text.setPosition((win_width - button_width) / 2.0f, (win_height - button_height) / 2.0f);
			
			window.draw(button);
			window.draw(text);
			window.display();
		}
		else {
			auto state = manager.get_state();
			render_world(state, window, render_params);

			window.display();

			Direction dir = Direction::None;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
				dir = Direction::Left;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
				dir = Direction::Right;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
				dir = Direction::Down;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
				dir = Direction::Up;

			// If collided to enemy, show modal window and die
			if (manager.update_world(dir)) {

				sf::RenderWindow popUpWindow(sf::VideoMode(320, 240), "GAME OVER", sf::Style::Close);
				popUpWindow.setPosition(window.getPosition() + sf::Vector2i(100, 100));
				sf::Event event;
				while (popUpWindow.isOpen())
				{
					popUpWindow.clear(sf::Color::Cyan);
					popUpWindow.display();

					while (popUpWindow.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
							popUpWindow.close();
						if (event.type == sf::Event::LostFocus)
						{
							if (window.hasFocus())
								popUpWindow.requestFocus();
						}
					}
					sf::sleep(sf::milliseconds(1));
				}
				window.close();
			}
		}
    }

	return 0;
}
