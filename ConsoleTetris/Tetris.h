#pragma once

#include "ConsoleEngine.h"

class Tetris : public ConsoleEngine::Engine
{
private:

protected:
	virtual void setup() override;
	virtual void update(double deltaTime) override;

public:
	Tetris(int consoleWidth, int consoleHeight, int fontWidth = 8, int fontHeight = 8);
	~Tetris();
};