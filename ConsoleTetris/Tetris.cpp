#include "Tetris.h"

void Tetris::setup()
{
}

void Tetris::update(double deltaTime)
{
	// Updating move timers
	if (isKeyDown(ConsoleEngine::KEY_LEFT) || isKeyDown(ConsoleEngine::KEY_A))
		moveLeftKeyDownTime += deltaTime;
	else if (isKeyDown(ConsoleEngine::KEY_RIGHT) || isKeyDown(ConsoleEngine::KEY_D))
		moveRightKeyDownTime += deltaTime;

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

	// Resetting move timer
	if (isKeyReleased(ConsoleEngine::KEY_LEFT) || isKeyReleased(ConsoleEngine::KEY_A))
		moveLeftKeyDownTime = 0.0;
	else if (isKeyReleased(ConsoleEngine::KEY_RIGHT) || isKeyReleased(ConsoleEngine::KEY_D))
		moveRightKeyDownTime = 0.0;

	// Moving falling tetromino after holding move key for ${moveAfterTime} seconds;
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

	board->draw();
}

Tetris::Tetris(int consoleWidth, int consoleHeight, int fontWidth, int fontHeight)
	: Engine(consoleWidth, consoleHeight, fontWidth, fontHeight)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	board = new Board(16, 40, window);

	moveLeftKeyDownTime = 0.0;
	moveRightKeyDownTime = 0.0;
}

Tetris::~Tetris()
{
	delete board;
}