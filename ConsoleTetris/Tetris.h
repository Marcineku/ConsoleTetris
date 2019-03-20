#pragma once

#include "ConsoleEngine.h"
#include "Board.h"

class Tetris : public ce::Engine
{
private:
	enum class State { InProgress, Over };
	enum class Menu { Resume, Restart, Exit, Quantity };

	static constexpr double move_after_time    = 0.12;
	static constexpr int    board_width        = 16;
	static constexpr int    board_height       = 30;
	static constexpr auto   deleting_row_color = ce::Text::Color::LightGrey;

	Board board;

	State state;

	bool paused;
	Menu selectedMenuItem;

	bool pausedByEvent;

	ce::Timer fallTimer;
	ce::Timer pauseTimer;
	ce::Timer moveLeftTimer;
	ce::Timer moveRightTimer;
	ce::Timer fallFasterTimer;

	int score;

	auto restart() -> void;

protected:
	virtual void update(const double deltaTime) override;

public:
	Tetris(int consoleWidth, int consoleHeight, int fontWidth, int fontHeight);
};