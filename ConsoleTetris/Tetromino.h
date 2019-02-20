#pragma once

#include "Window.h"

#include <string>
#include <vector>

constexpr unsigned int to_array_point(int x, int y, int width)
{
	return x * width + y;
}

enum class TetrominoType
{
	I, J, L, O, S, T, Z, SIZE
};
constexpr int to_underlying(TetrominoType type)
{
	return static_cast<std::underlying_type_t<TetrominoType>>(type);
}

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

	Vector2 operator+(const Vector2& v)
	{
		return Vector2(this->x + v.x, this->y + v.y);
	}
	Vector2 operator-(const Vector2& v)
	{
		return Vector2(this->x - v.x, this->y - v.y);
	}
};

class Tetromino
{
private:
	static const wchar_t* patterns[to_underlying(TetrominoType::SIZE)];

	static inline void swap(wchar_t& a, wchar_t& b);
	static inline void transpose(std::wstring& arr, unsigned int cols);
	static inline void reverseRows(std::wstring& arr, unsigned int cols);
	static inline void reverseCols(std::wstring& arr, unsigned int cols);
	static std::wstring getPattern(TetrominoType type, int rotation);

	Vector2 position;
	TetrominoType type;

	ConsoleEngine::Window* window;

	ConsoleEngine::PIXEL_COLOR color;
	ConsoleEngine::TEXT_COLOR fill;
	int rotation;

public:
	static constexpr unsigned int pattern_width = 4;

	Tetromino(int x, int y, TetrominoType type, ConsoleEngine::Window* window);
	Tetromino(int x, int y, ConsoleEngine::Window* window);
	~Tetromino();

	void rotateClockwise();
	void rotateCounterClockwise();
	void fall();
	void moveLeft();
	void moveRight();

	std::wstring getPattern();
	inline Vector2 getPosition() { return Vector2(position.x, position.y); }
	std::vector<Vector2> getPoints();

	void draw();
};