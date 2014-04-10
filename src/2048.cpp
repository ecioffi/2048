#include "Board.h"
#include "Search.h"

#include <iostream>

Board board;
Search search(board);

int main()
{
	board.print();
	while (!(board.isDead() || board.isWon()))
	{
		board.move(search.getBestMove(10));
		board.print();
	}

	std::cout << (board.isWon() ? "WON" : "DEAD") << std::endl;
}
