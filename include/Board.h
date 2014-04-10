#ifndef BOARD_H_
#define BOARD_H_

#include <time.h>

#include <random>
#include <vector>

#include "Defs.h"

enum MoveType : U8 {UP, DOWN, LEFT, RIGHT, NO_MOVE};
enum Sq : U8 {A1, A2, A3, A4, B1, B2, B3, B4, C1, C2, C3, C4, D1, D2, D3, D4};
enum Player : U8 {PLAYER, RESPONSE, NO_PLAYER};

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
		U8 whoToMove = PLAYER;

		std::vector<U8> getEmptySquares();
		U8 getHighestTile();

		inline bool isEmpty(U8 sq) { return board[sq] == 0; }
		inline void moveTile(U8 sSq, U8 tSq) { board[tSq] = board[sSq]; board[sSq] = 0; }
		inline void moveIfAble(U8 sSq, U8 tSq) { if (isEmpty(tSq)) moveTile(sSq, tSq); }
		inline bool isMergeable(U8 sSq, U8 tSq) { return (board[sSq] == board[tSq]) * board[sSq]; }
		inline void mergeTiles(U8 sSq, U8 tSq) { board[tSq]++; board[sSq] = 0; }
		inline void mergeIfAbleAA(U8 sSq, U8 tSq, U8& sq, S8 num) { if (isMergeable(sSq, tSq)) { mergeTiles(sSq, tSq); sq += num; } }

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		void doMove(U8 moveType);
		inline void updateBoardPointer() { board = &history.back()[0]; }
		inline U8* getPreviousBoard() { return &(*(history.rbegin() + 1))[0]; }
		inline bool isEqual(U8* boardA, U8* boardB) { for (U8 x = 0; x < 2; x++) if (((U64*) boardA)[x] != ((U64*) boardB)[x]) { return false; } return true; }

		inline U8 getNewTile() { return newTile[tileDistribution(engine)]; }
		void putNewTile();

		bool isFullBoard();
		bool areNoMerges();
	public:
		Board() : engine(std::mt19937(seed__)), tileDistribution(std::uniform_int_distribution<U8>(0_U8, 9_U8)) { putNewTile(); putNewTile(); }

		static inline constexpr U8 getSqIndex(U8 row, U8 col) { return (col * 4) + row; }
		static inline constexpr U8 getSqRow(U8 index) { return index % 4; }
		static inline constexpr U8 getSqCol(U8 index) { return index / 4; }

		bool canMove(U8 moveType);
		std::vector<U8> getAllMoves();
		void move(U8 moveType);
		inline void unMove() { history.pop_back(); updateBoardPointer(); }

		inline S16 evaluate() { return getEmptySquares().size() + getHighestTile(); }
		inline bool isDead() { return isFullBoard() && areNoMerges(); }
		inline bool isWon() { return getHighestTile() >= 12; }
		inline U64 movesMade() { return history.size() - 1; }

		void print();
};

#endif /* BOARD_H_ */
