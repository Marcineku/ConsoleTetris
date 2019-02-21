#include "Board.h"

bool Board::isFallingTetrominoColliding()
{
	if (fallingTetromino != nullptr)
	{
		std::vector<Vector2> points = fallingTetromino->getPoints();
		for (unsigned int i = 0; i < points.size(); ++i)
		{
			Vector2 offset = points[i] - position;
			if (!(offset.x < 0 || offset.x >= width || offset.y < 0 || offset.y >= height) && collisions[offset.y][offset.x])
				return true;
		}
	}

	return false;
}

inline void Board::updateCollisions(Tetromino* tetromino)
{
	if (tetromino != nullptr)
	{
		std::vector<Vector2> points = tetromino->getPoints();
		for (unsigned i = 0; i < points.size(); ++i)
		{
			Vector2 offset = points[i] - position;
			if (!(offset.x < 0 || offset.x >= width || offset.y < 0 || offset.y >= height))
				collisions[offset.y][offset.x] = tetromino->getFillColor();
		}
	}
}

Board::Board(int width, int height, ConsoleEngine::Window* window)
{
	this->width = width;
	this->height = height;

	this->window = window;

	position.x = window->getWidth() / 2 - width;
	position.y = window->getHeight() / 2 - height / 2;

	// Setting collisions for boundaries
	collisions.assign(height, std::vector<int>(width, 0));
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
		{
			if (i == height - 1 || j == 0 || j == width - 1)
				collisions[i][j] = -1;
			else
				collisions[i][j] = 0;
		}

	collisionsVisible = false;
}

Board::~Board()
{
	if (fallingTetromino != nullptr)
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

void Board::fallFallingTetromino()
{
	if (fallingTetromino != nullptr)
		fallingTetromino->fall();

	if (isFallingTetrominoColliding())
	{
		fallingTetromino->rise();
		updateCollisions(fallingTetromino);

		delete fallingTetromino;
		fallingTetromino = nullptr;
	}
}

std::vector<int> Board::getRowsCompleted()
{
	std::vector<int> rowsCompleted;
	for (int i = 0; i < height - 1; ++i)
	{
		int blocksInRow = 0;
		for (int j = 1; j < width - 1; ++j)
			if (collisions[i][j])
				++blocksInRow;

		if (blocksInRow == width - 2)
			rowsCompleted.push_back(i);
	}

	return rowsCompleted;
}

void Board::changeRowsColor(std::vector<int> rows, ConsoleEngine::TEXT_COLOR fill)
{
	for (int row : rows)
		for (int i = 1; i < width - 1; ++i)
			collisions[row][i] = fill;
}
 
void Board::deleteRows(std::vector<int> rows)
{
	for (int row : rows)
		for (int i = row; i > 0; --i)
		{
			collisions[i].swap(collisions[i - 1]);
			for (int j = 1; j < width - 1; ++j)
				collisions[0][j] = 0;
		}
}

void Board::draw()
{
	if (fallingTetromino != nullptr)
		fallingTetromino->draw();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			if (collisions[i][j] == 0)
				continue;
			else if (collisions[i][j] < 0)
				window->drawPixel(position.x + j, position.y + i, ConsoleEngine::PIXEL_WHITE, ConsoleEngine::TYPE_FULL_BLOCK, ConsoleEngine::TEXT_GREY);
			else if (collisions[i][j] > 0)
				window->drawPixel(position.x + j, position.y + i, Tetromino::color_accent, Tetromino::pixel_type, static_cast<ConsoleEngine::TEXT_COLOR>(collisions[i][j]));

	if (collisionsVisible)
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				if (collisions[i][j])
					window->drawPixel(j + position.x, i + position.y, ConsoleEngine::PIXEL_CYAN);
}