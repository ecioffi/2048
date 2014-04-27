#include "Board.h"
#include "Search.h"

#include <iostream>
#include <array>

inline std::string getMoveString(Move& move)
{
	std::array<std::string, 5> t = {"Move::Right", "Move::Left", "Move::Up", "Move::Down", "Move::NoMove"};
	return t.at((U8) move);
}

int main()
{
	Board board;
	Search search(board);

//	board.test();

	U64 num = 0;
	board.print();
	while (!(board.isDead() || board.isWon()))
	{
		num++;
		Move move = search.getBestMove((U8) (num  / 1000) + 6);
		std::cout << num << " : " << getMoveString(move) << std::endl;
		board.move(move);
		board.print();
	}

	std::cout << (board.isWon() ? "WON" : "DEAD") << std::endl;
}
