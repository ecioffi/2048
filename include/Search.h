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

		float EvNode(U8 depth);
		float maxNode(U8 depth);
		Move EvMax(U8 depth);
	public:
		Search(Board& board) : board(board) { }

		Move getBestMove(U8 depth);
};

#endif /* SEARCH_H_ */
