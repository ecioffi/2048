#ifndef BOARD_H_
#define BOARD_H_

#include <time.h>

#include <random>
#include <vector>
#include <memory>

#include "Defs.h"
#include "Response.h"

enum class Move : U8 {Up, Down, Left, Right, NoMove};
enum Sq : U8 {A1, A2, A3, A4, B1, B2, B3, B4, C1, C2, C3, C4, D1, D2, D3, D4, NO_SQ};

static const constexpr Move allMoves[4] = {Move::Up, Move::Down, Move::Left, Move::Right};


#define useStaleSeed__

#ifdef useStaleSeed__
#define seed__ 33
#else
#define seed__ time(NULL)
#endif

class Board
{
	friend class Search;
	friend class Response;

	private:
		std::mt19937 engine;
		std::uniform_int_distribution<U8> tileDistribution;
		std::uniform_int_distribution<U8> distribution;

		static const constexpr U8 newTile[10] = {1, 1, 1, 1, 1, 1, 1, 2, 1, 1};
		static const constexpr char* printString[12] = {"    ", " 2  ", " 4  ", " 8  ", " 16 ", " 32 ", " 64 ", " 128", "256 ", "512 ", "1024", "2048"};

		std::vector<std::vector<U8>>* history_ = new std::vector<std::vector<U8>>;
		std::vector<std::vector<U8>>& history = *history_;
		U8* board;

		std::vector<U8> getEmptySquares();
		U8 getHighestTile();

		inline bool isEmpty(U8 sq) { return (board[sq] == 0); }
		inline void moveTile(U8 sSq, U8 tSq) { board[tSq] = board[sSq]; board[sSq] = 0; }
		inline void moveIfAble(U8 sSq, U8 tSq) { if (isEmpty(tSq)) moveTile(sSq, tSq); }
		inline bool isMergeable(U8 sSq, U8 tSq) { return ((board[sSq] == board[tSq]) && board[sSq] != 0); }
		inline void mergeTiles(U8 sSq, U8 tSq) { board[tSq]++; board[sSq] = 0; }
		inline void mergeIfAbleAA(U8 sSq, U8 tSq, U8& sq, S8 num) { if (isMergeable(sSq, tSq)) { mergeTiles(sSq, tSq); sq += num; } }

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		void doMove(Move move);
		inline void updateBoardPointer() { board = &history.back()[0]; }

		inline U8* getPreviousBoard() { return &(*(history.rbegin() + 1))[0]; }
		inline bool isEqual(U8* boardA, U8* boardB) { return (((U64*) boardA)[0] == ((U64*) boardB)[0]) && (((U64*) boardA)[1] == ((U64*) boardB)[1]); }

		inline U8 getNewTile() { return newTile[tileDistribution(engine)]; }
		void respond();

		bool isFull();
		bool areNoMerges();
	public:
		Board() : engine(std::mt19937(seed__)), tileDistribution(std::uniform_int_distribution<U8>(0_U8, 9_U8)) { history.push_back(std::vector<U8>(16)); updateBoardPointer(); respond(); respond(); }
		~Board() { delete history_; }

		static inline constexpr U8 getSqIndex(U8 row, U8 col) { return (col * 4) + row; }

		void move(Move move);
		inline bool wasPreviousMoveIllegal() { return isEqual(getPreviousBoard(), board); }

		inline void unMove() { history.pop_back(); updateBoardPointer(); }

		inline S32 evaluate() { return getEmptySquares().size() * 128 + getHighestTile(); }
		inline bool isDead() { return isFull() && areNoMerges(); }
		inline bool isWon() { return getHighestTile() >= 12; }
		inline U64 movesMade() { return history.size() - 1; }

		void unRespond(Response response);
		void respond(Response response);
		void moveNR(Move move);
		std::vector<Response> getAllResponses();

		void print();
};

#endif /* BOARD_H_ */
