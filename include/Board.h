#ifndef BOARD_H_
#define BOARD_H_

#include <time.h>

#include <random>
#include <vector>
#include <array>

#include "Defs.h"
#include "Response.h"

enum class Move : U8 {Right, Left, Up, Down, NoMove};
enum Sq : U8 {A1, A2, A3, A4, B1, B2, B3, B4, C1, C2, C3, C4, D1, D2, D3, D4, NO_SQ};

static const constexpr Move allMoves[4] = {Move::Right, Move::Left, Move::Up, Move::Down};
static const constexpr U8 maxDepth = 10;
static const constexpr U8 winTile = 11;


#define useStaleSeed__

#ifdef useStaleSeed__
#define seed__ 33
#else
#define seed__ time(NULL)
#endif

struct MoveResult {
		U8 row;
		U8 mergeBits;
};

class Board {
		friend class Search;
		friend class Response;

	private:
		std::mt19937 engine;
		std::uniform_int_distribution<U8> tileDistribution;
		std::uniform_int_distribution<U8> distribution;

		static const constexpr U8 newTile[10] = {1, 1, 1, 1, 1, 1, 1, 2, 1, 1};
		static const constexpr char* printString[12] = {"    ", " 2  ", " 4  ", " 8  ", " 16 ", " 32 ", " 64 ", " 128", "256 ", "512 ", "1024", "2048"};

		U8 depth = 0;

		const std::array<std::array<MoveResult, 256>, 2> moveResultsLR;
		std::array<std::array<U8, 16>, maxDepth + 1> history;
		Board& board = *this;

		inline U8& operator[](U8 index) { return history[depth][index]; }
		inline const U8& operator[](U8 index) const { return history[depth][index]; }

		inline bool isEmpty(U8 sq) { return (board[sq] == 0); }
		inline void moveTile(U8 sSq, U8 tSq) { board[tSq] = board[sSq]; board[sSq] = 0; }
		inline void moveIfAble(U8 sSq, U8 tSq) { if (isEmpty(tSq)) moveTile(sSq, tSq); }
		inline bool isMergeable(U8 sSq, U8 tSq) { return ((board[sSq] == board[tSq]) && board[sSq] != 0); }
		inline void mergeTiles(U8 sSq, U8 tSq) { board[tSq]++; board[sSq] = 0; }
		inline void mergeIfAbleAA(U8 sSq, U8 tSq, U8& sq, S8 num) { if (isMergeable(sSq, tSq)) { mergeTiles(sSq, tSq); sq += num; } }

		std::array<std::array<MoveResult, 256>, 2> getMoveResultsLR();

		inline static const constexpr U8 getRSqMask(U8 sq) { return 3 << (sq * 2); }
		inline static const constexpr bool isRSqEmpty(U8 row, U8 sq) { return (row & getRSqMask(sq)) == 0; }
		inline static const constexpr bool isRSqFull(U8 row, U8 sq) { return (row & getRSqMask(sq)) != 0; }
		inline static const constexpr U8 getRSqValue(U8 row, U8 sq) { return (row & getRSqMask(sq)) >> (sq * 2); }
		inline static void setRSqValue(U8& row, U8 sq, U8 value) { row = (row & ~getRSqMask(sq)) | (value << (sq * 2)); }
		U8 getNextFullRSq(U8 row, U8 sq, U8 stop, U8 inc);
		U8 getNextEmptyRSq(U8 row, U8 sq, U8 stop, U8 inc);

		MoveResult moveRight(U8 row);
		MoveResult moveLeft(U8 row);

		void doMove(Move move);

		//inline void updateBoardPointer() { board = &history[depth][0]; }
		inline U8* getPreviousBoard() { return &history[depth - 1][0]; }
		inline bool isEqual(U8* boardA, U8* boardB) { return (((U64*) boardA)[0] == ((U64*) boardB)[0]) && (((U64*) boardA)[1] == ((U64*) boardB)[1]); }

//		inline U8 getDepthAI() { return depth++; }
//		inline void moveNR(Move move) { history[depth + 1] = history[getDepthAI()]; updateBoardPointer(); doMove(move); }
//		inline void unMoveNR() { depth--; updateBoardPointer();}
//		inline bool wasPreviousMoveIllegal() { return isEqual(getPreviousBoard(), board); }
		inline void moveNR(Move move) { depth++; doMove(move); }
		inline void unMoveNR() { depth--; }
		inline bool wasPreviousMoveIllegal() { return isEqual(getPreviousBoard(), &board[0]); }

		std::vector<Response> getAllResponses();
		inline void respond(Response response) { board[response.getSquare()] = response.getTile(); }
		inline void unRespond(Response response) { board[response.getSquare()] = 0; }
		inline U8 getNewTile() { return newTile[tileDistribution(engine)]; }
		void respond();

		bool isFull();
		bool areNoMerges();
	public:
		Board() : engine(seed__), tileDistribution(0, 9), moveResultsLR(getMoveResultsLR()) { respond(); respond(); }

		static inline constexpr U8 getSqIndex(U8 x, U8 y) { return (y * 4) + x; }

		std::vector<U8> getEmptySquares();
		U8 getHighestTile();
		inline U32 getHighestTileValue() { return 1 << getHighestTile(); }
		float getAverageTileValue();

		void move(Move move);
		float evaluate();
		inline bool isDead() { return (isFull() && areNoMerges()); }
		inline bool isWon() { return (getHighestTile() == winTile); }

		void test();
		void print();
};

#endif /* BOARD_H_ */
