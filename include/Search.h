#ifndef SEARCH_H_
#define SEARCH_H_

#include "Board.h"

class Search
{
	private:
		Board& board;

		U64 nodes;

		S32 searchResponseNode();
		S32 searchMoveNode();
		S32 searchRoot();
	public:
		Search(Board& board) : board(board) { }

		Move getBestMove(U8 depth);
};

#endif /* SEARCH_H_ */
