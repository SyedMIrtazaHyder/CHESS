#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <string>
#include <conio.h>
#include <iostream>

class Player {
	bool isWhite, isChecked, AI;
	std::list<Pieces*> pieces;
	//These are the pieces that have been moved/captured
	std::list<Pieces*> movedPieces;
	King* king;
	int counter;
public:
	void setPieces();

	void setAI(bool val);

	bool isAI();

	void setCheck(bool check);

	bool Checked();

	King* getKing();

	Player(bool isWhite);

	std::list<Pieces*>& getPieces();

	std::list<Pieces*>& getMovedPieces();

	void pushPiece(Pieces* pushed);

	void undo();

	void Captured(Pieces* piece);

	std::map<Pieces*, std::set<std::string>> LegalMovesInCheck();

	void Promotion(Pieces* piece);
};

bool PlayerTurn(Player& A, Player& B, bool isWhite, int& WinorLose);
void AITurn(Player& AI, std::map<Pieces*, std::set<std::string>>& movesInCheck, int& WinorLose);
void vsPlayerGame(bool AisAI, bool BisAI);

#endif // !PLAYER_H
