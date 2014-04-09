#ifndef BOARD_H_
#define BOARD_H_

#include <time.h>

#include <random>
#include <vector>

#include "Defs.h"

enum MoveType : U8 {UP, DOWN, LEFT, RIGHT};
enum Sq : U8 {A1, A2, A3, A4, B1, B2, B3, B4, C1, C2, C3, C4, D1, D2, D3, D4};
enum TileComparison : U8 {MAIN_ZERO, SAME_NZ, DIFFERENT_NZ};

 #define useStaleSeed__

#ifdef useStaleSeed__
#define seed__ 33
#else
#define seed__ time(NULL)
#endif

class Board
{
	private:
		std::mt19937 engine;
		std::uniform_int_distribution<U8> tileDistribution;
		std::uniform_int_distribution<U8> distribution;

		static const constexpr U8 newTile[10] = {1, 1, 1, 1, 1, 1, 1, 2, 1, 1};
		static const constexpr char* printString[12] = {"    ", " 2  ", " 4  ", " 8  ", " 16 ", " 32 ", " 64 ", " 128", "256 ", "512 ", "1024", "2048"};

		std::vector<std::vector<U8>> history = {std::vector<U8>(16)};
		U8* board = &history.back()[0];

		std::vector<U8> getEmptySquares();

		inline bool isEmpty(U8 sq) { return board[sq] == 0; }
		inline void moveTile(U8 sSq, U8 tSq) { board[tSq] = board[sSq]; board[sSq] = 0; }
		inline void moveIfAble(U8 sSq, U8 tSq) { if (isEmpty(sSq)) moveTile(sSq, tSq); }
		inline bool isMergeable(U8 sSq, U8 tSq) { return (board[sSq] == board[tSq]) * board[sSq]; }
		inline void mergeTiles(U8 sSq, U8 tSq) { board[tSq]++; board[sSq] = 0; }
		inline void mergeIfAbleAP(U8 sSq, U8 tSq, U8& sq) { if (isMergeable(sSq, tSq)) { mergeTiles(sSq, tSq); sq++; } }
		inline void mergeIfAbleAM(U8 sSq, U8 tSq, U8& sq) { if (isMergeable(sSq, tSq)) { mergeTiles(sSq, tSq); sq--; } }

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		inline U8 getNewTile() { return newTile[tileDistribution(engine)]; }
		void putNewTile();
	public:
		Board() : engine(std::mt19937(seed__)), tileDistribution(std::uniform_int_distribution<U8>(0_U8, 9_U8)) { putNewTile(); putNewTile(); }

		static inline constexpr U8 getSqIndex(U8 row, U8 col) { return (col * 4) + row; }
		static inline constexpr U8 getSqRow(U8 index) { return index % 4; }
		static inline constexpr U8 getSqCol(U8 index) { return index / 4; }

		void move(U8 moveType);
		inline void unMove() { history.pop_back(); }

		void print();

		void test();
};

#endif /* BOARD_H_ */
