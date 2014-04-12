#ifndef BOARD_H_
#define BOARD_H_

#include <time.h>

#include <random>
#include <vector>

#include "Defs.h"

enum class Move : U8 {Up, Down, Left, Right};
enum Sq : U8 {A1, A2, A3, A4, B1, B2, B3, B4, C1, C2, C3, C4, D1, D2, D3, D4, NO_SQ};
//enum class Actor : U8 {Player, Response, NoActor};

static const constexpr Move allMoves[4] = {Move::Up, Move::Down, Move::Left, Move::Right};
//static const constexpr Actor allActors[2] = {Actor::Player, Actor::Response};


#define useStaleSeed__

#ifdef useStaleSeed__
#define seed__ 33
#else
#define seed__ time(NULL)
#endif

class Board
{
	friend class Search;
	private:
		std::mt19937 engine;
		std::uniform_int_distribution<U8> tileDistribution;
		std::uniform_int_distribution<U8> distribution;

		static const constexpr U8 newTile[10] = {1, 1, 1, 1, 1, 1, 1, 2, 1, 1};
		static const constexpr char* printString[12] = {"    ", " 2  ", " 4  ", " 8  ", " 16 ", " 32 ", " 64 ", " 128", "256 ", "512 ", "1024", "2048"};

//		std::vector<std::vector<U8>> history = {std::vector<U8>(16)};
//		U8* board = &history.back()[0];
//		Actor actorTM = Actor::Player;
		U8 board[16] = {};

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

		void doMove(Move move);
//		inline void updateBoardPointer() { board = &history.back()[0]; }
//		inline void flipActorTM() { actorTM = (Actor) ((U8) actorTM ^ 1_U8);  }
//		inline U8* getPreviousBoard() { return &(*(history.rbegin() + 1))[0]; }
		inline bool isEqual(U8* boardA, U8* boardB) { return (((U64*) boardA)[0] == ((U64*) boardB)[0]) && (((U64*) boardA)[1] == ((U64*) boardB)[1]); }

		inline U8 getNewTile() { return newTile[tileDistribution(engine)]; }
		void respond();

		bool isFullBoard();
		bool areNoMerges();
	public:
		Board() : engine(std::mt19937(seed__)), tileDistribution(std::uniform_int_distribution<U8>(0_U8, 9_U8)) { respond(); respond(); }

		static inline constexpr U8 getSqIndex(U8 row, U8 col) { return (col * 4) + row; }

		void move(Move move);
//		inline bool isPreviousMoveIllegal() { return isEqual(getPreviousBoard(), board); }

//		inline void unMove() { history.pop_back(); updateBoardPointer(); flipActorTM(); }

		inline S16 evaluate() { return getEmptySquares().size() + getHighestTile(); }
		inline bool isDead() { return isFullBoard() && areNoMerges(); }
		inline bool isWon() { return getHighestTile() >= 12; }
//		inline Actor getActorTM() { return actorTM; }
//		inline U64 movesMade() { return history.size() - 1; }

		void print();
};

#endif /* BOARD_H_ */
