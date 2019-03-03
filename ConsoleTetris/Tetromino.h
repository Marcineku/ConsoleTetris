#pragma once

#include "ConsoleEngine.h"

#include <string>
#include <vector>

class Tetromino
{
public:
	enum class Type { I, J, L, O, S, T, Z, Quantity };
	static constexpr unsigned int     pattern_width = 4;
	static constexpr ce::Pixel::Color pixel_color   = ce::Pixel::Color::White;
	static constexpr ce::Text::Type   text_type     = ce::Text::Type::FullBlock;

private:
	static const wchar_t* patterns[ce::to_underlying(Type::Quantity)];

	static auto transpose(std::wstring& arr, const int cols)   -> void;
	static auto reverseRows(std::wstring& arr, const int cols) -> void;
	static auto reverseCols(std::wstring& arr, const int cols) -> void;

private:
	ce::Vector2Int position;
	Type type;

	ce::Text::Color fill;
	int rotation;

public:
	static auto getPattern(const Type type, const int rotation)->std::wstring;

	Tetromino(const int x, const int y, const Type type);
	Tetromino(const int x, const int y);

	auto rotateClockwise()        -> void;
	auto rotateCounterClockwise() -> void;
	auto fall()                   -> void;
	auto rise()                   -> void;
	auto moveLeft()               -> void;
	auto moveRight()              -> void;

	auto getPattern() const   -> std::wstring;
	auto getPoints() const    -> std::vector<ce::Vector2Int>;
	auto getPosition() const  -> ce::Vector2Int;
	auto getFillColor() const -> ce::Text::Color;

	auto draw(ce::Engine& engine) const -> void;
};