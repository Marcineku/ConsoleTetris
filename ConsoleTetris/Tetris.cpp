#include "Tetris.h"

auto Tetris::restart() -> void
{
	score = 0;
	board.clear();
	state = State::InProgress;
	paused = false;
	fallAfterTime = 0.25;
}

auto Tetris::update(const double deltaTime) -> void
{
	if (isKeyPressed(ce::Key::Esc))
	{
		paused = !paused;

		if (state == State::Over)
		{
			selectedMenuItem = Menu::Restart;
		}
		else
		{
			selectedMenuItem = Menu::Resume;
		}
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
			pauseTime += deltaTime;

			if (pauseTime >= stopPauseAfterTime)
			{
				pausedByEvent = false;
				pauseTime = 0.0;
				std::vector<int> rowsCompleted = board.getRowsCompleted();
				board.deleteRows(rowsCompleted);
				score += rowsCompleted.size() * rowsCompleted.size() * 100;
			}
		}
		else
		{
			// Updating move timers
			if (isKeyHeld(ce::Key::Left) || isKeyHeld(ce::Key::A))
				moveLeftKeyDownTime += deltaTime;
			else if (isKeyHeld(ce::Key::Right) || isKeyHeld(ce::Key::D))
				moveRightKeyDownTime += deltaTime;

			// Update fall faster timer
			if (isKeyHeld(ce::Key::Down) || isKeyHeld(ce::Key::S))
				fallFasterKeyDownTime += deltaTime;

			// Updating fall timer
			fallTimer += deltaTime;

			// Spawing falling tetromino if there is no falling tetromino
			if (!board.isFallingTetrominoSpawned())
			{
				if (board.spawnFallingTetromino())
				{
					fallAfterTime -= 0.001;
					if (fallAfterTime <= 0.0)
					{
						fallAfterTime = 0.0;
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
				board.rotateFallingTetromino();

			// Moving falling tetromino immediately after key press
			if (isKeyPressed(ce::Key::Left) || isKeyPressed(ce::Key::A))
				board.moveLeftFallingTetromino();
			else if (isKeyPressed(ce::Key::Right) || isKeyPressed(ce::Key::D))
				board.moveRightFallingTetromino();

			// Falling tetromino falls faster immediately after key press
			if (isKeyPressed(ce::Key::Down) || isKeyPressed(ce::Key::S))
				board.fallFallingTetromino();

			// Resetting move timer
			if (isKeyReleased(ce::Key::Left) || isKeyReleased(ce::Key::A))
				moveLeftKeyDownTime = 0.0;
			else if (isKeyReleased(ce::Key::Right) || isKeyReleased(ce::Key::D))
				moveRightKeyDownTime = 0.0;

			// Moving falling tetromino after holding move key for ${moveAfterTime} seconds
			if (moveLeftKeyDownTime >= move_after_time)
			{
				moveLeftKeyDownTime = 0.0;
				board.moveLeftFallingTetromino();
			}
			else if (moveRightKeyDownTime >= move_after_time)
			{
				moveRightKeyDownTime = 0.0;
				board.moveRightFallingTetromino();
			}

			// Falling tetromino falls faster after holding falling faster key for move_after_time seconds
			if (fallFasterKeyDownTime >= move_after_time)
			{
				fallFasterKeyDownTime = 0.0;
				board.fallFallingTetromino();
				score += 1;
			}

			// Make falling tetromino fall
			if (fallTimer >= fallAfterTime)
			{
				fallTimer = 0.0;
				board.fallFallingTetromino();
			}

			std::vector<int> rowsCompleted = board.getRowsCompleted();
			if (rowsCompleted.size() > 0)
			{
				pausedByEvent = true;
				pauseTime = 0.0;
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
	pauseTime(0.0),
	stopPauseAfterTime(0.5),
	moveLeftKeyDownTime(0.0),
	moveRightKeyDownTime(0.0),
	fallFasterKeyDownTime(0.0),
	fallAfterTime(0.25),
	fallTimer(0.0),
	score(0)
{
	srand(static_cast<unsigned int>(time(nullptr)));
}