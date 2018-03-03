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
		// Move move = search.getBestMove((U8) (moveNum  / 1000) + 6 + (board.getEmptySquares().size < 4));
		Move move = search.getBestMove(8);
		std::cout << moveNum << " : " << getMoveName(move) << std::endl;
		board.move(move);
		board.print();
	}

	std::cout << (board.isWon() ? "WON" : "DEAD") << std::endl;
	const U8 highestTile = board.getHighestTile();
	board.newGame();
	return highestTile;
}

void testEngine(int rounds)
{
	int results[winTile] = {};
	for (U8 x = 0; x < rounds; x++)
	{
		const U8 highestTile = playGame();
		for (U8 tileIndex = 0; tileIndex < winTile; tileIndex++)
		{
			if (highestTile >= (tileIndex + 1))
				results[tileIndex]++;
		}
	}

	std::cout << "\nRESULTS FROM " << rounds << " ROUNDS:\n" << std::endl;
	for (int tileIndex = 0; tileIndex < winTile; tileIndex++)
	{
		std::cout << (1 << (tileIndex + 1)) << ": " << results[tileIndex] << std::endl;
	}
}

int main()
{
	//board.test();
	playGame();
	return 0;
}
