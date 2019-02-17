#pragma once

#include "ConsoleEngine.h"
#include "Tetromino.h"

#include <vector>

class Tetris : public ConsoleEngine::Engine
{
private:
	std::vector<Tetromino> tetrominos;

protected:
	virtual void setup() override;
	virtual void update(double deltaTime) override;

public:
	Tetris(int consoleWidth, int consoleHeight, int fontWidth = 8, int fontHeight = 8);
	~Tetris();
};