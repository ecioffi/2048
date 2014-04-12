#ifndef RESPONSE_H_
#define RESPONSE_H_

#include "Defs.h"

class Response
{
	friend class Board;
	private:
		U8 response;
		Response(U8 tile, U8 index) : response((index << 2) | tile) { }
	public:
		inline U8 getTile() { return response & 3; }
		inline U8 getIndex() { return response >> 2; }
};

#endif /* RESPONSE_H_ */
