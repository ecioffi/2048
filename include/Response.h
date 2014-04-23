#ifndef RESPONSE_H_
#define RESPONSE_H_

#include "Defs.h"

class Response
{
	friend class Board;
	private:
		const U8 response;
		inline Response(U8 tile, U8 index, float probability) : response((index << 2) | tile), probability(probability) { }
	public:
		inline U8 getTile() { return response & 3; }
		inline U8 getSquare() { return response >> 2; }
		const float probability;
};

#endif /* RESPONSE_H_ */
