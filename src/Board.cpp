#include <cmath>
#include <iostream>

#include <Board.h>
#include <set>

const constexpr U8 Board::newTile[10];
const constexpr char* Board::printString[14];

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
	for (U8 sq = start; sq != (stop + inc); sq += inc)
		if (isRSqFull(row, sq))
			return sq;
	return start;
}

U8 Board::getNextEmptyRSq(U8 row, U8 start, U8 stop, U8 inc)
{
	for (U8 sq = start; sq != (stop + inc); sq += inc)
		if (isRSqEmpty(row, sq))
			return sq;
	return start;
}

MoveResult Board::moveRight(U8 row)
{
	U8 mergeBits = 0;
	for (U8 sq = 3; sq > 0; sq--)
	{
		if (isRSqEmpty(row, sq))
		{
			const U8 sSq = getNextFullRSq(row, sq, 0, -1);
			setRSqValue(row, sq, getRSqValue(row, sSq));
			setRSqValue(row, sSq, 0);
		}
	}

	for (S8 sq = 3; sq > 0; sq--)
	{
		if (getRSqValue(row, sq) == getRSqValue(row, sq - 1) && getRSqValue(row, sq) != 0)
		{
			setRSqValue(row, sq - 1, 0);
			mergeBits |= 1 << sq;

			for (U8 sq = 3; sq > 0; sq--)
			{
				if (isRSqEmpty(row, sq))
				{
					const U8 sSq = getNextFullRSq(row, sq, 0, -1);
					setRSqValue(row, sq, getRSqValue(row, sSq));
					setRSqValue(row, sSq, 0);
				}
			}
			//sq--;
		}
	}

	//	for (U8 sq = 3; sq > 0; sq--)
	//	{
	//		if (isRSqEmpty(row, sq))
	//		{
	//			const U8 sSq = getNextFullRSq(row, sq, 0, -1);
	//			setRSqValue(row, sq, getRSqValue(row, sSq));
	//			setRSqValue(row, sSq, 0);
	//		}
	//	}

	return {row, mergeBits};
}

MoveResult Board::moveLeft(U8 row)
{
	U8 mergeBits = 0;
	for (U8 sq = 0; sq < 3; sq++)
	{
		if (isRSqEmpty(row, sq))
		{
			const U8 sSq = getNextFullRSq(row, sq, 4, 1);
			setRSqValue(row, sq, getRSqValue(row, sSq));
			setRSqValue(row, sSq, 0);
		}
	}

	for (U8 sq = 0; sq < 3; sq++)
	{
		if (getRSqValue(row, sq) == getRSqValue(row, sq + 1) && getRSqValue(row, sq) != 0)
		{
			setRSqValue(row, sq + 1, 0);
			mergeBits |= 1 << sq;

			for (U8 sq = 0; sq < 3; sq++)
			{
				if (isRSqEmpty(row, sq))
				{
					const U8 sSq = getNextFullRSq(row, sq, 4, 1);
					setRSqValue(row, sq, getRSqValue(row, sSq));
					setRSqValue(row, sSq, 0);
				}
			}
			//sq++;
		}
	}

//	for (U8 sq = 0; sq < 3; sq++)
//	{
//		if (isRSqEmpty(row, sq))
//		{
//			const U8 sSq = getNextFullRSq(row, sq, 4, 1);
//			setRSqValue(row, sq, getRSqValue(row, sSq));
//			setRSqValue(row, sSq, 0);
//		}
//	}

	return {row, mergeBits};
}

void Board::doMove(Move move)
{
	if ((U8) move > (U8) Move::Left)
	{
		move = (Move) ((U8) move - 2);
		for (U8 x = 0; x < 4; x++)
		{
			std::array<U8, 4> codeToTile = {};
			std::array<U8, winTile + 1> tileToCode = {};
			U8 row = 0;
			U8 nextCode = 1;
			for (U8 sq = 0; sq < 4; sq++)
			{
				if (tileToCode[board[getSqIndex(x, sq)]] == 0 && board[getSqIndex(x, sq)] != 0)
				{
					if (nextCode == 4)
						nextCode = 0;
					tileToCode[board[getSqIndex(x, sq)]] = nextCode;
					codeToTile[nextCode] = board[getSqIndex(x, sq)];
					nextCode++;
				}
				setRSqValue(row, sq, tileToCode[board[getSqIndex(x, sq)]]);
			}
			for (U8 sq = 0; sq < 4; sq++)
			{
				board[getSqIndex(x, sq)] = codeToTile[getRSqValue(moveResultsLR[(U8) move][row].row, sq)] + ((moveResultsLR[(U8) move][row].mergeBits >> sq) & 1);
			}
		}
	}
	else
	{
		for (U8 y = 0; y < 4; y++)
		{
			std::array<U8, 4> codeToTile = {};
			std::array<U8, winTile + 1> tileToCode = {};
			U8 row = 0;
			U8 nextCode = 1;
			for (U8 sq = 0; sq < 4; sq++)
			{
				if (tileToCode[board[getSqIndex(sq, y)]] == 0 && board[getSqIndex(sq, y)] != 0)
				{
					if (nextCode == 4)
						nextCode = 0;
					tileToCode[board[getSqIndex(sq, y)]] = nextCode;
					codeToTile[nextCode] = board[getSqIndex(sq, y)];
					nextCode++;
				}
				setRSqValue(row, sq, tileToCode[board[getSqIndex(sq, y)]]);
			}
			for (U8 sq = 0; sq < 4; sq++)
			{
				board[getSqIndex(sq, y)] = codeToTile[getRSqValue(moveResultsLR[(U8) move][row].row, sq)] + ((moveResultsLR[(U8) move][row].mergeBits >> sq) & 1);
			}
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
//				board[A1] = 2;
//				board[B1] = 3;
//				board[C1] = 4;
//				board[D1] = 5;
//
//				print();
//				doMove(Move::Up);
//				print();

	board[A1] = 2;
	board[A2] = 3;
	board[A3] = 4;
	board[A4] = 5;

	print();
	moveNR(Move::Right);
	print();
	unMoveNR();
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
