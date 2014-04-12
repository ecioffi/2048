#ifndef MOVE_H_
#define MOVE_H_

#include "Defs.h"

inline U8 Move(U8 moveType)
{
	return moveType;
}

inline U8 Move(U8 tile, U8 index)
{
	return (1 << 8) | (tile << 5) | index;
}

inline U8 getActor(U8 move)
{
	return move >> 8;
}

inline U8 getMoveType(U8 move)
{
	return move;
}

inline U8 getTile(U8 move)
{
	return U8_MAX;
}

#endif /* MOVE_H_ */
