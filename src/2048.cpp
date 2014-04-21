#include "Board.h"
#include "Search.h"

#include <iostream>

Board board;
Search search(board);

int main()
{
	U64 num = 0;
	board.print();
	while (!(board.isDead() || board.isWon()))
	{
		num++;
		Move move = search.getBestMove(6);
		std::cout << num << std::endl;
		board.move(move);
		board.print();
	}

	std::cout << (board.isWon() ? "WON" : "DEAD") << std::endl;
}
