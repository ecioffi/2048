#include "Response.h"
#include "Board.h"

float tileProbability[3] = {0.0, 0.9, 0.1};

float Response::getProbability(Board& board)
{
	return tileProbability[getTile()] / board.getEmptySquares().size();
}
