#include <chrono>
#include <iostream>
#include <cfloat>

#include "Search.h"
#include "Board.h"

static constexpr float probabilityCutoff = 0.001;
static constexpr float mm[2] = {FLT_MIN, FLT_MAX};



float Search::EvNode(U8 depth)
{
	if (depth == 0)
	{
		return board.evaluate();
	}
	else if (board.isDead() || board.isWon())
	{
		return mm[board.isWon()];
	}

	float score = 0;

	ResponseArray responses = board.getAllResponses();
	for (U8 i = 0; i < responses.size; i++)
	{
		nodes++;

		probability *= responses[i].probability();
		board.respond(responses[i]);
		score += responses[i].probability() * maxNode(depth - 1);
		board.unRespond(responses[i]);
		probability /= responses[i].probability();
	}

	return score;
}

float Search::maxNode(U8 depth)
{
	if (depth == 0 || probability < probabilityCutoff)
	{
		return board.evaluate();
	}
	else if (board.isDead() || board.isWon())
	{
		return mm[board.isWon()];
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
	nodes = 0;

	Move bestMove = Move::NoMove;
	float bestScore = FLT_MIN;
	probability = 1.0;

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
		if (score >= bestScore)
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
	std::chrono::time_point<std::chrono::system_clock> start, end;

	start = std::chrono::system_clock::now();
	Move bestMove = EvMax(std::min(std::max(1_U8, depth), maxDepth));
	end = std::chrono::system_clock::now();

	U64 msecs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << (double) nodes / msecs << " KN/s" << std::endl;
	return bestMove;
}
