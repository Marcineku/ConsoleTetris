#include "Tetris.h"

auto Tetris::restart() -> void
{
	score = 0;
	board.clear();
	state = State::InProgress;
	paused = false;
	fallTimer.setTime(0.25);
}

auto Tetris::update(const double deltaTime) -> void
{
	if (isKeyPressed(ce::Key::Esc))
	{
		paused = !paused;

		selectedMenuItem = state == State::Over ? Menu::Restart : Menu::Resume;
	}

	if (paused || state == State::Over)
	{
		if (isKeyPressed(ce::Key::Down) || isKeyPressed(ce::Key::S))
		{
			int selectedMenuItemNumber = ce::to_underlying(selectedMenuItem);
			++selectedMenuItemNumber;
			if (selectedMenuItemNumber >= ce::to_underlying(Menu::Quantity))
			{
				selectedMenuItemNumber = 0;
			}
			if (state == State::Over && static_cast<Menu>(selectedMenuItemNumber) == Menu::Resume)
			{
				++selectedMenuItemNumber;
			}

			selectedMenuItem = static_cast<Menu>(selectedMenuItemNumber);
		}

		if (isKeyPressed(ce::Key::Up) || isKeyPressed(ce::Key::W))
		{
			int selectedMenuItemNumber = ce::to_underlying(selectedMenuItem);
			--selectedMenuItemNumber;
			if (state == State::Over && static_cast<Menu>(selectedMenuItemNumber) == Menu::Resume)
			{
				--selectedMenuItemNumber;
			}
			if (selectedMenuItemNumber < 0)
			{
				selectedMenuItemNumber = ce::to_underlying(Menu::Quantity) - 1;
			}
			selectedMenuItem = static_cast<Menu>(selectedMenuItemNumber);
		}

		const ce::Vector2Int mousePosition = getMousePosition();
		const ce::Vector2Int boardPosition = board.getPosition();

		if (mousePosition.x < boardPosition.x - 4 &&
			mousePosition.x >= boardPosition.x - 12)
		{
			if (state == State::InProgress &&
				mousePosition.y < boardPosition.y + 2 &&
				mousePosition.y >= boardPosition.y - 1)
			{
				if (isKeyPressed(ce::Mouse::Left))
				{
					paused = false;
				}

				selectedMenuItem = Menu::Resume;
			}
			else if (mousePosition.y < boardPosition.y + 6 &&
				     mousePosition.y >= boardPosition.y + 3)
			{
				if (isKeyPressed(ce::Mouse::Left))
				{
					restart();
				}

				selectedMenuItem = Menu::Restart;
			}
			else if (mousePosition.y < boardPosition.y + 10 &&
				     mousePosition.y >= boardPosition.y + 7)
			{
				if (isKeyPressed(ce::Mouse::Left))
				{
					close();
				}

				selectedMenuItem = Menu::Exit;
			}
		}
		

		switch (selectedMenuItem)
		{
		case Tetris::Menu::Resume:
			if (isKeyPressed(ce::Key::Enter))
			{
				paused = false;
			}

			draw(board.getPosition() + ce::Vector2Int(-12, 0), L"Resume", ce::Text::Color::White);
			draw(board.getPosition() + ce::Vector2Int(-12, 4), L"Restart", ce::Text::Color::LightGrey);
			draw(board.getPosition() + ce::Vector2Int(-12, 8), L"Exit", ce::Text::Color::LightGrey);
			draw(board.getPosition() + ce::Vector2Int(-4, 0), L'#', ce::Text::Color::LightGreen);
			break;
		case Tetris::Menu::Restart:
			if (isKeyPressed(ce::Key::Enter))
			{
				restart();
			}

			draw(board.getPosition() + ce::Vector2Int(-12, 0), L"Resume", ce::Text::Color::LightGrey);
			draw(board.getPosition() + ce::Vector2Int(-12, 4), L"Restart", ce::Text::Color::White);
			draw(board.getPosition() + ce::Vector2Int(-12, 8), L"Exit", ce::Text::Color::LightGrey);
			draw(board.getPosition() + ce::Vector2Int(-4, 4), L'#', ce::Text::Color::LightGreen);
			break;
		case Tetris::Menu::Exit:
			if (isKeyPressed(ce::Key::Enter))
			{
				close();
			}

			draw(board.getPosition() + ce::Vector2Int(-12, 0), L"Resume", ce::Text::Color::LightGrey);
			draw(board.getPosition() + ce::Vector2Int(-12, 4), L"Restart", ce::Text::Color::LightGrey);
			draw(board.getPosition() + ce::Vector2Int(-12, 8), L"Exit", ce::Text::Color::White);
			draw(board.getPosition() + ce::Vector2Int(-4, 8), L'#', ce::Text::Color::LightGreen);
			break;
		}

		if (state == State::Over)
		{
			draw(board.getPosition() + ce::Vector2Int(-12, 0), L"Resume", ce::Text::Color::Grey);
		}
		
		switch (state)
		{
		case Tetris::State::InProgress:
			draw(board.getPosition() - ce::Vector2Int(0, 2), L"Game paused");
			break;
		case Tetris::State::Over:
			draw(board.getPosition() - ce::Vector2Int(0, 2), L"Game over");
			break;
		default:
			break;
		}
	}
	else
	{
		if (pausedByEvent)
		{
			pauseTimer.update(deltaTime);

			if (pauseTimer.hasFinished())
			{
				pausedByEvent = false;
				std::vector<int> rowsCompleted = board.getRowsCompleted();
				board.deleteRows(rowsCompleted);
				score += rowsCompleted.size() * rowsCompleted.size() * 100;
			}
		}
		else
		{
			// Updating move timers
			if (isKeyHeld(ce::Key::Left) || isKeyHeld(ce::Key::A))
			{
				moveLeftTimer.update(deltaTime);
			}
			else if (isKeyHeld(ce::Key::Right) || isKeyHeld(ce::Key::D))
			{
				moveRightTimer.update(deltaTime);
			}

			// Update fall faster timer
			if (isKeyHeld(ce::Key::Down) || isKeyHeld(ce::Key::S))
			{
				fallFasterTimer.update(deltaTime);
			}

			// Updating fall timer
			fallTimer.update(deltaTime);

			// Spawing falling tetromino if there is no falling tetromino
			if (!board.isFallingTetrominoSpawned())
			{
				if (board.spawnFallingTetromino())
				{
					fallTimer.setTime(fallTimer.getTime() - 0.001);
					if (fallTimer.getTime() < 0)
					{
						fallTimer.setTime(0.0);
					}
				}
				else
				{
					state = State::Over;
					selectedMenuItem = Menu::Restart;
				}
			}

			// Rotating falling tetromino immediately after key press
			if (isKeyPressed(ce::Key::Up) || isKeyPressed(ce::Key::W))
			{
				board.rotateFallingTetromino();
			}

			// Moving falling tetromino immediately after key press
			if (isKeyPressed(ce::Key::Left) || isKeyPressed(ce::Key::A))
			{
				board.moveLeftFallingTetromino();
			}
			else if (isKeyPressed(ce::Key::Right) || isKeyPressed(ce::Key::D))
			{
				board.moveRightFallingTetromino();
			}

			// Falling tetromino falls faster immediately after key press
			if (isKeyPressed(ce::Key::Down) || isKeyPressed(ce::Key::S))
			{
				board.fallFallingTetromino();
			}

			// Resetting move timer
			if (isKeyReleased(ce::Key::Left) || isKeyReleased(ce::Key::A))
			{
				moveLeftTimer.reset();
			}
			else if (isKeyReleased(ce::Key::Right) || isKeyReleased(ce::Key::D))
			{
				moveRightTimer.reset();
			}

			// Moving falling tetromino after holding move key for ${moveAfterTime} seconds
			if (moveLeftTimer.hasFinished())
			{
				board.moveLeftFallingTetromino();
			}
			else if (moveRightTimer.hasFinished())
			{
				board.moveRightFallingTetromino();
			}

			// Falling tetromino falls faster after holding falling faster key for move_after_time seconds
			if (fallFasterTimer.hasFinished())
			{
				board.fallFallingTetromino();
				score += 1;
			}

			// Make falling tetromino fall
			if (fallTimer.hasFinished())
			{
				board.fallFallingTetromino();
			}

			std::vector<int> rowsCompleted = board.getRowsCompleted();
			if (rowsCompleted.size() > 0)
			{
				pausedByEvent = true;
				pauseTimer.reset();
				board.changeRowsColor(rowsCompleted, deleting_row_color);
			}
		}

		board.updateScore(score);
	}

	board.draw(*this);
}

Tetris::Tetris(int consoleWidth, int consoleHeight, int fontWidth, int fontHeight)
	:
	Engine(consoleWidth, consoleHeight, fontWidth, fontHeight, L"Tetris"),
	board(board_width, board_height, consoleWidth, consoleHeight),
	state(State::InProgress),
	paused(true),
	selectedMenuItem(Menu::Resume),
	pausedByEvent(false),
	fallTimer (0.25),
	pauseTimer(0.5),
	moveLeftTimer (move_after_time),
	moveRightTimer(move_after_time),
	fallFasterTimer(0.1),
	score(0)
{
	srand(static_cast<unsigned int>(time(nullptr)));
}