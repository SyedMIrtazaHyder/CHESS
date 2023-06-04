#include "board.h"
#include "pieces.h"
#include "player.h"
using namespace std;


Player::Player(bool isWhite) : isWhite(isWhite), isChecked(false), AI(false) {
	//The counter keeps track of how many undo's the player can do
	counter = 0;

	//Adding a buffer element so that the stack does not give errors
	movedPieces.push_front(NULL);
	//color only needed for king assignment
	if (isWhite)
	{
		king = new King("e1", "w");
	}
	else
	{
		king = new King("e8", "b");

	}
	pieces.push_back(king);
	//placing pawns
	for (char i = 'a'; i < 'i'; i++)
	{
		//if (isWhite)
			//pieces.push_back(new Pawn(string(1, i) + "2", "w"));
		//else
			//pieces.push_back(new Pawn(string(1, i) + "7", "b"));
	}

	if (isWhite) {
		//pieces.push_back(new Queen("d1", "w"));
		//pieces.push_back(new Bishop("c1", "w"));
		//pieces.push_back(new Bishop("f1", "w"));
		//pieces.push_back(new Knight("g1", "w"));
		//pieces.push_back(new Knight("b1", "w"));
		pieces.push_back(new Rook("h1", "w"));
		pieces.push_back(new Rook("a1", "w"));
	}

	else {
		//pieces.push_back(new Queen("d8", "b"));
		//pieces.push_back(new Bishop("c8", "b"));
		//pieces.push_back(new Bishop("f8", "b"));
		//pieces.push_back(new Knight("g8", "b"));
		//pieces.push_back(new Knight("b8", "b"));
		pieces.push_back(new Rook("a8", "b"));
		pieces.push_back(new Rook("h8", "b"));
	}

}
void Player::setAI(bool val) {
	AI = val;
}
bool Player::isAI() {
	return AI;
}
void Player::setCheck(bool check) {
	isChecked = check;
}
bool Player::Checked() {
	return isChecked;
}
King* Player::getKing() {
	return king;
}
list<Pieces*>& Player::getPieces() {
	return pieces;
}
list<Pieces*>& Player::getMovedPieces()
{
	return movedPieces;
}
void Player::pushPiece(Pieces* pushed) {
	if (counter < 3)
	{
		counter++;
	}
	movedPieces.push_front(pushed);
}
void Player::undo() {
	if (movedPieces.front() == NULL && !movedPieces.empty())
	{
		movedPieces.pop_front();
		//>>>>problem when returning the latest of the captured pieces<<<,
		//movedPieces.pop();//popping twice to retrive the captured piece as well
	}

	else if (movedPieces.size() > 1 && counter > 0)
	{
		movedPieces.front()->undoMove();

		list<Pieces*>::iterator i = find(pieces.begin(), pieces.end(), movedPieces.front());
		if (i == pieces.end())
			pieces.push_back(movedPieces.front());
		
		movedPieces.pop_front();

		counter--;
	}

	while (!Board::movedPieces.empty())
		Board::movedPieces.pop();
}
void Player::Captured(Pieces* piece)
{
	piece->prevX.push_front(piece->x);
	piece->prevY.push_front(piece->y);

	this->pushPiece(piece);
	

	list<Pieces*>::iterator i = find(pieces.begin(),pieces.end(),piece);
	if (i != pieces.end())
		pieces.erase(i);

}
map<Pieces*, set<string>> Player::LegalMovesInCheck() {//>>>>Avoding checkmate logic here, user only allowed to make these moves
	//atm just generating the kingMoves
	map<Pieces*, set<string>> movesInCheck;
	kingToMove = king->name;
	king->pseudoLegalMoves();
	if (!king->possibleMoves.empty())
		movesInCheck[king] = king->possibleMoves;//for avoiding general checks
	list<string> danger = king->checkedList();//getting positions where piece is checked from
	int* attackerPos = Board::decodePosition(*danger.begin());
	Pieces* attackerPiece = Board::board[attackerPos[1]][attackerPos[0]];
	if (danger.size() > 1)//for double check, can only move king
		return movesInCheck;

	/*for (string kingMove : king->possibleMoves)values already in unordered set
		danger.push_back(kingMove);*/

		//adding empty spaces where we can block forseen check
	if (attackerPos[0] == king->x)// so we know that opposing piece has vertical or horizontal ray
	{
		int mul = 1;
		if (attackerPos[1] > king->y)
			mul *= -1;
		for (int i = attackerPos[1] + mul; i != king->y; i += mul)
			danger.push_back(Board::toMove(king->x, i));
	}

	else if (attackerPos[1] == king->y) {
		int mul = 1;
		if (attackerPos[0] > king->x)
			mul *= -1;
		for (int i = attackerPos[0] + mul; i != king->x; i += mul)
			danger.push_back(Board::toMove(i, king->y));
	}

	else if (attackerPiece->name[1] != 'N' || attackerPiece->name[1] != 'P')//Hence if both of these eliminated then only diagonal check possible
	{
		int mulEW = 1, mulNS = 1;
		if (attackerPos[0] > king->x)
			mulEW *= -1;
		if (attackerPos[1] > king->y)
			mulNS *= -1;

		int i = attackerPos[0] + mulEW, j = attackerPos[1] + mulNS;
		for (; king->x != i && king->y != j; i += mulEW, j += mulNS) {
			danger.push_back(Board::toMove(i, j));
		}
	}

	//generating all possible moves for all pieces and comparing it with moves in danger
	for (Pieces* p : pieces) {
		if (p == NULL)
			continue;
		if (p->name[1] == 'K')
			continue;
		p->pseudoLegalMoves();
		set<string> blockingMoves;
		for (string threatSpace : danger) {
			if (p->possibleMoves.find(threatSpace) != p->possibleMoves.end())
				blockingMoves.insert(threatSpace);
		}
		if (!blockingMoves.empty())
			movesInCheck[p] = blockingMoves;
	}

	return movesInCheck;
	//call function to get captured Pieces in each case
	//capturing the checking piece
	//if (attackerPiece->name[1] == 'N' || attackerPiece->name[1] == 'P')//checked by knight, pawn checked cannot be blocked so need to capture or move king
	//blocking check and not possible if checked by knight, pawn checked cannot be blocked, and king cannot give check



}
bool Player::Promotion(Pieces* piece)
{
	if (piece->name[1] == 'P' && (piece->y == 0 || piece->y == 7))
	{
		pieces.remove(piece);
		char choice = ' ';
		string piecePosition = "Z0";
		piecePosition[0] = piece->x + 'a';
		piecePosition[1] = piece->y + '1';

		Captured(piece);

		if (isAI()) {
			pieces.push_back(new Queen(piecePosition, string(1, piece->name[0])));
			counter = -1;
			return true;
		}

		cout << "\n\n";
		cout << "What would you like to promote your pawn into?\n";
		cout << "N for Knight - Q for Queen - B for Bishop - R for Rook\n" << "choice: ";
		while (choice != 'N' && choice != 'Q' && choice != 'B' && choice != 'R')
			cin >> choice;

		switch (choice)
		{
		case 'N':
			pieces.push_back(new Knight(piecePosition, string(1, piece->name[0])));
			break;
		case 'Q':
			pieces.push_back(new Queen(piecePosition, string(1, piece->name[0])));
			break;
		case 'B':
			pieces.push_back(new Bishop(piecePosition, string(1, piece->name[0])));
			break;
		case 'R':
			pieces.push_back(new Knight(piecePosition, string(1, piece->name[0])));
			break;
		}

		//Cannot undo a Promotion
		counter = -1;

		return true;
	}

	return false;
}

//forwarded functions
void King::setOpponent(Player& opp) {
	opponent = &opp;
}

void King::legalMoves() {//>>>Can be made more efficient by only going through the king moves, check isin function or smthn<<<
	//possibleMoves = pseudoLegalMoves(toMove(this->x, this->y));
	Board::board[this->y][this->x] = NULL;//so the king is no longer in the "ray" of the attacking piece
	for (Pieces* piece : opponent->getPieces()) {//Error here ://////
		//PAWN HAS DIFFERENT ATTACK PATTERN
		if (piece->name[1] == 'P') {
			piece->resetMoves();
			if (piece->isWhite()) {
				piece->possibleMoves.insert(Board::toMove(piece->x + 1, piece->y + 1));//can generate illegal moves as we do not care for these moves
				piece->possibleMoves.insert(Board::toMove(piece->x - 1, piece->y + 1));
			}

			else {//black Pawn
				piece->possibleMoves.insert(Board::toMove(piece->x + 1, piece->y - 1));//can generate illegal moves as we do not care for these moves
				piece->possibleMoves.insert(Board::toMove(piece->x - 1, piece->y - 1));
			}
		}
		else
			piece->pseudoLegalMoves();//setting the pieces pseudoLegalMoves unordered set
		for (string move : piece->possibleMoves)
			if (possibleMoves.find(move) != possibleMoves.end())
				possibleMoves.erase(possibleMoves.find(move));
	}
	Board::board[this->y][this->x] = this;
}
//other functions
bool PlayerTurn(Player& A, Player& B, bool isWhite, int& WinorLose)
{
	//Player A is the current player
	//need info on the opponent king so we can prevent their move generation
	kingToMove = A.getKing()->name;
	string p1, p2;
	bool CheckRollback = false;
	Board::displayBoard();
	map<Pieces*, set<string>> movesInCheck;
	Pieces* pieceSelected = NULL;
	
	cout << A.getPieces().size() << " " << B.getPieces().size() << endl;
	if (A.getPieces().size() == 1 && B.getPieces().size() == 1) {
		WinorLose = 2;
		return 1;
	}

	if (A.getKing()->isChecked()) {
		movesInCheck = A.LegalMovesInCheck();
		if (movesInCheck.empty())
		{
			cout << "CheckMate...\n";
			if (isWhite)
				WinorLose = -1;
			else
				WinorLose = 1;
			return 1;
		}

		//add player losing logic here
		else {
			A.setCheck(true);
			cout << "Check!!!\n";
			/*for (auto i = movesInCheck.begin(); i != movesInCheck.end(); i++) {
				cout << "Piece: " << i->first->name << endl << "Moves: ";
				for (auto j = i->second.begin(); j != i->second.end(); j++)
					cout << *j << " ";
				cout << endl;
			}*/
		}
	}


	if (A.isAI()) {
		cout << "AI making move...\n";
		system("pause");
		AITurn(A, B, movesInCheck, WinorLose);
		return 0;
	}

	cout << "What would you like to do?\n" << "1.Make a move" << endl << "2.Undo a Move" << endl << "3.Resign" << endl << endl << "Your choice: ";

	do {
		cin >> p1;
	} while (p1 != "1" && p1 != "2" && p1 != "3");

	if (p1 == "3")
	{
		if (isWhite)
			WinorLose = -1;
		else
			WinorLose = 1;
		return 1;
	}

	if (p1 == "2")
	{
		system("CLS");
		if (A.getMovedPieces().size() > 2 && B.getMovedPieces().size() > 2)
		{
			//Undo because of Player
			B.undo();
			A.undo();


			//Undo for the Opponent
			A.undo();
			B.undo();

			A.setCheck(false);
		}

		else
			cout << "No more Undo's Possible" << endl << endl;

		return 1;
	}

	//Loop for the player
	while (true)
	{
		//User's turn
		do
		{
			cout << "Enter current position of piece: ";
			cin >> p1;

			if (Board::validPosition(p1) && A.Checked() && !movesInCheck.empty()) {
				int* boardPos = Board::decodePosition(p1);
				if (movesInCheck.find(Board::board[boardPos[1]][boardPos[0]]) == movesInCheck.end())//only move valid piece in check
				{
					cout << "Invalid Move, you are currently in check" << endl;
					p1 = "999";
				}
			}
		} while (!Board::validPosition(p1));

		//This checks if the piece chosen is actually the player's
		if (!Board::isvalid(isWhite, p1))
			continue;

		int* selectedPiecePos = Board::decodePosition(p1);
		pieceSelected = Board::board[selectedPiecePos[1]][selectedPiecePos[0]];


		cout << "Enter move: ";
		cin >> p2;
		if (Board::validPosition(p2) && A.Checked() && !movesInCheck.empty()) {
			int* boardPos = Board::decodePosition(p2);
			if (movesInCheck[pieceSelected].find(p2) == movesInCheck[pieceSelected].end())//only play valid move in check
			{
				cout << "Invalid Move, you are currently in check" << endl;
				continue;
			}
		}


		if (Board::board[p1[1] - '1'][p1[0] - 'a']->validMoves(p2))//, Board::board[p1[1] - '1'][p1[0] - 'a']->name))
		{

			//Pushing previous Piece and moving it
			A.pushPiece(Board::board[p1[1] - '1'][p1[0] - 'a']);

			//Moving the piece on the Board::board
			//Can't undo a castle
			if (castling != 0)
			{
				A.counter = -1;
				B.counter = -2;
			}

			Pieces* captured = Board::board[p1[1] - '1'][p1[0] - 'a']->move(p2);
			//Pushing the enemy piece onto the Undo Stack
			if (captured != NULL)
				B.Captured(captured);
			//Adding in NULL to make sure Undo's work properly
			else
				B.pushPiece(NULL);

			if (ThreefoldRepition(Board::board[p2[1] - '1'][p2[0] - 'a'], p2)) {
				WinorLose = -2;//-2 for draw
			}
			//Promotion in case it happens
			if (A.Promotion(Board::board[p2[1] - '1'][p2[0] - 'a']))
				B.counter = -1;
			A.setCheck(false);
		}
		else
		{
			cout << "Invalid Move" << endl;
			continue;
		}
		system("pause");
		system("CLS");
		break;
	}

	return 0;
}
void AITurn(Player& AI, Player &B, map<Pieces*, set<string>>& movesInCheck, int& WinorLose)
{
	list<Pieces*> allPieces = AI.getPieces();
	Pieces* pieceToMove = NULL;
	Moves MoveToPlay;
	multimap<Moves, Pieces*, greater<Moves>> movepool;

	if (AI.Checked()) {
		for (auto i : movesInCheck) {
			Moves temp = i.first->BM();
			if (i.second.find(temp.move) != i.second.end())
				movepool.insert({ temp, i.first });
			else {
				Moves temp2(*(i.second.begin()), 0);
				movepool.insert({ temp2, i.first });
			}
		}
	}

	else {
		for (Pieces* piece : allPieces) {
			Moves pieceMove = piece->BM();
			if (MoveToPlay <= pieceMove && pieceMove.value != -1)
			{
				movepool.insert(pair<Moves, Pieces*>(pieceMove, piece));
				MoveToPlay = pieceMove;
			}
		}
	}

	if (movepool.empty()) {
		if (AI.Checked()) {
			if (AI.getKing()->isWhite())
				WinorLose = -1;
			else
				WinorLose = 1;
		}
		else
			WinorLose = -2;
		return;
	}
	AI.setCheck(false);
	srand(unsigned(time(NULL)));

	int pos = 1;
	multimap<Moves, Pieces*>::iterator prev = movepool.begin();
	for (multimap<Moves, Pieces*>::iterator i = ++movepool.begin(); i != movepool.end(); i++, pos++) {
		if (prev->first.value - i->first.value < 10)
			prev++;
		else
			break;
	}

	pos = rand() % pos;


	multimap<Moves, Pieces*>::iterator it = movepool.begin();
	advance(it, pos);
	MoveToPlay = (*it).first;
	pieceToMove = (*it).second;
	//Added this line to test the condition for repetition stalemate
	AI.pushPiece(pieceToMove);

	int* decodedPos = Board::decodePosition(MoveToPlay.move);

	//no Undoing a castle.
	if (castling != 0)
	{
		AI.counter = -1;
		B.counter = -2;
	}
	Pieces* captured = pieceToMove->move(MoveToPlay.move);

	if (captured != NULL)
		B.Captured(captured);
	else
		B.pushPiece(NULL);


	if (AI.Promotion(Board::board[decodedPos[1]][decodedPos[0]]))
		B.counter = -1;
	if (ThreefoldRepition(Board::board[decodedPos[1]][decodedPos[0]], MoveToPlay.move)) {
		WinorLose = -2;//-2 for draw
	}
}
void vsPlayerGame(bool AisAI, bool BisAI)
{
	system("CLS");
	bool isWhite = 1;// rand() % 2;
	bool CheckRollback = false;
	bool skipPlayer = false;
	int WinorLose = 0;
	char choice;
	//setting counter for Draw
	int drawCounter = 0;
	//setting new players
	Player A(isWhite);
	Player B(!isWhite);

	if (AisAI)
		A.setAI(true);
	if (BisAI)
		B.setAI(true);

	//>>>>Setting player's kings with the data they require<<<<
	A.getKing()->setOpponent(B);
	B.getKing()->setOpponent(A);//king needs opponent pieces potential moves
	//>>><<<<
	if (isWhite)
		cout << "Player 1 will Control White and Player 2 will Control Black." << endl;
	else
		cout << "Player 1 will Control Black and Player 2 will Control White." << endl;


	//Main gameplay loop is here
	do {
		if (AisAI && BisAI)
		{
			cout << "Would you like to quit the match?\nPress 1 to quit";
			choice = _getch();
			if (choice == '1')
			{
				//Random value here
				WinorLose = 9;
				break;
			}
			cout << endl;
		}
		if (!skipPlayer)
		{
			cout << "Player 1 Turn: " << endl;
			skipPlayer = PlayerTurn(A, B, isWhite, WinorLose);
		}
		else
			skipPlayer = false;

		if (!skipPlayer)
		{
			cout << "Player 2 Turn: " << endl;
			skipPlayer = PlayerTurn(B, A, !isWhite, WinorLose);
		}
		else
			skipPlayer = false;
	} while (WinorLose == 0);
	// WinorLose: 1 = white wins, -1 = black wins, 2 = draw by repitition, -2 = stalemate
	if (WinorLose == 1)
		cout << endl << "The White Player has won the game!";
	else if (WinorLose == -1)
		cout << endl << "The Black Player has won the game!";
	else if (WinorLose == 2)
		cout << endl << "Stalemate";
	else if (WinorLose == -2)
		cout << endl << "Draw by repitition!";

	_getch();
}

bool ThreefoldRepition(Pieces* p, string move) {
	static float drawCounter = 0;
	if (Board::movedPieces.size() < 4)
		Board::movedPieces.push(p);
	else {
		if (p == Board::movedPieces.front()) {
			int currentX = p->prevX.front(), currentY = p->prevY.front();
			pair<list<int>::iterator, list<int>::iterator> i = { p->prevX.begin(), p->prevY.begin() };//getting 2nd last move from current move
			advance(i.first, 2);
			advance(i.second, 2);
			if (currentX == *(i.first) && currentY == *(i.second))
				drawCounter += 0.25f;
			else
				drawCounter = 0;
		}
		else
			drawCounter = 0;
		Board::movedPieces.pop();
		Board::movedPieces.push(p);
	}

	if (drawCounter > 1.75)
		return true;
	return false;
}