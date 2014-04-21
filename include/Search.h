#ifndef SEARCH_H_
#define SEARCH_H_

#include "Defs.h"

class Board;
enum class Move : U8;

class Search
{
	private:
		Board& board;

		U64 nodes;

		S32 EvNode(S8 depth);
		S32 maxNode(S8 depth);
		Move EvMax(S8 depth);
	public:
		Search(Board& board) : board(board) { }

		Move getBestMove(U8 depth);
};

#endif /* SEARCH_H_ */
