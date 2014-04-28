#include "Board.h"
#include "Search.h"

#include <iostream>

Board board;
Search search(board);

U8 playGame()
{
	U16 moveNum = 0;
	board.print();
	while (!(board.isDead() || board.isWon()))
	{
		moveNum++;
		Move move = search.getBestMove((U8) (moveNum  / 1000) + 6 + (board.getEmptySquares().size < 4));
		std::cout << moveNum << " : " << getMoveName(move) << std::endl;
		board.move(move);
		board.print();
	}

	std::cout << (board.isWon() ? "WON" : "DEAD") << std::endl;
	const U8 highestTile = board.getHighestTile();
	board.newGame();
	return highestTile;
}

void testEngine(U8 rounds)
{
	U8 results[winTile] = {};
	for (U8 x = 0; x < rounds; x++)
	{
		const U8 highestTile = playGame();
		for (U8 tileIndex = 0; tileIndex < winTile; tileIndex++)
		{
			if (highestTile >= (tileIndex + 1))
				results[tileIndex]++;
		}
	}

	std::cout << "\nRESULTS FROM " << (int) rounds << " ROUNDS:\n" << std::endl;
	for (U8 tileIndex = 0; tileIndex < winTile; tileIndex++)
	{
		std::cout << (int) (1 << (tileIndex + 1)) << ": " << (int) results[tileIndex] << std::endl;
	}
}

int main()
{
	//board.test();

	testEngine(10);
}
