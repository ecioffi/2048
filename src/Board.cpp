#include <Board.h>

#include <iostream>

const constexpr char* Board::printString[12];
const constexpr U8 Board::newTile[10];

std::vector<U8> Board::getEmptySquares()
{
	std::vector<U8> squares;
	squares.reserve(16);

	for (U8 index = 0; index < 16; index++)
	{
		if (!board[index])
		{
			squares.push_back(index);
		}
	}
	squares.shrink_to_fit();
	return squares;
}

void Board::moveUp()
{
	for (U8 sq = A1; sq < D1; sq++)
		moveIfAble(sq, sq + 4);

	for (U8 sq = A1; sq < D1; sq++)
		mergeIfAbleAA(sq, sq + 4, sq, 1);

	for (U8 sq = A1; sq < D1; sq++)
		moveIfAble(sq, sq + 4);
}

void Board::moveDown()
{
	for (U8 sq = D4; sq > A4; sq--)
		moveIfAble(sq, sq - 4);

	for (U8 sq = D4; sq > A4; sq--)
		mergeIfAbleAA(sq, sq - 4, sq, -1);

	for (U8 sq = D4; sq > A4; sq--)
		moveIfAble(sq, sq - 4);
}

void Board::moveLeft()
{
	for (U8 bsq = A1; bsq < D4; bsq += 4)
	{
		for (U8 sq = bsq; sq < bsq + 3; sq++)
			moveIfAble(sq, sq + 1);

		for (U8 sq = bsq; sq < bsq + 3; sq++)
			mergeIfAbleAA(sq + 1, sq, sq, 1);

		for (U8 sq = bsq; sq < bsq + 3; sq++)
			moveIfAble(sq, sq + 1);
	}
}

void Board::moveRight()
{
	for (U8 bsq = D4; bsq < D4 + 1; bsq -= 4)
	{
		for (U8 sq = bsq; sq > bsq - 3; sq--)
			moveIfAble(sq - 1, sq);

		for (U8 sq = bsq; sq > bsq - 3; sq--)
			mergeIfAbleAA(sq - 1, sq, sq, -1);

		for (U8 sq = bsq; sq > bsq - 3; sq--)
			moveIfAble(sq - 1, sq);
	}
}

void Board::putNewTile()
{
	std::vector<U8> emptySquares = getEmptySquares();
	board[emptySquares[distribution(engine) % emptySquares.size()]] = getNewTile();
}

void Board::move(U8 moveType)
{
	history.push_back(history.back());
	board = &history.back()[0];
	switch (moveType)
	{
		case UP:
			moveUp();
			break;
		case DOWN:
			moveDown();
			break;
		case LEFT:
			moveLeft();
			break;
		case RIGHT:
			moveRight();
			break;
	}
	putNewTile();
}

void Board::print()
{
	for (U8 col = 3; col < 4; col--)
	{
		std::cout << "+----+----+----+----+\n";
		for (U8 row = 0; row < 4; row++)
		{
			std::cout << "|" << printString[board[getSqIndex(row, col)]];
		}
		std::cout << "|\n";
	}
	std::cout << "+----+----+----+----+\n" << std::endl;
}

void Board::test()
{

}
