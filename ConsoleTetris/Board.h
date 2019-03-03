#pragma once

#include "Tetromino.h"

#include <vector>
#include <string>

class Board
{
private:
	static constexpr ce::Pixel::Color pixel_color = ce::Pixel::Color::White;
	static constexpr ce::Text::Type   fill_type   = ce::Text::Type::FullBlock;
	static constexpr ce::Text::Color  fill_color  = ce::Text::Color::Grey;

	int width;
	int height;

	ce::Vector2Int position;
	// -1 == wall, 0 == empty, >0 == block with ce::Text::Color color
	std::vector<std::vector<int>> collisions;
	Tetromino* fallingTetromino;
	bool collisionsVisible;

	int score;

	Tetromino::Type nextFallingTetrominoType;

	auto isFallingTetrominoColliding() const          -> bool;
	auto updateCollisions(const Tetromino* tetromino) -> void;

public:
	Board(const int width, const int height, const int windowWidth, const int windowHeight);
	~Board();

	auto isFallingTetrominoSpawned() const -> bool;
	auto spawnFallingTetromino()           -> bool;
	auto rotateFallingTetromino()          -> void;
	auto moveLeftFallingTetromino()        -> void;
	auto moveRightFallingTetromino()       -> void;
	auto fallFallingTetromino()            -> void;

	auto getRowsCompleted() const                                                  -> std::vector<int>;
	auto changeRowsColor(const std::vector<int>& rows, const ce::Text::Color fill) -> void;
	auto deleteRows(const std::vector<int>& rows)                                  -> void;

	auto showCollisions()             -> void;
	auto hideCollisions()             -> void;
	auto areCollisionsVisible() const -> bool;

	auto getPosition() const          -> ce::Vector2Int;
	auto updateScore(const int score) -> void;

	auto clear()                      -> void;
 
	auto draw(ce::Engine& engine)     -> void;
};