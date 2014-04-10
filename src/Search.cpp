#include <chrono>
#include <iostream>

#include "Search.h"

S32 Search::maxi(U8 depth)
{
	if (depth == 0)
		return board.evaluate();
	S32 max = INT_MIN;

	for (U8 move : board.getAllMoves())
	{
		nodes++;
		board.move(move);
		S32 score = mini(depth - 1);
		max = std::max(max, score);
		board.unMove();
	}
	return max;
}

S32 Search::mini(U8 depth)
{
	if (depth == 0)
		return -board.evaluate();
	S32 min = INT_MAX;

	for (U8 move : board.getAllMoves())
	{
		nodes++;
		board.move(move);
		S32 score = maxi(depth - 1);
		min = std::min(min, score);
		board.unMove();
	}
	return min;
}

U32 Search::minimax(U8 depth)
{
	S32 max = INT_MIN;
	U8 bestMove = NO_MOVE;

	for (U32 move : board.getAllMoves())
	{
		nodes++;
		board.move(move);
		S32 score = maxi(depth - 1);
		if (score > max)
		{
			max = score;
			bestMove = move;
		}
		board.unMove();
	}
	return bestMove;
}

U8 Search::getBestMove(U8 depth)
{
	nodes = 0;
	std::chrono::time_point<std::chrono::system_clock> start, end;

	start = std::chrono::system_clock::now();
	U8 bestMove = minimax(depth);
	end = std::chrono::system_clock::now();

	U64 msecs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << (double) nodes / msecs << " KN/s" << std::endl;
	return bestMove;
}
