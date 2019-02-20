#include "Board.h"

bool Board::isFallingTetrominoColliding()
{
	if (fallingTetromino != nullptr)
	{
		std::vector<Vector2> points = fallingTetromino->getPoints();
		for (unsigned int i = 0; i < points.size(); ++i)
		{
			Vector2 offset = points[i] - position;
			if (offset.x < 0 || offset.x >= width || offset.y < 0 || offset.y >= height)
				return false;
			else if (collisions[offset.x][offset.y])
				return true;
		}
	}

	return false;
}

inline void Board::drawBoundaries()
{
	// Left line
	window->drawLine(position.x, position.y, position.x, position.y + height - 1, ConsoleEngine::PIXEL_WHITE, ConsoleEngine::TYPE_FULL_BLOCK, ConsoleEngine::TEXT_GREY);

	// Right line
	window->drawLine(position.x + width - 1, position.y, position.x + width - 1, position.y + height - 1, ConsoleEngine::PIXEL_WHITE, ConsoleEngine::TYPE_FULL_BLOCK, ConsoleEngine::TEXT_GREY);

	// Bottom line
	window->drawLine(position.x, position.y + height - 1, position.x + width, position.y + height - 1, ConsoleEngine::PIXEL_WHITE, ConsoleEngine::TYPE_FULL_BLOCK, ConsoleEngine::TEXT_GREY);
}

Board::Board(int width, int height, ConsoleEngine::Window* window)
{
	this->width = width;
	this->height = height;

	this->window = window;

	position.x = window->getWidth() / 2 - width;
	position.y = window->getHeight() / 2 - height / 2;

	// Setting collisions for boundaries
	collisions.assign(width, std::vector<bool>(height, 0));
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			if (i == 0 || i == width - 1 || j == height - 1)
				collisions[i][j] = true;
			else
				collisions[i][j] = false;
		}
}

Board::~Board()
{
	delete fallingTetromino;
}

bool Board::isFallingTetrominoSpawned()
{
	return fallingTetromino != nullptr;
}

void Board::spawnFallingTetromino()
{
	fallingTetromino = new Tetromino(position.x + (width - 1) / 2, position.y, window);
}

void Board::rotateFallingTetromino()
{
	if (fallingTetromino != nullptr)
		fallingTetromino->rotateClockwise();

	if (isFallingTetrominoColliding())
		fallingTetromino->rotateCounterClockwise();
}

void Board::moveLeftFallingTetromino()
{
	if (fallingTetromino != nullptr)
		fallingTetromino->moveLeft();

	if (isFallingTetrominoColliding())
		fallingTetromino->moveRight();
}

void Board::moveRightFallingTetromino()
{
	if (fallingTetromino != nullptr)
		fallingTetromino->moveRight();

	if (isFallingTetrominoColliding())
		fallingTetromino->moveLeft();
}

void Board::draw()
{
	drawBoundaries();

	if (fallingTetromino != nullptr)
		fallingTetromino->draw();
}