#include <chrono>
#include <iostream>

#include "Search.h"
#include "Board.h"

S32 Search::EvNode(S8 depth)
{
	if (depth <= 0 && !board.isDead())
	{
		return board.evaluate();
	}
	else if (board.isDead())
	{
		return S32_MIN;
	}
	else if (board.isWon())
	{
		return S32_MAX;
	}

	S32 score = 0;

	for (Response response : board.getAllResponses())
	{
		nodes++;
		float probability = response.getProbability(board);

		board.respond(response);
		score += probability * maxNode(depth - 1);
		board.unRespond(response);
	}

	return score;
}

S32 Search::maxNode(S8 depth)
{
	if (depth <= 0 && !board.isDead())
	{
		return board.evaluate();
	}
	else if (board.isDead())
	{
		return S32_MIN;
	}
	else if (board.isWon())
	{
		return S32_MAX;
	}

	S32 bestScore = S32_MIN;
	for (Move move : allMoves)
	{
		nodes++;
		board.moveNR(move);
		if (board.wasPreviousMoveIllegal())
		{
			board.unMove();
			continue;
		}
		bestScore = std::max(bestScore, EvNode(depth - 1));
		board.unMove();
	}

	return bestScore;
}

Move Search::EvMax(S8 depth)
{
	Move bestMove = Move::NoMove;
	S32 bestScore = S32_MIN;

	for (Move move : allMoves)
	{
		nodes++;
		board.moveNR(move);
		if (board.wasPreviousMoveIllegal())
		{
			board.unMove();
			continue;
		}

		S32 score = EvNode(depth - 1);
		if (score > bestScore)
		{
			bestScore = score;
			bestMove = move;
		}
		board.unMove();
	}

	return bestMove;
}

Move Search::getBestMove(U8 depth)
{
	nodes = 0;
	std::chrono::time_point<std::chrono::system_clock> start, end;

	start = std::chrono::system_clock::now();
	Move bestMove = EvMax(depth);
	end = std::chrono::system_clock::now();

	U64 msecs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << (double) nodes / msecs << " KN/s" << std::endl;
	return bestMove;
}
