#include <chrono>
#include <cfloat>
#include <iostream>

#include "Search.h"
#include "Board.h"

static const constexpr float mm[2] = {FLT_MIN, FLT_MAX};

float Search::EvNode(U8 depth)
{
	if (board.isDead() || board.isWon())
	{
		return mm[board.isWon()];
	}
	else if (depth == 0)
	{
		return board.evaluate();
	}

	float score = 0;
	for (Response response : board.getAllResponses())
	{
		nodes++;

		board.respond(response);
		score += response.probability * maxNode(depth - 1);
		board.unRespond(response);
	}

	return score;
}

float Search::maxNode(U8 depth)
{
	if (board.isDead() || board.isWon())
	{
		return mm[board.isWon()];
	}
	else if (depth == 0)
	{
		return board.evaluate();
	}

	float bestScore = FLT_MIN;
	for (Move move : allMoves)
	{
		nodes++;
		board.moveNR(move);
		if (board.wasPreviousMoveIllegal())
		{
			board.unMoveNR();
			continue;
		}
		bestScore = std::max(bestScore, EvNode(depth - 1));
		board.unMoveNR();
	}

	return bestScore;
}

Move Search::EvMax(U8 depth)
{
	Move bestMove = Move::NoMove;
	float bestScore = FLT_MIN;

	for (Move move : allMoves)
	{
		nodes++;
		board.moveNR(move);
		if (board.wasPreviousMoveIllegal())
		{
			board.unMoveNR();
			continue;
		}

		float score = EvNode(depth - 1);
		if (score > bestScore)
		{
			bestScore = score;
			bestMove = move;
		}
		board.unMoveNR();
	}

	return bestMove;
}

Move Search::getBestMove(U8 depth)
{
	nodes = 0;
	std::chrono::time_point<std::chrono::system_clock> start, end;

	start = std::chrono::system_clock::now();
	Move bestMove = EvMax(std::min(std::max(1_U8, depth), maxDepth));
	end = std::chrono::system_clock::now();

	U64 msecs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << (double) nodes / msecs << " KN/s" << std::endl;
	return bestMove;
}
