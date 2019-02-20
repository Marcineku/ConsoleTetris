#include "Tetromino.h"

const wchar_t* Tetromino::patterns[to_underlying(TetrominoType::SIZE)] = { L"..X."
																		   L"..X."
																		   L"..X."
																		   L"..X.",
																		   
																		   L"...."
																		   L".XXX"
																		   L"...X"
																		   L"....",
																		   
																		   L"...."
																		   L"XXX."
																		   L"X..."
																		   L"....",
																		   
																		   L"...."
																		   L".XX."
																		   L".XX."
																		   L"....",
																		   
																		   L"...."
																		   L"..XX"
																		   L".XX."
																		   L"....",
																		   
																		   L"...."
																		   L".XXX"
																		   L"..X."
																		   L"....",
																		   
																		   L"...."
																		   L"XX.."
																		   L".XX."
																		   L"...." };

inline void Tetromino::swap(wchar_t& a, wchar_t& b)
{
	wchar_t tmp = a;
	a = b;
	b = tmp;
}

inline void Tetromino::transpose(std::wstring& arr, unsigned int cols)
{
	for (unsigned int  col = 0; col < cols - 1; ++col)
		for (unsigned int  row = col + 1; row < cols; ++row)
			swap(arr[to_array_point(col, row, cols)], arr[to_array_point(row, col, cols)]);
}

inline void Tetromino::reverseRows(std::wstring& arr, unsigned int cols)
{
	for (unsigned int  row = 0; row < cols; ++row)
		for (unsigned int  col = 0; col < cols / 2; ++col)
			swap(arr[to_array_point(row, col, cols)], arr[to_array_point(row, cols - col - 1, cols)]);
}

inline void Tetromino::reverseCols(std::wstring& arr, unsigned int cols)
{
	for (unsigned int  col = 0; col < cols; ++col)
		for (unsigned int  row = 0; row < cols / 2; ++row)
			swap(arr[to_array_point(row, col, cols)], arr[to_array_point(cols - row - 1, col, cols)]);
}

std::wstring Tetromino::getPattern(TetrominoType type, int rotation)
{
	std::wstring pattern(patterns[to_underlying(type)]);

	switch (rotation)
	{
	case 90:
		transpose(pattern, pattern_width);
		reverseRows(pattern, pattern_width);
		break;
	case 180:
		reverseRows(pattern, pattern_width);
		reverseCols(pattern, pattern_width);
		break;
	case 270:
		transpose(pattern, pattern_width);
		reverseCols(pattern, pattern_width);
		break;
	}

	return pattern;
}

Tetromino::Tetromino(int x, int y, TetrominoType type, ConsoleEngine::Window* window)
{
	position.x = x;
	position.y = y;

	this->type = type;

	this->window = window;

	switch (type)
	{
	case TetrominoType::I:
		fill = ConsoleEngine::TEXT_CYAN;
		break;
	case TetrominoType::J:
		fill = ConsoleEngine::TEXT_BLUE;
		break;
	case TetrominoType::L:
		fill = ConsoleEngine::TEXT_ORANGE;
		break;
	case TetrominoType::O:
		fill = ConsoleEngine::TEXT_YELLOW;
		break;
	case TetrominoType::S:
		fill = ConsoleEngine::TEXT_GREEN;
		break;
	case TetrominoType::T:
		fill = ConsoleEngine::TEXT_VIOLET;
		break;
	case TetrominoType::Z:
		fill = ConsoleEngine::TEXT_RED;
		break;
	}
	color = ConsoleEngine::PIXEL_WHITE;

	rotation = 0;
}

Tetromino::Tetromino(int x, int y, ConsoleEngine::Window * window)
	: Tetromino(x, y, static_cast<TetrominoType>(rand() % to_underlying(TetrominoType::SIZE)), window)
{
}

Tetromino::~Tetromino()
{
}

void Tetromino::rotateClockwise()
{
	rotation += 90;

	if (rotation > 270)
		rotation = 0;
}

void Tetromino::rotateCounterClockwise()
{
	rotation -= 90;

	if (rotation < 0)
		rotation = 270;
}

void Tetromino::fall()
{
	position.y += 1;
}

void Tetromino::moveLeft()
{
	position.x -= 1;
}

void Tetromino::moveRight()
{
	position.x += 1;
}

std::wstring Tetromino::getPattern()
{
	return getPattern(type, rotation);
}

std::vector<Vector2> Tetromino::getPoints()
{
	std::vector<Vector2> points;

	for (int i = 0; i < pattern_width; ++i)
		for (int j = 0; j < pattern_width; ++j)
			if (getPattern()[to_array_point(j, i, pattern_width)] == L'X')
				points.push_back(position + Vector2(i, j));

	return points;
}

void Tetromino::draw()
{
	std::wstring pattern = getPattern(type, rotation);
	for (unsigned int i = 0; i < pattern.length(); ++i)
		if (pattern[i] == L'X')
		{
			int x = i % pattern_width;
			int y = i / pattern_width;
			window->drawPixel(x + position.x, y + position.y, color, ConsoleEngine::TYPE_FULL_BLOCK, fill);
		}
}