#include <cmath>
#include <iostream>

#include <Board.h>

const constexpr char* Board::printString[12];
const constexpr U8 Board::newTile[10];

std::vector<U8> Board::getEmptySquares()
{
	std::vector<U8> squares;
	squares.reserve(16);

	for (U8 index = 0; index < 16; index++)
		if (board[index] == 0)
			squares.push_back(index);

	squares.shrink_to_fit();
	return squares;
}

U8 Board::getHighestTile()
{
	U8 highestTile = 0;
	for (U8 index = 0; index < 16; index++)
		highestTile = std::max(highestTile, board[index]);
	return highestTile;
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
			moveIfAble(sq + 1, sq);

		for (U8 sq = bsq; sq < bsq + 3; sq++)
			mergeIfAbleAA(sq + 1, sq, sq, 1);

		for (U8 sq = bsq; sq < bsq + 3; sq++)
			moveIfAble(sq + 1, sq);
	}
}

void Board::moveRight()
{
	for (U8 bsq = D4; bsq < NO_SQ; bsq -= 4)
	{
		for (U8 sq = bsq; sq > bsq - 3; sq--)
			moveIfAble(sq - 1, sq);

		for (U8 sq = bsq; sq > bsq - 3; sq--)
			mergeIfAbleAA(sq - 1, sq, sq, -1);

		for (U8 sq = bsq; sq > bsq - 3; sq--)
			moveIfAble(sq - 1, sq);
	}
}

void Board::doMove(Move move)
{
	switch (move)
	{
		case Move::Up:
			moveUp();
			break;
		case Move::Down:
			moveDown();
			break;
		case Move::Left:
			moveLeft();
			break;
		case Move::Right:
			moveRight();
			break;
		case Move::NoMove:
			std::cout << "uhoh nomove" << std::endl;
			exit(1);
			break;
	}
}

std::vector<Response> Board::getAllResponses()
{
	std::vector<Response> responses;
	std::vector<U8> emptySquares = getEmptySquares();
	responses.reserve(emptySquares.size() * 2);

	for (U8 sq : emptySquares)
	{
		responses.push_back(Response(1, sq, 0.9 / emptySquares.size()));
		responses.push_back(Response(2, sq, 0.1 / emptySquares.size()));
	}

	return responses;
}

void Board::respond()
{
	std::vector<U8> emptySquares = getEmptySquares();
	board[emptySquares[distribution(engine) % emptySquares.size()]] = getNewTile();
}

void Board::move(Move move)
{
	doMove(move);
	respond();
	history = {history.back()};
	updateBoardPointer();
}

float Board::getAverageTile()
{

}

float Board::evaluate()
{
	float adjBonus = 0;

	//up
	for (U8 sq = A1; sq < D1; sq++)
	{
		adjBonus += (abs((S8) board[sq] - (S8) board[sq + 4]) == 1) * (1 << std::max(board[sq], board[sq + 4])) * (std::max(board[sq], board[sq + 4]) != 1);
		adjBonus += (board[sq] == board[sq + 4]) * (1 << board[sq]) * (board[sq] != 0);
	}

	//down
	for (U8 sq = D4; sq > A4; sq--)
	{
		adjBonus += (abs((S8) board[sq] - (S8) board[sq - 4]) == 1) * (1 << std::max(board[sq], board[sq - 4])) * (std::max(board[sq], board[sq - 4]) != 1);
		adjBonus += (board[sq] == board[sq - 4]) * (1 << board[sq]) * (board[sq] != 0);
	}

	//left
	for (U8 bsq = A1; bsq < D4; bsq += 4)
	{
		for (U8 sq = bsq; sq < bsq + 3; sq++)
		{
			adjBonus += (abs((S8) board[sq] - (S8) board[sq + 1]) == 1) * (1 << std::max(board[sq], board[sq + 1])) * (std::max(board[sq], board[sq + 1]) != 1);
			adjBonus += (board[sq] == board[sq + 1]) * (1 << board[sq]) * (board[sq] != 0);
		}
	}

	//right
	for (U8 bsq = D4; bsq < NO_SQ; bsq -= 4)
	{
		for (U8 sq = bsq; sq > bsq - 3; sq--)
		{
			adjBonus += (abs((S8) board[sq] - (S8) board[sq - 1]) == 1) * (1 << std::max(board[sq], board[sq - 1])) * (std::max(board[sq], board[sq - 1]) != 1);
			adjBonus += (board[sq] == board[sq - 1]) * (1 << board[sq]) * (board[sq] != 0);
		}
	}

	adjBonus /= 10;

	return adjBonus + (getEmptySquares().size() * getHighestTile());
}

bool Board::isFull()
{
	for (U8 index = 0; index < 16; index++)
		if (board[index] == 0)
			return false;

	return true;
}

bool Board::areNoMerges()
{
	for (U8 sq = A1; sq < D1; sq++)
		if (isMergeable(sq, sq + 4))
			return false;

	for (U8 sq = D4; sq > A4; sq--)
		if (isMergeable(sq, sq - 4))
			return false;

	for (U8 bsq = A1; bsq < D4; bsq += 4)
	{
		for (U8 sq = bsq; sq < bsq + 3; sq++)
			if (isMergeable(sq + 1, sq))
				return false;
	}

	for (U8 bsq = D4; bsq < NO_SQ; bsq -= 4)
	{
		for (U8 sq = bsq; sq > bsq - 3; sq--)
			if (isMergeable(sq - 1, sq))
				return false;
	}

	return true;
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
