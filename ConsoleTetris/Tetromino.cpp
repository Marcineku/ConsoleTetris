#include "Tetromino.h"

const wchar_t* Tetromino::patterns[ce::to_underlying(Type::Quantity)] = { L"..X."
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

auto Tetromino::transpose(std::wstring& arr, const int cols) -> void
{
	for (int col = 0; col < cols - 1; ++col)
	{
		for (int row = col + 1; row < cols; ++row)
		{
			std::swap(arr[ce::to_array_point(col, row, cols)], arr[ce::to_array_point(row, col, cols)]);
		}
	}
}

auto Tetromino::reverseRows(std::wstring& arr, const int cols) -> void
{
	for (int row = 0; row < cols; ++row)
	{
		for (int col = 0; col < cols / 2; ++col)
		{
			std::swap(arr[ce::to_array_point(row, col, cols)], arr[ce::to_array_point(row, cols - col - 1, cols)]);
		}
	}
}

auto Tetromino::reverseCols(std::wstring& arr, const int cols) -> void
{
	for (int col = 0; col < cols; ++col)
	{
		for (int row = 0; row < cols / 2; ++row)
		{
			std::swap(arr[ce::to_array_point(row, col, cols)], arr[ce::to_array_point(cols - row - 1, col, cols)]);
		}
	}
}

auto Tetromino::getPattern(const Type type, const int rotation) -> std::wstring
{
	std::wstring pattern(patterns[ce::to_underlying(type)]);

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

Tetromino::Tetromino(const int x, const int y, const Type type)
	:
	position(x, y),
	type(type),
	rotation(0)
{
	switch (type)
	{
	case Type::I:
		fill = ce::Text::Color::Cyan;
		break;
	case Type::J:
		fill = ce::Text::Color::Blue;
		break;
	case Type::L:
		fill = ce::Text::Color::Orange;
		break;
	case Type::O:
		fill = ce::Text::Color::Yellow;
		break;
	case Type::S:
		fill = ce::Text::Color::Green;
		break;
	case Type::T:
		fill = ce::Text::Color::Violet;
		break;
	case Type::Z:
		fill = ce::Text::Color::Red;
		break;
	}
}

Tetromino::Tetromino(const int x, const int y)
	: Tetromino(x, y, static_cast<Type>(std::rand() % ce::to_underlying(Type::Quantity)))
{}

auto Tetromino::rotateClockwise() -> void
{
	rotation += 90;

	if (rotation > 270)
		rotation = 0;
}

auto Tetromino::rotateCounterClockwise() -> void
{
	rotation -= 90;

	if (rotation < 0)
		rotation = 270;
}

auto Tetromino::fall() -> void
{
	position.y += 1;
}

auto Tetromino::rise() -> void
{
	position.y -= 1;
}

auto Tetromino::moveLeft() -> void
{
	position.x -= 1;
}

auto Tetromino::moveRight() -> void
{
	position.x += 1;
}

auto Tetromino::getPattern() const -> std::wstring
{
	return getPattern(type, rotation);
}

auto Tetromino::getPoints() const -> std::vector<ce::Vector2Int>
{
	std::vector<ce::Vector2Int> points;

	for (size_t i = 0; i < pattern_width; ++i)
	{
		for (size_t j = 0; j < pattern_width; ++j)
		{
			if (getPattern()[ce::to_array_point(i, j, pattern_width)] == L'X')
			{
				points.push_back(position + ce::Vector2Int(i, j));
			}
		}
	}

	return points;
}

auto Tetromino::getPosition() const -> ce::Vector2Int
{
	return position;
}

auto Tetromino::getFillColor() const -> ce::Text::Color
{
	return fill;
}

auto Tetromino::draw(ce::Engine& engine) const -> void
{
	const std::wstring pattern = getPattern(type, rotation);

	for (size_t i = 0; i < pattern.length(); ++i)
	{
		if (pattern[i] == L'X')
		{
			const ce::Vector2Int v(i % pattern_width, i / pattern_width);
			engine.draw(position + v, pixel_color, text_type, fill);
		}
	}
}