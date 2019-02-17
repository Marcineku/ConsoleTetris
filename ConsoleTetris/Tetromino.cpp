#include "Tetromino.h"

std::wstring Tetromino::getPattern(TetrominoType type)
{
	std::wstring pattern;

	switch (type)
	{
	case TetrominoType::I:
		pattern.append(L"..X.");
		pattern.append(L"..X.");
		pattern.append(L"..X.");
		pattern.append(L"..X.");
		break;
	case TetrominoType::J:
		pattern.append(L"....");
		pattern.append(L".XXX");
		pattern.append(L"...X");
		pattern.append(L"....");
		break;
	case TetrominoType::L:
		pattern.append(L"....");
		pattern.append(L"XXX.");
		pattern.append(L"X...");
		pattern.append(L"....");
		break;
	case TetrominoType::O:
		pattern.append(L"....");
		pattern.append(L".XX.");
		pattern.append(L".XX.");
		pattern.append(L"....");
		break;
	case TetrominoType::S:
		pattern.append(L"....");
		pattern.append(L"..XX");
		pattern.append(L".XX.");
		pattern.append(L"....");
		break;
	case TetrominoType::T:
		pattern.append(L"....");
		pattern.append(L".XXX");
		pattern.append(L"..X.");
		pattern.append(L"....");
		break;
	case TetrominoType::Z:
		pattern.append(L"....");
		pattern.append(L"XX..");
		pattern.append(L".XX.");
		pattern.append(L"....");
		break;
	}

	return pattern;
}

Tetromino::Tetromino(int x, int y, TetrominoType type)
{
	position.x = x;
	position.y = y;

	this->type = type;

	switch (type)
	{
	case TetrominoType::I:
		color = ConsoleEngine::PIXEL_CYAN;
		break;
	case TetrominoType::J:
		color = ConsoleEngine::PIXEL_BLUE;
		break;
	case TetrominoType::L:
		color = ConsoleEngine::PIXEL_ORANGE;
		break;
	case TetrominoType::O:
		color = ConsoleEngine::PIXEL_YELLOW;
		break;
	case TetrominoType::S:
		color = ConsoleEngine::PIXEL_GREEN;
		break;
	case TetrominoType::T:
		color = ConsoleEngine::PIXEL_VIOLET;
		break;
	case TetrominoType::Z:
		color = ConsoleEngine::PIXEL_RED;
		break;
	}

	rotation = 0;
}

Tetromino::~Tetromino()
{
}

void Tetromino::rotate()
{
	rotation += 90;

	if (rotation > 270)
		rotation = 0;
}

void Tetromino::fall()
{
	position.y += 1;
}

void Tetromino::draw(ConsoleEngine::Window* window)
{
	std::wstring pattern = getPattern(type);
	for (int i = 0; i < pattern.length(); ++i)
		if (pattern[i] == L'X')
		{
			int x = i % 4;
			int y = i / 4;
			window->drawPixel(x + position.x, y + position.y, color);
		}
}