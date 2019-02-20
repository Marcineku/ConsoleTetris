#pragma once

#include "ConsoleEngine.h"
#include "Board.h"

class Tetris : public ConsoleEngine::Engine
{
private:
	const double moveAfterTime = 0.12;

	Board* board;

	double moveLeftKeyDownTime;
	double moveRightKeyDownTime;

protected:
	virtual void setup() override;
	virtual void update(double deltaTime) override;

public:
	Tetris(int consoleWidth, int consoleHeight, int fontWidth = 8, int fontHeight = 8);
	~Tetris();
};