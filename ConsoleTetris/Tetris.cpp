#include "Tetris.h"

void Tetris::setup()
{
}

void Tetris::update(double deltaTime)
{
	window->drawLine(1, 1, window->getWidth() - 1, window->getHeight() - 1);
}

Tetris::Tetris(int consoleWidth, int consoleHeight, int fontWidth, int fontHeight)
	: Engine(consoleWidth, consoleHeight, fontWidth, fontHeight)
{
}

Tetris::~Tetris()
{
}