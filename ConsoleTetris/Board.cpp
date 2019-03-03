#include "Board.h"

auto Board::isFallingTetrominoColliding() const -> bool
{
	if (fallingTetromino != nullptr)
	{
		std::vector<ce::Vector2Int> points = fallingTetromino->getPoints();
		for (unsigned int i = 0; i < points.size(); ++i)
		{
			ce::Vector2Int offset = points[i] - position;
			if (!(offset.x < 0 || offset.x >= width || offset.y < 0 || offset.y >= height) && collisions[offset.y][offset.x])
			{
				return true;
			}
		}
	}

	return false;
}

auto Board::updateCollisions(const Tetromino* tetromino) -> void
{
	if (tetromino != nullptr)
	{
		std::vector<ce::Vector2Int> points = tetromino->getPoints();
		for (unsigned i = 0; i < points.size(); ++i)
		{
			ce::Vector2Int offset = points[i] - position;
			if (!(offset.x < 0 || offset.x >= width || offset.y < 0 || offset.y >= height))
			{
				collisions[offset.y][offset.x] = ce::to_underlying<ce::Text::Color>(tetromino->getFillColor());
			}
		}
	}
}

Board::Board(const int width, const int height, const int windowWidth, const int windowHeight)
	:
	width(width),
	height(height),
	position(windowWidth / 2 - width, windowHeight / 2 - height / 2),
	collisionsVisible(false),
	collisions(height, std::vector<int>(width, 0)),
	score(0),
	nextFallingTetrominoType(static_cast<Tetromino::Type>(std::rand() % ce::to_underlying(Tetromino::Type::Quantity)))
{
	// Setting collisions for boundaries
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (i == height - 1 || j == 0 || j == width - 1)
			{
				collisions[i][j] = -1;
			}
			else
			{
				collisions[i][j] = 0;
			}
		}
	}
}

Board::~Board()
{
	if (fallingTetromino != nullptr)
	{
		delete fallingTetromino;
	}
}

auto Board::isFallingTetrominoSpawned() const -> bool
{
	return fallingTetromino != nullptr;
}

auto Board::spawnFallingTetromino() -> bool
{
	fallingTetromino = new Tetromino(position.x + (width - 1) / 2, position.y, nextFallingTetrominoType);
	nextFallingTetrominoType = static_cast<Tetromino::Type>(std::rand() % ce::to_underlying(Tetromino::Type::Quantity));

	if (isFallingTetrominoColliding())
	{
		return false;
	}

	return true;
}

auto Board::rotateFallingTetromino() -> void
{
	if (fallingTetromino != nullptr)
	{
		fallingTetromino->rotateClockwise();
	}

	if (isFallingTetrominoColliding())
	{
		fallingTetromino->rotateCounterClockwise();
	}
}

auto Board::moveLeftFallingTetromino() -> void
{
	if (fallingTetromino != nullptr)
	{
		fallingTetromino->moveLeft();
	}

	if (isFallingTetrominoColliding())
	{
		fallingTetromino->moveRight();
	}
}

auto Board::moveRightFallingTetromino() -> void
{
	if (fallingTetromino != nullptr)
	{
		fallingTetromino->moveRight();
	}

	if (isFallingTetrominoColliding())
	{
		fallingTetromino->moveLeft();
	}
}

auto Board::fallFallingTetromino() -> void
{
	if (fallingTetromino != nullptr)
	{
		fallingTetromino->fall();
	}

	if (isFallingTetrominoColliding())
	{
		fallingTetromino->rise();
		updateCollisions(fallingTetromino);

		delete fallingTetromino;
		fallingTetromino = nullptr;
	}
}

auto Board::getRowsCompleted() const -> std::vector<int>
{
	std::vector<int> rowsCompleted;
	for (int i = 0; i < height - 1; ++i)
	{
		int blocksInRow = 0;
		for (int j = 1; j < width - 1; ++j)
		{
			if (collisions[i][j])
			{
				++blocksInRow;
			}
		}

		if (blocksInRow == width - 2)
		{
			rowsCompleted.push_back(i);
		}
	}

	return rowsCompleted;
}

auto Board::changeRowsColor(const std::vector<int>& rows, const ce::Text::Color fill) -> void
{
	for (int row : rows)
	{
		for (int i = 1; i < width - 1; ++i)
		{
			collisions[row][i] = ce::to_underlying<ce::Text::Color>(fill);
		}
	}
}
 
auto Board::deleteRows(const std::vector<int>& rows) -> void
{
	for (int row : rows)
	{
		for (int i = row; i > 0; --i)
		{
			collisions[i].swap(collisions[i - 1]);
			for (int j = 1; j < width - 1; ++j)
			{
				collisions[0][j] = 0;
			}
		}
	}
}

auto Board::showCollisions() -> void
{
	collisionsVisible = true;
}

auto Board::hideCollisions() -> void
{
	collisionsVisible = false;
}

auto Board::areCollisionsVisible() const -> bool
{
	return collisionsVisible;
}

auto Board::getPosition() const -> ce::Vector2Int
{
	return position;
}

auto Board::updateScore(const int score) -> void
{
	this->score = score;
}

auto Board::clear() -> void
{
	score = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (i == height - 1 || j == 0 || j == width - 1)
			{
				collisions[i][j] = -1;
			}
			else
			{
				collisions[i][j] = 0;
			}
		}
	}

	if (fallingTetromino != nullptr)
	{
		delete fallingTetromino;
		fallingTetromino = nullptr;
	}
}

auto Board::draw(ce::Engine& engine) -> void
{
	if (fallingTetromino != nullptr)
	{
		fallingTetromino->draw(engine);
	}

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (collisions[i][j] == 0)
			{
				continue;
			}
			else if (collisions[i][j] < 0)
			{
				engine.draw(position.x + j, position.y + i, pixel_color, fill_type, fill_color);
			}
			else if (collisions[i][j] > 0)
			{
				engine.draw(position.x + j, position.y + i, Tetromino::pixel_color, Tetromino::text_type, static_cast<ce::Text::Color>(collisions[i][j]));
			}
		}
	}

	if (collisionsVisible)
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				if (collisions[i][j])
				{
					engine.draw(j + position.x, i + position.y, ce::Pixel::Color::Cyan);
				}
			}
		}
	}

	engine.draw(position + ce::Vector2Int(width + 2, 0), L"Score: " + std::to_wstring(score));

	engine.draw(position + ce::Vector2Int(width + 2, 6), L"Next:");
	Tetromino(position.x + width + 2, position.y + 8, nextFallingTetrominoType).draw(engine);
}