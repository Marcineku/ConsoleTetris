#pragma once

#include "Window.h"

#include <string>

enum class TetrominoType
{
	I, J, L, O, S, T, Z
};

struct Vector2
{
	int x, y;
	Vector2()
	{
		x = 0;
		y = 0;
	}
	Vector2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

class Tetromino
{
private:
	Vector2 position;

	TetrominoType type;
	ConsoleEngine::PIXEL_COLOR color;
	int rotation;

	static std::wstring getPattern(TetrominoType type);

public:
	Tetromino(int x, int y, TetrominoType type);
	~Tetromino();

	void rotate();
	void fall();

	void draw(ConsoleEngine::Window* window);
};