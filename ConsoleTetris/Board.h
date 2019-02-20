#pragma once

#include "Tetromino.h"

#include <vector>

class Board
{
private:
	int width;
	int height;

	ConsoleEngine::Window* window;

	Vector2 position;

	std::vector<std::vector<bool>> collisions;

	Tetromino* fallingTetromino;

	bool isFallingTetrominoColliding();

	inline void drawBoundaries();

public:
	Board(int width, int height, ConsoleEngine::Window* window);
	~Board();

	bool isFallingTetrominoSpawned();
	void spawnFallingTetromino();
	void rotateFallingTetromino();
	void moveLeftFallingTetromino();
	void moveRightFallingTetromino();

	void draw();
};