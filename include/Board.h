#ifndef BOARD_H_
#define BOARD_H_

#include <random>
#include <array>
#include <string>

#include "Defs.h"
#include "Response.h"

#define useStaleSeed__

#ifdef useStaleSeed__
	static std::mt19937 engine(33);
#else
	static std::random_device randomDevice();
	static std::mt19937 engine(randomDevice());
#endif

enum class Move : U8 {Right, Left, Up, Down, NoMove};
enum Sq : U8 {A1, A2, A3, A4, B1, B2, B3, B4, C1, C2, C3, C4, D1, D2, D3, D4, NO_SQ};

constexpr Move allMoves[4] = {Move::Right, Move::Left, Move::Up, Move::Down};
constexpr const char* moveName[5] = {"Move::Right", "Move::Left", "Move::Up", "Move::Down", "Move::NoMove"};
inline std::string getMoveName(Move move) { return std::string(moveName[(U8) move]); }

constexpr U8 maxDepth = 12;
constexpr U8 winTile = 13;

struct SquareArray {
		U8 size = 0;
		std::array<U8, 16> array;

		inline void pushBack(U8 a) { array[size] = a; size++; }

		inline U8& operator[](U8 index) { return array[index]; }
		inline const U8& operator[](U8 index) const { return array[index]; }
};

struct MoveResult {
		U8 row;
		U8 mergeBits;
};

class Board
{
	friend class Search;
	private:
		std::uniform_int_distribution<U8> tileDistribution;

		static constexpr const char* printString[14] = {"    ", " 2  ", " 4  ", " 8  ", " 16 ", " 32 ", " 64 ", " 128", "256 ", "512 ", "1024", "2048", "4096", "8192"};

		U8 depth = 0;

		const std::array<std::array<MoveResult, 256>, 2> moveResultsLR;
		std::array<std::array<U8, 16>, maxDepth + 1> history = {};
		//const constexpr U8* transposedHistory[16] = getTransposed

		Board& board = *this;
		inline U8& operator[](U8 index) { return history[depth][index]; }
		inline const U8& operator[](U8 index) const { return history[depth][index]; }

//		inline bool isEmpty(U8 sq) { return (board[sq] == 0); }
//		inline void moveTile(U8 sSq, U8 tSq) { board[tSq] = board[sSq]; board[sSq] = 0; }
//		inline void moveIfAble(U8 sSq, U8 tSq) { if (isEmpty(tSq)) moveTile(sSq, tSq); }
		inline bool isMergeable(U8 sSq, U8 tSq) { return ((board[sSq] == board[tSq]) && board[sSq] != 0); }
//		inline void mergeTiles(U8 sSq, U8 tSq) { board[tSq]++; board[sSq] = 0; }
//		inline void mergeIfAbleAA(U8 sSq, U8 tSq, U8& sq, S8 num) { if (isMergeable(sSq, tSq)) { mergeTiles(sSq, tSq); sq += num; } }

		std::array<std::array<MoveResult, 256>, 2> getMoveResultsLR();

		inline static const constexpr U8 getRSqMask(U8 sq) { return 3 << (sq * 2); }
		inline static const constexpr bool isRSqEmpty(U8 row, U8 sq) { return (row & getRSqMask(sq)) == 0; }
		inline static const constexpr bool isRSqFull(U8 row, U8 sq) { return (row & getRSqMask(sq)) != 0; }
		inline static const constexpr U8 getRSqValue(U8 row, U8 sq) { return (row >> (sq * 2)) & 3; }
		inline static void setRSqValue(U8& row, U8 sq, U8 value) { row = (row & ~getRSqMask(sq)) | (value << (sq * 2)); }
		inline static void setRSqValueNC(U8& row, U8 sq, U8 value) { row |= (value << (sq * 2)); }

		U8 getNextFullRSq(U8 row, U8 sq, U8 stop, U8 inc);
		U8 getNextEmptyRSq(U8 row, U8 sq, U8 stop, U8 inc);

		MoveResult moveRight(U8 row);
		MoveResult moveLeft(U8 row);

		void doMove(Move move);

		//inline void updateBoardPointer() { board = &history[depth][0]; }
		inline U8* getPreviousBoard() { return history[depth - 1].data(); }
		inline bool isEqual(U8* boardA, U8* boardB) { return (((U64*) boardA)[0] == ((U64*) boardB)[0]) && (((U64*) boardA)[1] == ((U64*) boardB)[1]); }

//		inline U8 getDepthAI() { return depth++; }
//		inline void moveNR(Move move) { history[depth + 1] = history[getDepthAI()]; updateBoardPointer(); doMove(move); }
//		inline void unMoveNR() { depth--; updateBoardPointer();}
//		inline bool wasPreviousMoveIllegal() { return isEqual(getPreviousBoard(), board); }
		inline void moveNR(Move move) { history[depth + 1] = history[depth]; depth++; doMove(move); }
		inline void unMoveNR() { depth--; }
		inline bool wasPreviousMoveIllegal() { return isEqual(getPreviousBoard(), history[depth].data()); }

		ResponseArray getAllResponses();
		inline void respond(Response response) { board[response.getSquare()] = response.getTile(); }
		inline void unRespond(Response response) { board[response.getSquare()] = 0; }
		inline U8 getNewTile() { return (tileDistribution(engine) == 0) ? 2 : 1; }
		void respond();

		bool isFull();
		bool areNoMerges();
	public:
		Board() : tileDistribution(0, 9), moveResultsLR(getMoveResultsLR()) { respond(); respond(); }

		static inline constexpr U8 getSqIndex(U8 x, U8 y) { return (y * 4) + x; }

		SquareArray getEmptySquares();
		inline U8 getNumberOfEmptySquares() { U8 num = 0; for (U8 sq = 0; sq < 16; sq++) { num += (board[sq] == 0); } return num; }
		U8 getHighestTile();
		inline U32 getHighestTileValue() { return 1 << getHighestTile(); }
		float getAverageTileValue();

		void move(Move move);
		//float evaluate();
		inline U8 evaluate() { return getNumberOfEmptySquares(); }
		inline bool isDead() { return (isFull() && areNoMerges()); }
		inline bool isWon() { return (getHighestTile() == winTile); }

		void newGame();

		void test();
		void print();
};

#endif /* BOARD_H_ */
