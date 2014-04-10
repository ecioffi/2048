#ifndef SEARCH_H_
#define SEARCH_H_

#include "Board.h"

class Search
{
	private:
		Board& board;

		U64 nodes;

		S32 maxi(U8 depth);
		S32 mini(U8 depth);
		U32 minimax(U8 depth);
	public:
		Search(Board& board) : board(board) { }

		U8 getBestMove(U8 depth);
};

#endif /* SEARCH_H_ */
