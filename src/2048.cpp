#include "Board.h"
#include "Search.h"

#include <iostream>
#include <array>

Board board;
Search search(board);

inline std::string getMoveString(Move& move) // FIXME
{
	std::array<std::string, 5> t = {"Move::Right", "Move::Left", "Move::Up", "Move::Down", "Move::NoMove"};
	return t.at((U8) move);
}

bool play()
{
	if (board.isDead() || board.isWon())
		board.newGame();

	U64 num = 0;
	board.print();
	while (!(board.isDead() || board.isWon()))
	{
		num++;
		Move move = search.getBestMove((U8) (num  / 1000) + 6 + (board.getEmptySquares().size() < 4));
		std::cout << num << " : " << getMoveString(move) << std::endl;
		board.move(move);
		board.print();
	}

	std::cout << (board.isWon() ? "WON" : "DEAD") << std::endl;

	return board.isWon();
}

void testEngine(U8 rounds)
{
	U8 results[winTile] = {};
	for (U8 x = 0; x < rounds; x++)
	{
		play();
		for (U8 tileIndex = 0; tileIndex < winTile; tileIndex++)
		{
			if (board.getHighestTile() >= (tileIndex + 1))
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
	//	board.test();

	testEngine(50);
}
