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
	/* -1 == wall, 0 == empty, >0 == block with ConsoleEngine::TEXT_COLOR color*/
	std::vector<std::vector<int>> collisions;
	Tetromino* fallingTetromino;

	bool isFallingTetrominoColliding();
	void updateCollisions(Tetromino* tetromino);

	bool collisionsVisible;

public:
	Board(int width, int height, ConsoleEngine::Window* window);
	~Board();

	bool isFallingTetrominoSpawned();
	void spawnFallingTetromino();
	void rotateFallingTetromino();
	void moveLeftFallingTetromino();
	void moveRightFallingTetromino();
	void fallFallingTetromino();

	std::vector<int> getRowsCompleted();
	void changeRowsColor(std::vector<int> rows, ConsoleEngine::TEXT_COLOR fill);
	void deleteRows(std::vector<int> rows);

	inline void showCollisions() { collisionsVisible = true; }
	inline void hideCollisions() { collisionsVisible = false; }
	bool areCollisionsVisible() { return collisionsVisible; }

	void draw();
};