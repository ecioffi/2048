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

std::array<std::array<MoveResult, 256>, 2> Board::getMoveResultsLR()
{
	std::array<std::array<MoveResult, 256>, 2> moveResults;

	for (U8 row = 1; row > 0; row++)
	{
		moveResults[(U8) Move::Right][row] = moveRight(row);
		moveResults[(U8) Move::Left][row] = moveLeft(row);
	}

	return moveResults;
}

U8 Board::getNextFullRSq(U8 row, U8 start, U8 stop, U8 inc)
{
	for (U8 sq = start; sq != stop; sq += inc)
		if (isRSqEmpty(row, sq))
			return sq;
	return start;
}

U8 Board::getNextEmptyRSq(U8 row, U8 start, U8 stop, U8 inc)
{
	for (U8 sq = start; sq != stop; sq += inc)
		if (isRSqFull(row, sq))
			return sq;
	return start;
}

MoveResult Board::moveRight(U8 row)
{
	U8 mergeBits = 0;
	for (U8 sq = 0; sq < 3; sq++)
		if (isRSqEmpty(row, sq))
			setRSqValue(row, sq, getRSqValue(row, getNextFullRSq(row, sq, 4, 1)));

	for (U8 sq = 4; sq > 0; sq--)
	{
		if (getRSqValue(row, sq) == getRSqValue(row, sq - 1))
		{
			setRSqValue(row, sq, 0);
			mergeBits |= 1 << sq;
		}

	}

	for (U8 sq = 0; sq < 3; sq++)
		if (isRSqEmpty(row, sq))
			setRSqValue(row, sq, getRSqValue(row, getNextFullRSq(row, sq, 4, 1)));

	return {row, mergeBits};
}

MoveResult Board::moveLeft(U8 row)
{
	U8 mergeBits = 0;
	for (U8 sq = 4; sq > 0; sq--)
		if (isRSqEmpty(row, sq))
			setRSqValue(row, sq, getRSqValue(row, getNextFullRSq(row, sq, 0, -1)));

	for (U8 sq = 0; sq < 3; sq++)
	{
		if (getRSqValue(row, sq) == getRSqValue(row, sq + 1))
		{
			setRSqValue(row, sq, 0);
			mergeBits |= 1 << sq;
		}

	}

	for (U8 sq = 4; sq > 0; sq--)
		if (isRSqEmpty(row, sq))
			setRSqValue(row, sq, getRSqValue(row, getNextFullRSq(row, sq, 0, -1)));

	return {row, mergeBits};
}

void Board::doMove(Move move)
{
	if ((U8) move > (U8) Move::Right)
	{

	}
	else
	{
		for (U8 y = 0; y < 4; y++)
		{
			std::array<U8, 4> tileKey;
			std::array<U8, winTile> tileCode;
			U8 tileKeyIndex = 1;
			U8 row = 0;
			std::array<U8, 4> target;
			for (U8 sq = 0; sq < 4; sq++)
			{
				target[sq] = y + sq;
				if (board[getSqIndex(sq, y)] != 0 && tileCode[board[getSqIndex(sq, y)]] == 0)
				{
					tileCode[board[getSqIndex(sq, y)]] = tileKeyIndex;
					tileKey[tileKeyIndex] = board[getSqIndex(sq, y)];
				}
				setRSqValue(row, sq, tileCode[board[getSqIndex(sq, y)]]);
			}
			applyMoveResult(target, moveResultsLR[(U8) move][row], tileKey);
		}
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
}

float Board::getAverageTileValue()
{
	U32 sum = 0;
	U8 div = 0;
	for (U8 x = 0; x < 16; x++)
	{
		sum += board[x];
		div += board[x] != 0;
	}

	return (float) sum / (float) div;
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

	adjBonus /= 4;

	return adjBonus + (getEmptySquares().size() * 4096) + getAverageTileValue();
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

void Board::test()
{
//	board[A1] = 2;
//	board[B1] = 2;
//	board[C1] = 2;
//	board[D1] = 0;
//
//	print();
//	doMove(Move::Up);
//	print();

			board[A1] = 2;
			board[A2] = 2;
			board[A3] = 0;
			board[A4] = 2;

			print();
			doMove(Move::Left);
			print();
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
