#pragma once
#ifndef PIECES_H
#define PIECES_H

#include <list>
#include <stack>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include <queue>
#include <variant>

struct Moves {
	int value;
	std::string move;

	Moves(std::string, int);

	Moves();

	bool operator < (const Moves&) const;

	bool operator <= (const Moves&) const;

	bool operator > (const Moves&) const;
};

class Player;

class Pieces{
public:
	int x, y, value;
	std::string name;
	std::list<int> prevX;
	std::list<int> prevY;
	std::set<std::string> possibleMoves;//listing all the possible moves possible by a certain piece
	std::set<std::string> captureMoves;//lists all the possible pieces current piece can capture
	std::priority_queue<Moves> bestMoves;

	Pieces();
	Pieces(std::string pos, std::string name, int value);
	bool isWhite();
	bool isOpponent(Pieces& Opp);
	void resetMoves();
	bool validMoves(std::string pos);
	Pieces* move(std::string pos);
	void undoMove();
	bool isProtected();
	bool verticalPin();
	bool horizontalPin();
	bool rightDiagonalPin();
	bool leftDiagonalPin();
	bool operator == (Pieces&);
	virtual std::set<std::string> pseudoLegalMoves() = 0;
	virtual Moves BM() = 0; //BestMove
};

class Pawn :public Pieces {
public:
	Pawn(std::string pos, std::string color);
	std::set<std::string> pseudoLegalMoves();
	Moves BM();
};

class Knight :public Pieces {
public:
	Knight(std::string pos, std::string color);
	std::set<std::string> pseudoLegalMoves();
	Moves BM();
};

class Bishop :public Pieces {
public:
	Bishop(std::string pos, std::string color);
	int rays(bool isNorth, bool isEast);
	std::set<std::string> pseudoLegalMoves();
	Moves BM();
};

class Rook :public Pieces {
public:
	Rook(std::string pos, std::string color);
	int lasers(bool isVertical, bool increment);
	std::set<std::string> pseudoLegalMoves();
	Moves BM();
};

class Queen :public Pieces {
public:
	Queen(std::string pos, std::string color);
	std::set<std::string> pseudoLegalMoves();
	Moves BM();
};

//legalMoves and setOpponent undefined
class King :public Pieces {
	Player* opponent;
	//vector<string> castlingMoves(2, "0");
public:
	std::list<std::string> checks;
	King(std::string pos, std::string color);
	void setOpponent(Player& opp);
	bool isChecked();
	std::list<std::string> checkedList();
	std::set<std::string> pseudoLegalMoves();
	void legalMoves();
	Moves BM();
};

extern Pieces* getEnPassant;
extern Pieces* checkEnPassant;
extern std::string kingToMove;//0 = white, 1 = black
extern int castling;//-1 for queen Side, 1 for king side, 0 for not possible
extern std::string castlingMoves[4];
#endif // !PIECES_H
