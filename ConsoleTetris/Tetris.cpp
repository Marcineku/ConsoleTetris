#include "Tetris.h"

void Tetris::setup()
{
}

void Tetris::update(double deltaTime)
{
	if (paused)
	{
		pauseTime += deltaTime;

		if (pauseTime >= stopPauseAfterTime)
		{
			paused = false;
			pauseTime = 0.0;
			std::vector<int> rowsCompleted = board->getRowsCompleted();
			board->deleteRows(rowsCompleted);
		}
	}
	else
	{
		// Updating move timers
		if (isKeyDown(ConsoleEngine::KEY_LEFT) || isKeyDown(ConsoleEngine::KEY_A))
			moveLeftKeyDownTime += deltaTime;
		else if (isKeyDown(ConsoleEngine::KEY_RIGHT) || isKeyDown(ConsoleEngine::KEY_D))
			moveRightKeyDownTime += deltaTime;

		// Update fall faster timer
		if (isKeyDown(ConsoleEngine::KEY_DOWN) || isKeyDown(ConsoleEngine::KEY_S))
			fallFasterKeyDownTime += deltaTime;

		// Updating fall timer
		fallTimer += deltaTime;

		// Spawing falling tetromino if there is no falling tetromino
		if (!board->isFallingTetrominoSpawned())
			board->spawnFallingTetromino();

		// Rotating falling tetromino immediately after key press
		if (isKeyPressed(ConsoleEngine::KEY_UP) || isKeyPressed(ConsoleEngine::KEY_W))
			board->rotateFallingTetromino();

		// Moving falling tetromino immediately after key press
		if (isKeyPressed(ConsoleEngine::KEY_LEFT) || isKeyPressed(ConsoleEngine::KEY_A))
			board->moveLeftFallingTetromino();
		else if (isKeyPressed(ConsoleEngine::KEY_RIGHT) || isKeyPressed(ConsoleEngine::KEY_D))
			board->moveRightFallingTetromino();

		// Falling tetromino falls faster immediately after key press
		if (isKeyPressed(ConsoleEngine::KEY_DOWN) || isKeyPressed(ConsoleEngine::KEY_S))
			board->fallFallingTetromino();

		// Resetting move timer
		if (isKeyReleased(ConsoleEngine::KEY_LEFT) || isKeyReleased(ConsoleEngine::KEY_A))
			moveLeftKeyDownTime = 0.0;
		else if (isKeyReleased(ConsoleEngine::KEY_RIGHT) || isKeyReleased(ConsoleEngine::KEY_D))
			moveRightKeyDownTime = 0.0;

		// Moving falling tetromino after holding move key for ${moveAfterTime} seconds
		if (moveLeftKeyDownTime >= moveAfterTime)
		{
			moveLeftKeyDownTime = 0.0;
			board->moveLeftFallingTetromino();
		}
		else if (moveRightKeyDownTime >= moveAfterTime)
		{
			moveRightKeyDownTime = 0.0;
			board->moveRightFallingTetromino();
		}

		// Falling tetromino falls faster after holding falling faster key for ${moveAfterTime} seconds
		if (fallFasterKeyDownTime >= moveAfterTime)
		{
			fallFasterKeyDownTime = 0.0;
			board->fallFallingTetromino();
		}

		// Make falling tetromino fall
		if (fallTimer >= fallAfterTime)
		{
			fallTimer = 0.0;
			board->fallFallingTetromino();
		}

		std::vector<int> rowsCompleted = board->getRowsCompleted();
		if (rowsCompleted.size() > 0)
		{
			paused = true;
			pauseTime = 0.0;
			board->changeRowsColor(rowsCompleted, ConsoleEngine::TEXT_LIGHT_GREY);
		}
	}
	
	board->draw();
}

Tetris::Tetris(int consoleWidth, int consoleHeight, int fontWidth, int fontHeight)
	: Engine(consoleWidth, consoleHeight, fontWidth, fontHeight)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	paused = false;
	pauseTime = 0.0;
	stopPauseAfterTime = 0.5;

	board = new Board(16, 30, window);

	moveLeftKeyDownTime = 0.0;
	moveRightKeyDownTime = 0.0;
	fallFasterKeyDownTime = 0.0;

	fallAfterTime = 0.25;
	fallTimer = 0.0;

	points = 0;
}

Tetris::~Tetris()
{
	delete board;
}