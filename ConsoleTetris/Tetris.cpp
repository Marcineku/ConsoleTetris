#include "Tetris.h"

void Tetris::setup()
{
}

void Tetris::update(double deltaTime)
{
	for (Tetromino tetromino : tetrominos)
		tetromino.draw(window);
}

Tetris::Tetris(int consoleWidth, int consoleHeight, int fontWidth, int fontHeight)
	: Engine(consoleWidth, consoleHeight, fontWidth, fontHeight)
{
	Tetromino tetromino(0, 0, TetrominoType::I);
	tetrominos.push_back(tetromino);
}

Tetris::~Tetris()
{
}