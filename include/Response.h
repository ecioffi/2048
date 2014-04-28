#ifndef RESPONSE_H_
#define RESPONSE_H_

#include "Defs.h"

class Response
{
	private:
		U8 response;
		float probability_;
	public:
		inline Response(U8 tile, U8 sq, float probability) : response((sq << 2) | tile), probability_(probability) { }
		inline Response() { }

		inline float probability() { return probability_; }
		inline U8 getTile() { return response & 3; }
		inline U8 getSquare() { return response >> 2; }
};

struct ResponseArray {
		U8 size = 0;
		std::array<Response, 32> array;

		inline void pushBack(Response response) { array[size] = response; size++; }

		inline Response& operator[](U8 index) { return array[index]; }
		inline const Response& operator[](U8 index) const { return array[index]; }
};

#endif /* RESPONSE_H_ */
