#ifndef RESPONSE_H_
#define RESPONSE_H_

#include "Defs.h"

class Board;

class Response
{
	friend class Board;
	private:
		const U8 response;
		inline Response(U8 tile, U8 index) : response((index << 2) | tile) { }
	public:
		inline U8 getTile() { return response & 3; }
		inline U8 getSquare() { return response >> 2; }
		float getProbability(Board& board);
};

#endif /* RESPONSE_H_ */
