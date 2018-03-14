#ifndef SEARCH_H_
#define SEARCH_H_

#include "Defs.h"

class Board;
enum class Move : U8;

struct bestMove {
	Move move;
	U8 score;
	U8 searchDepth;
	U8 gameDepth;
};

class Search
{
	private:
		Board& board;
		U64 nodes;
		float probability;



		float EvNode(U8 depth);
		float maxNode(U8 depth);
		Move EvMax(U8 depth);
	public:
		Search(Board& board) : board(board) { }

		Move getBestMove(U8 depth);
};

#endif /* SEARCH_H_ */
