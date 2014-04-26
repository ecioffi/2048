#include "Board.h"
#include "Search.h"

#include <iostream>

inline std::string getMoveString(Move& move)
{
	std::string t[5] = {"Move::Left", "Move::Right", "Move::Up", "Move::Down", "Move::NoMove"};
	return t[(U8) move];
}

int main()
{
	Board board;
	Search search(board);

	board.test();

//	U64 num = 0;
//	board.print();
//	while (!(board.isDead() || board.isWon()))
//	{
//		num++;
//		Move move = search.getBestMove(6);
//		std::cout << num << " : " << getMoveString(move) << std::endl;
//		board.move(move);
//		board.print();
//	}
//
//	std::cout << (board.isWon() ? "WON" : "DEAD") << std::endl;
}
