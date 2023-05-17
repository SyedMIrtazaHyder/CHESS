#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>
#include <math.h>
#include <conio.h>
#include <unordered_set>

using namespace std;
class Pieces;
class Player;

int* decodePosition(string pos);
bool validPosition(string Space);
void displayBoard();
//call capture logic when a piece is captured like we did for enpassant
//all directions based on black side
//make capture Moves a priority Queue for AI

bool Enpassant = false;
Pieces* getEnPassant = NULL;
Pieces* checkEnPassant;
vector< vector<Pieces*> > board(8, vector<Pieces*>(8, NULL));
string kingToMove;//0 = white, 1 = black

//Module 1-Pieces
class Pieces {
public:
	int x, y, value;
	string name;
	stack<int> prevX;
	stack<int> prevY;
	unordered_set<string> possibleMoves;//listing all the possible moves possible by a certain piece
	unordered_set<string> captureMoves;//lists all the possible pieces current piece can capture
	Pieces() :x(0), y(0), value(0), name("NULL") {}
	Pieces(string pos, string name, int value) :x(pos[0] - 'a'), y(pos[1] - '1'), value(value), name(name) {
		board[y][x] = this;

		//pushes original position of piece onto stack.
		prevX.push(x);
		prevY.push(y);
	}

	//Used to see if a certain piece can be captured by another Piece
	bool isChecked() {
		string PiecePos = "  ";
		PiecePos[1] = this->y + '1';
		PiecePos[0] = this->x + 'a';

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				//if the pieces have the same colour, it continues to the next piece on the board
				if (board[i][j] == NULL || name[0] == board[i][j]->name[0])
					continue;

				if ((board[i][j])->validMoves(PiecePos))//, (board[i][j])->name))
					return true;
			}

		}

		return false;
	}

	//Possible for extremely small fringe cases to not be checkmated.
	//Testing required.
	bool Checkmate()
	{
		//Three conditions for a checkmate
		//1. The King is Checked
		//2. The King's has no unchecked Valid Spaces
		//3. the Pieces that are checking the King are not Checked

		bool chance = true;

		string PiecePos = "  ";
		PiecePos[1] = this->y + '1';
		PiecePos[0] = this->x + 'a';

		//Getting all valid spaces of the king
		string isvalidSpace = "  ";
		vector<string> validSpaces;
		for (int k = -1; k < 2; k++)
			for (int j = -1; j < 2; j++)
			{
				isvalidSpace[1] = PiecePos[1] + k;
				isvalidSpace[0] = PiecePos[0] + j;

				//checking if space is in bounds and if it can be moved to
				if (validPosition(isvalidSpace) && validMoves(isvalidSpace))//, name))
				{
					validSpaces.push_back(isvalidSpace);
				}
			}


		//Checking if the King is checked
		if (isChecked())
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{

					//if the pieces have the same colour, it continues to the next piece on the board
					if (board[i][j] == NULL || name[0] == board[i][j]->name[0])
						continue;

					//This is to check if there is more than one piece checking the king
					if (board[i][j]->isChecked() && chance && board[i][j]->validMoves(PiecePos))//, board[i][j]->name))
					{
						chance = false;
						continue;
					}


					if (board[i][j]->validMoves(PiecePos))//, board[i][j]->name))
					{
						for (int k = 0; k < validSpaces.size(); k++)
						{
							//If the move overtakes another piece, it needs to be stored to be undone somehow
							//Because at this point player's do not exist, this logic needs to be used
							Pieces* overTaken = board[validSpaces[k][1] - '1'][validSpaces[k][0] - 'a'];
							if (overTaken != NULL)
							{
								overTaken->prevX.push(overTaken->x);
								overTaken->prevY.push(overTaken->y);
							}

							move(validSpaces[k]);

							if (!isChecked())
							{
								undoMove();

								//Same if as below because return statement cuts it off
								if (overTaken != NULL)
									overTaken->undoMove();

								return false;
							}

							//This undo needs to be done first as it nulls its previous positions
							//if the overtaken piece's move was undone first, then this undo would make it NULL again
							undoMove();

							//puts the captured piece back so the board is the same as before.
							if (overTaken != NULL)
								overTaken->undoMove();
						}

						return true;
					}
				}

			}
		}

		return false;
	}

	bool validMoves(string pos) {
		checkEnPassant = getEnPassant;
		pseudoLegalMoves(pos);//EnPassant decided after this line
		cout << "Moves: ";
		for (string move : this->possibleMoves)
			cout << move << " ";

		cout << "\nCaptures: ";
		for (string move : this->captureMoves)
			cout << move << " ";

		cout << endl;
		return find(possibleMoves.begin(), possibleMoves.end(), pos) != possibleMoves.end();
	}

	void move(string pos) {
		//Pushes the new position of the piece onto the stack.
		//Pieces* checkEnPassant = getEnPassant;
		//for (string move : this->possibleMoves)
		//	cout << move << " ";

		if (checkEnPassant == getEnPassant && getEnPassant != NULL)//may have sm bugs but idk not gonna deep test dis
		{
			if ((pos == toMove(getEnPassant->x, getEnPassant->y - 1) && getEnPassant->name[0] == 'w') ||
				(pos == toMove(getEnPassant->x, getEnPassant->y + 1) && getEnPassant->name[0] == 'b'))
				board[getEnPassant->y][getEnPassant->x] = NULL;
			getEnPassant = NULL;

		}

		system("pause");
		prevX.push(x);
		prevY.push(y);


		board[y][x] = NULL;
		x = pos[0] - 'a';
		y = pos[1] - '1';
		board[y][x] = this;

	}

	string toMove(int x, int y) {
		return string(1, (char)('a' + x)) + string(1, (char)('1' + y));
	}

	void undoMove()
	{
		int oldx = prevX.top();
		int oldy = prevY.top();

		board[y][x] = NULL;
		x = oldx;
		y = oldy;
		board[y][x] = this;

		prevX.pop();
		prevY.pop();
	}

	virtual unordered_set<string> pseudoLegalMoves(string pos) = 0;

};

class Pawn :public Pieces {
public:
	Pawn(string pos, string color) :Pieces(pos, color + "P", 10) {}
	//list<string> possibleMoves;//will move this into pieces later as a unordered_set, as two pieces can never have the same start pos but can have same end pos

	unordered_set<string> pseudoLegalMoves(string move) {//generating a rough lookup-table
		possibleMoves.clear();
		//for white
		if (name[0] == 'w') {
			if (board[y + 1][x - 1] != NULL && board[y + 1][x - 1]->name[0] == 'b')//capture left
			{
				possibleMoves.insert(toMove(x - 1, y + 1));
				captureMoves.insert(toMove(x - 1, y + 1));
			}

			if (board[y + 1][x + 1] != NULL && board[y + 1][x + 1]->name[0] == 'b')//capture right
			{
				possibleMoves.insert(toMove(x + 1, y + 1));
				captureMoves.insert(toMove(x + 1, y + 1));
			}

			if (board[y + 2][x] == NULL && this->y == 1)//double push so chance to enpassant
			{
				possibleMoves.insert(toMove(x, y + 2));
				if (toMove(x, y + 2) == move && (
						(board[y + 2][x - 1] != NULL && board[y + 2][x - 1]->name == "bP") ||
						(board[y + 2][x + 1] != NULL && board[y + 2][x + 1]->name == "bP")
						)//checking if pawn on right or left
					)//if this is the move made then update enPassant var
					getEnPassant = this;
			}

			if (board[y + 1][x] == NULL)//single push l8r with promotion
				possibleMoves.insert(toMove(x, y + 1));//check for promotion later

			if (y == 4) {//enPassant baybee
				Pieces* west = board[y][x - 1];
				Pieces* east = board[y][x + 1];

				if (west != NULL && west == getEnPassant)
				{
					possibleMoves.insert(toMove(x - 1, y + 1));
					captureMoves.insert(toMove(x - 1, y));
				}
				if (east != NULL && east == getEnPassant)
				{
					possibleMoves.insert(toMove(x + 1, y + 1));
					captureMoves.insert(toMove(x + 1, y));
				}

			}
		}
		else {
			if (board[y - 1][x - 1] != NULL && board[y + 1][x - 1]->name[0] == 'w')//capture left
			{
				possibleMoves.insert(toMove(x - 1, y - 1));
				captureMoves.insert(toMove(x - 1, y - 1));
			}

			if (board[y - 1][x + 1] != NULL && board[y + 1][x - 1]->name[0] == 'w')//capture right
			{
				possibleMoves.insert(toMove(x + 1, y - 1));
				captureMoves.insert(toMove(x + 1, y - 1));
			}

			if (board[y - 2][x] == NULL && this->y == 6) {//Double push
				possibleMoves.insert(toMove(x, y - 2));
				if (
					toMove(x, y - 2) == move && (
						(board[y - 2][x - 1] != NULL && board[y - 2][x - 1]->name == "wP") ||
						(board[y - 2][x + 1] != NULL && board[y - 2][x + 1]->name == "wP")
						)
					)//checking if pawn can be enpassanted
					getEnPassant = this;
			}

			if (board[y - 1][x] == NULL)//single push
				possibleMoves.insert(toMove(x, y - 1));//promotion logic exists so will make AI most likely only choose to promote queen ???

			if (y == 3) {//lmao get enPassanted 
				Pieces* west = board[y][x - 1];
				Pieces* east = board[y][x + 1];

				if (west != NULL && west == getEnPassant)
				{
					possibleMoves.insert(toMove(x - 1, y - 1));
					captureMoves.insert(toMove(x - 1, y));
				}
				if (east != NULL && east == getEnPassant)
				{
					possibleMoves.insert(toMove(x + 1, y - 1));
					captureMoves.insert(toMove(x + 1, y));
				}

			}
		}
		return possibleMoves;
	}
};

class Knight :public Pieces {
public:
	Knight(string pos, string color) :Pieces(pos, color + "N", 30) {}

	unordered_set<string> pseudoLegalMoves(string pos) {
		possibleMoves.clear();
		//moving like an octopus smh
		//--|
		if (this->x + 2 < 8 && this->y + 1 < 8 && (board[this->y + 1][this->x + 2] == NULL || board[this->y + 1][this->x + 2]->name[0] != this->name[0]))
		{
			possibleMoves.insert(toMove(this->x + 2, this->y + 1));
			if (board[this->y + 1][this->x + 2] != NULL && board[this->y + 1][this->x + 2]->name[0] != this->name[0])
				captureMoves.insert(toMove(this->x + 2, this->y + 1));
		}
		if (this->x + 2 < 8 && this->y - 1 >= 0 && (board[this->y - 1][this->x + 2] == NULL || board[this->y - 1][this->x + 2]->name[0] != this->name[0]))
		{
			possibleMoves.insert(toMove(this->x + 2, this->y - 1));
			if (board[this->y - 1][this->x + 2] != NULL && board[this->y - 1][this->x + 2]->name[0] != this->name[0])
				captureMoves.insert(toMove(this->x + 2, this->y - 1));

		}

		//|--
		if (this->x - 2 >= 0 && this->y + 1 < 8 && (board[this->y + 1][this->x - 2] == NULL || board[this->y + 1][this->x - 2]->name[0] != this->name[0]))
		{
			possibleMoves.insert(toMove(this->x - 2, this->y + 1));
			if (board[this->y + 1][this->x - 2] != NULL && board[this->y + 1][this->x - 2]->name[0] != this->name[0])
				captureMoves.insert(toMove(this->x - 2, this->y + 1));
		}
		if (this->x - 2 >= 0 && this->y - 1 >= 0 && (board[this->y - 1][this->x - 2] == NULL || board[this->y - 1][this->x - 2]->name[0] != this->name[0]))
		{
			possibleMoves.insert(toMove(this->x - 2, this->y - 1));
			if (board[this->y - 1][this->x - 2] != NULL && board[this->y - 1][this->x - 2]->name[0] != this->name[0])
				captureMoves.insert(toMove(this->x - 2, this->y - 1));
		}

		// |
		//-|
		// |
		if (this->x + 1 < 8 && this->y + 2 < 8 && (board[this->y + 2][this->x + 1] == NULL || board[this->y + 2][this->x + 1]->name[0] != this->name[0]))
		{
			possibleMoves.insert(toMove(this->x + 1, this->y + 2));
			if (board[this->y + 2][this->x + 1] != NULL && board[this->y + 2][this->x + 1]->name[0] != this->name[0])
				captureMoves.insert(toMove(this->x + 1, this->y + 2));
		}
		if (this->x + 1 < 8 && this->y - 2 >= 0 && (board[this->y - 2][this->x + 1] == NULL || board[this->y - 2][this->x + 1]->name[0] != this->name[0]))
		{
			possibleMoves.insert(toMove(this->x + 1, this->y - 2));
			if (board[this->y - 2][this->x + 1] != NULL && board[this->y - 2][this->x + 1]->name[0] != this->name[0])
				captureMoves.insert(toMove(this->x + 1, this->y - 2));
		}

		//|
		//|-
		//|
		if (this->x - 1 >= 0 && this->y + 2 < 8 && (board[this->y + 2][this->x - 1] == NULL || board[this->y + 2][this->x - 1]->name[0] != this->name[0]))
		{
			possibleMoves.insert(toMove(this->x - 1, this->y + 2));
			if (board[this->y + 2][this->x - 1] != NULL && board[this->y + 2][this->x - 1]->name[0] != this->name[0])
				captureMoves.insert(toMove(this->x - 1, this->y + 2));
		}
		if (this->x - 1 >= 0 && this->y - 2 >= 0 && (board[this->y - 2][this->x - 1] == NULL || board[this->y - 2][this->x - 1]->name[0] != this->name[0]))
		{
			possibleMoves.insert(toMove(this->x - 1, this->y - 2));
			if (board[this->y - 2][this->x - 1] != NULL && board[this->y - 2][this->x - 1]->name[0] != this->name[0])
				captureMoves.insert(toMove(this->x - 1, this->y - 2));
		}

		return possibleMoves;
	}
};

class Bishop :public Pieces {
public:
	Bishop(string pos, string color) :Pieces(pos, color + "B", 30) {}

	void theBishop() {//remove l8r
		cout << "Imma Bishop";
	}

	unordered_set<string> pseudoLegalMoves(string pos) {
		int i = 0; int j = 0;
		possibleMoves.clear();
		for (i = this->y - 1, j = this->x - 1; i >= 0 && j >= 0; i--, j--) {//sw
			if (board[i][j] != NULL)
			{
				if (board[i][j]->name[0] != this->name[0])
				{
					possibleMoves.insert(toMove(j, i));//adding the possible capture/attacked piece
					captureMoves.insert(toMove(j, i));//adding the possible capture/attacked piece
				}
				break;
			}
			possibleMoves.insert(toMove(j, i));
		}

		for (i = this->y - 1, j = this->x + 1; i >= 0 && j < 8; i--, j++)//se
		{
			if (board[i][j] != NULL)
			{
				if (board[i][j]->name[0] != this->name[0])
				{
					possibleMoves.insert(toMove(j, i));//adding the possible capture/attacked piece
					captureMoves.insert(toMove(j, i));//adding the possible capture/attacked piece
				}
				break;
			}
			possibleMoves.insert(toMove(j, i));

		}

		for (i = this->y + 1, j = this->x - 1; i < 8 && j >= 0; i++, j--)//nw
		{
			if (board[i][j] != NULL)
			{
				if (board[i][j]->name[0] != this->name[0])
				{
					possibleMoves.insert(toMove(j, i));//adding the possible capture/attacked piece
					captureMoves.insert(toMove(j, i));//adding the possible capture/attacked piece
				}
				break;
			}
			possibleMoves.insert(toMove(j, i));

		}

		for (i = this->y + 1, j = this->x + 1; i < 8 && j < 8; i++, j++)//ne
		{
			if (board[i][j] != NULL)
			{
				if (board[i][j]->name[0] != this->name[0])
				{
					possibleMoves.insert(toMove(j, i));//adding the possible capture/attacked piece
					captureMoves.insert(toMove(j, i));//adding the possible capture/attacked piece
				}
				break;
			}
			possibleMoves.insert(toMove(j, i));

		}

		return possibleMoves;
	}
};

class Rook :public Pieces {
public:
	Rook(string pos, string color) :Pieces(pos, color + "R", 50) {}
	//sliding piece so need to just check for obstacles

	void theRook() {//remove l8r
		cout << "Imma Rookzzz";
	}

	unordered_set<string> pseudoLegalMoves(string pos) {
		possibleMoves.clear();
		for (int i = this->y - 1; i >= 0; i--)//north
		{
			if (board[i][this->x] != NULL)
			{
				if (board[i][this->x]->name[0] != this->name[0])
				{
					possibleMoves.insert(toMove(this->x, i));//adding the possible capture/attacked piece
					captureMoves.insert(toMove(this->x, i));//adding the possible capture/attacked piece
				}
				break;
			}
			possibleMoves.insert(toMove(this->x, i));
		}

		for (int i = this->y + 1; i < 8; i++)//south
		{
			if (board[i][this->x] != NULL)//going till it is blocked
			{
				if (board[i][this->x]->name[0] != this->name[0])
				{
					possibleMoves.insert(toMove(this->x, i));//adding the possible capture/attacked piece
					captureMoves.insert(toMove(this->x, i));//adding the possible capture/attacked piece
				}
				break;
			}
			possibleMoves.insert(toMove(this->x, i));
		}


		for (int j = this->x - 1; j >= 0; j--)//west
		{
			if (board[this->y][j] != NULL)
			{
				if (board[this->y][j]->name[0] != this->name[0])
				{
					possibleMoves.insert(toMove(j, this->y));
					captureMoves.insert(toMove(j, this->y));//adding the possible capture/attacked piece
				}
				break;
			}
			possibleMoves.insert(toMove(j, this->y));
		}

		for (int j = this->x + 1; j < 8; j++)//east
		{
			if (board[this->y][j] != NULL)
			{
				if (board[this->y][j]->name[0] != this->name[0])
				{
					possibleMoves.insert(toMove(j, this->y));
					captureMoves.insert(toMove(j, this->y));//adding the possible capture/attacked piece
				}
				break;
			}

			possibleMoves.insert(toMove(j, this->y));
		}

		return possibleMoves;
	}
};

class Queen :public Pieces {
public:
	Queen(string pos, string color) :Pieces(pos, color + "Q", 90) {}

	unordered_set<string> pseudoLegalMoves(string pos) {
		possibleMoves.clear();
		Bishop bisMoves = *((Bishop*)this);
		Rook rookMoves = *((Rook*)this);

		unordered_set<string> diagonals = bisMoves.pseudoLegalMoves(pos);//downcasting piece as bishop and storing its possible moves
		possibleMoves = rookMoves.pseudoLegalMoves(pos);//downcasting piece as rook and storing its possible moves
		unordered_set<string> diagonalCapture = bisMoves.captureMoves;
		captureMoves = rookMoves.captureMoves;

		//both above functions clear the possibleMoves list by default so hence need to save in seperate variables

		for (string potentialPos : diagonals)
			possibleMoves.insert(potentialPos);
		for (string potentialCapture : diagonalCapture)
			captureMoves.insert(potentialCapture);
		return possibleMoves;
	}
};

class King :public Pieces {
	Player* opponent;
public:
	list<string> checks;
	King(string pos, string color) :Pieces(pos, color + "K", 999) {}

	void setOpponent(Player& opp);

	bool isChecked() {//logic for checking if piece checked
		checks.clear();
		Rook LinearThreats = *((Rook*)this);
		Bishop DiagonalThreats = *((Bishop*)this);
		Knight LThreats = *((Knight*)this);
		Pawn pawnThreats = *((Pawn*)this);

		LinearThreats.pseudoLegalMoves(toMove(this->x, this->y));//this covers all horizontal and vertical threats the king may face
		LThreats.pseudoLegalMoves(toMove(this->x, this->y));//this covers Knight threats
		DiagonalThreats.pseudoLegalMoves(toMove(this->x, this->y));//covers diagonals
		pawnThreats.pseudoLegalMoves(toMove(this->x, this->y));//covers smaller siagonal with a pawn

		//merging all the threats
		for (string threat : LinearThreats.captureMoves) {
			int* pos = decodePosition(threat);	
			Pieces* opPiece = board[pos[1]][pos[0]];
			if(opPiece->name[0] != this->name[0] &&//linear check
				(opPiece->name[1] == 'R' ||
				opPiece->name[1] == 'Q')
				)
				checks.push_back(threat);
		}

		for (string threat : DiagonalThreats.captureMoves) {
			int* pos = decodePosition(threat);
			Pieces* opPiece = board[pos[1]][pos[0]];
			if (opPiece->name[0] != this->name[0] &&//diagonal check
				(opPiece->name[1] == 'B' ||
				opPiece->name[1] == 'Q')
				)
				checks.push_back(threat);
		}

		for (string threat : pawnThreats.captureMoves) {
			int* pos = decodePosition(threat);
			Pieces* opPiece = board[pos[1]][pos[0]];
			if (opPiece->name[0] != this->name[0] && opPiece->name[1] == 'P') //Checking for Pawn
				checks.push_back(threat);
		}

		for (string threat : LThreats.captureMoves) {
			int* pos = decodePosition(threat);
			Pieces* opPiece = board[pos[1]][pos[0]];
			if (opPiece->name[0] != this->name[0] && opPiece->name[1] == 'N') //Checking for Knight 
				checks.push_back(threat);
		}

		for (string threat : checks) {
			cout << threat << " ";
		}
		cout << endl;
		if (checks.empty())
			return false;
		return true;

	}

	unordered_set<string> pseudoLegalMoves(string pos){
		possibleMoves.clear();
		//N
		if (y > 0 && (board[this->y - 1][this->x] == NULL || board[this->y - 1][this->x]->name[0] != this->name[0]))
			possibleMoves.insert(toMove(this->x, this->y - 1));
		//S
		if (y < 7 && (board[this->y + 1][this->x] == NULL || board[this->y + 1][this->x]->name[0] != this->name[0]))
			possibleMoves.insert(toMove(this->x, this->y + 1));
		//E
		if (x < 7 && (board[this->y][this->x + 1] == NULL || board[this->y][this->x + 1]->name[0] != this->name[0]))
			possibleMoves.insert(toMove(this->x + 1, this->y));
		//W
		if (x > 0 && (board[this->y][this->x - 1] == NULL || board[this->y][this->x - 1]->name[0] != this->name[0]))
			possibleMoves.insert(toMove(this->x - 1, this->y));
		//NW
		if (y > 0 && x > 0 && (board[this->y - 1][this->x - 1] == NULL || board[this->y - 1][this->x - 1]->name[0] != this->name[0]))
			possibleMoves.insert(toMove(this->x - 1, this->y - 1));
		//SW
		if (y < 7 && x > 0 && (board[this->y + 1][this->x - 1] == NULL || board[this->y + 1][this->x - 1]->name[0] != this->name[0]))
			possibleMoves.insert(toMove(this->x - 1, this->y + 1));
		//SE
		if (y < 7 && x < 7 && (board[this->y + 1][this->x + 1] == NULL || board[this->y + 1][this->x + 1]->name[0] != this->name[0]))
			possibleMoves.insert(toMove(this->x + 1, this->y + 1));
		//NE
		if (y > 0 && x < 7 && (board[this->y - 1][this->x + 1] == NULL || board[this->y - 1][this->x + 1]->name[0] != this->name[0]))
			possibleMoves.insert(toMove(this->x + 1, this->y - 1));

		//checking for castling
		// add color of rook as well just to prevent any bugs in custom positions
		//King side castling hence west
		if (board[this->y][0] != NULL &&//checking if piece in that location exists
			board[this->y][0]->name[1] == 'R' && board[this->y][0]->prevX.size() == 1 &&//the square has same coloured unmoved Rook
			board[this->y][1] == NULL && board[this->y][2] == NULL &&//empty spaces between left rook and king
			this->prevX.size() == 1 && this->x == 3)//king has not moved from its own position
		{
			possibleMoves.insert(toMove(this->x - 2, this->y));//Castling possible, write castling logic below
			if (pos == *(possibleMoves.end()--))//hence user wants to castle
			{
				//this->x -= 2;
				//moving and updating rooks position
				board[this->y][0]->move(toMove(2, this->y));
				//>>>>Add some undo logic here<<<<
			}
		}

		//Queen side castling hence east
		if (board[this->y][7] != NULL &&//checking if piece in that location exists
			board[this->y][7]->name[1] == 'R' && board[this->y][7]->prevX.size() == 1 &&//the square has same coloured unmoved Rook
			board[this->y][6] == NULL && board[this->y][5] == NULL && board[this->y][4] == NULL &&//empty spaces between right rook and king
			this->prevX.size() == 1 && this->x == 3)//king has not moved from its own position
		{
			possibleMoves.insert(toMove(this->x + 2, this->y));//Castling possible, write castling logic below
			if (pos == *(possibleMoves.end()--))//hence user wants to castle
			{
				//this->x += 2;
				board[this->y][7]->move(toMove(4, this->y));
				//>>>>Add some undo logic here<<<<
			}
		}

		if (kingToMove == this->name)
			legalMoves();
		return possibleMoves;
	}//change return to legalMoves function

	void legalMoves();
};

//Module 2-Player
class Player {
	bool isWhite, isChecked;
	list<Pieces*> pieces;
	//These are the pieces that have been moved/captured
	stack<Pieces*> movedPieces;
	King* king;
	int counter;
public:

	void setCheck(bool check){
		isChecked = check;
	}

	bool Checked() {
		return isChecked;
	}

	King* getKing() {
		return king;
	}

	Player(bool isWhite) : isWhite(isWhite), isChecked(false) {
		//The counter keeps track of how many undo's the player can do
		counter = 0;

		//Adding a buffer element so that the stack does not give errors
		movedPieces.push(NULL);
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
		/*for (char i = 'a'; i < 'i'; i++)
		{
			if (isWhite)
				pieces.push_back(new Pawn(string(1, i) + "2", "w"));
			else
				pieces.push_back(new Pawn(string(1, i) + "7", "b"));
		}

		if (isWhite) {
			pieces.push_back(new King("e1", "w"));
			pieces.push_back(new Queen("d1", "w"));
			pieces.push_back(new Bishop("c1", "w"));
			pieces.push_back(new Bishop("f1", "w"));
			pieces.push_back(new Knight("g1", "w"));
			pieces.push_back(new Knight("b1", "w"));
			pieces.push_back(new Rook("h1", "w"));
			pieces.push_back(new Rook("a1", "w"));
		}

		else {
			pieces.push_back(new King("e8", "b"));
			pieces.push_back(new Queen("d8", "b"));
			pieces.push_back(new Bishop("c8", "b"));
			pieces.push_back(new Bishop("f8", "b"));
			pieces.push_back(new Knight("g8", "b"));
			pieces.push_back(new Knight("b8", "b"));
			pieces.push_back(new Rook("a8", "b"));
			pieces.push_back(new Rook("h8", "b"));
		}*/

		//>>>>Add pieces here<<<<
		/*pieces.push_back(new Pawn("e4", "b"));
		pieces.push_back(new Pawn("b7", "b"));
		pieces.push_back(new Pawn("f2", "w"));
		pieces.push_back(new Pawn("c5", "w"));*/

		if (isWhite) {
			pieces.push_back(new Queen("c5", "w"));
			pieces.push_back(new Knight("h6", "w"));
			pieces.push_back(new Knight("b6", "w"));
		}
		else {
			pieces.push_back(new Rook("f8", "b"));
			pieces.push_back(new Bishop("h3", "b"));
		}
	}

	list<Pieces*>& getPieces() {
		return pieces;
	}

	stack<Pieces*>& getMovedPieces()
	{
		return movedPieces;
	}

	void pushPiece(Pieces* pushed) {
		if (counter < 3)
		{
			counter++;
		}
		movedPieces.push(pushed);
	}

	void undo() {
		if (movedPieces.top() == NULL && !movedPieces.empty())
		{
			movedPieces.pop();
			//>>>>problem when returning the latest of the captured pieces<<<,
			//movedPieces.pop();//popping twice to retrive the captured piece as well
		}

		else if (movedPieces.size() > 1 && counter > 0)
		{
			movedPieces.top()->undoMove();
			movedPieces.pop();

			counter--;
		}
	}

	void Captured(Pieces* piece)
	{
		piece->prevX.push(piece->x);
		piece->prevY.push(piece->y);

		this->pushPiece(piece);
	}

	void LegalMovesInCheck() {
		cout << "Legal Moves here";
	}

	void Promotion(Pieces* piece)
	{
		if (piece->name[1] == 'P' && (piece->y == 0 || piece->y == 7))
		{
			char choice = ' ';
			string piecePosition = "Z0";
			piecePosition[0] = piece->x + 'a';
			piecePosition[1] = piece->y + '1';

			Captured(piece);

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
			counter = 0;
		}
	}
};

//>>>>Forwading for the king functions<<<<
void King::setOpponent(Player& opp) {
	opponent = &opp;
}

void King::legalMoves() {
	//possibleMoves = pseudoLegalMoves(toMove(this->x, this->y));
	board[this->y][this->x] = NULL;//so the king is no longer in the "ray" of the attacking piece
	for (Pieces* piece : opponent->getPieces()) {
		piece->pseudoLegalMoves(toMove(piece->x, piece->y));//setting the pieces pseudoLegalMoves unordered set
		for (string move : piece->possibleMoves)
			if (possibleMoves.count(move) > 0)
				possibleMoves.erase(possibleMoves.find(move));
	}
	board[this->y][this->x] = this;
}

//Module 3-Board
void displayBoard() {
	cout << "   ";
	for (int i = 0; i < 8; i++)
		cout << "____ ";
	cout << endl;
	for (int i = 0; i < 8; i++) {
		cout << i + 1 << " ";
		for (int j = 0; j < 8; j++) {
			cout << "| ";
			if (board[i][j] != NULL)
				cout << board[i][j]->name;
			else
				cout << "..";
			cout << " ";
		}
		cout << "|\n  ";
		for (int j = 0; j < 8; j++) {
			cout << "|____";
		}
		cout << "|" << endl;
	}
	cout << "  ";
	for (char a = 'a'; a < 'i'; a++) {
		cout << "  " << a << "  ";
	}
	cout << endl;
}

//Used to decode the position of the spaces from string to integers
int* decodePosition(string pos) {
	int* position = new int[2];
	position[0] = pos[0] - 'a';
	position[1] = pos[1] - '1';

	return position;
}

//Input validation function
bool isvalid(bool isWhite, string space)
{
	if (board[space[1] - '1'][space[0] - 'a'] == NULL)
	{
		cout << "That is not a selectable Piece." << endl;
		return false;
	}

	else if ((board[space[1] - '1'][space[0] - 'a']->name[0] == 'w' && !isWhite) || (board[space[1] - '1'][space[0] - 'a']->name[0] == 'b' && isWhite))
	{
		cout << "You cannot control those pieces." << endl;
		return false;
	}

	else if (space[1] > '8' || space[0] > 'h')
	{
		cout << "Out of bounds" << endl;
		return false;
	}

	return true;
}

//This function checks if the user's input is validated
bool validPosition(string Space)
{
	try
	{
		board.at(Space[1] - '1').at(Space[0] - 'a');
		return true;
	}
	catch (...)
	{
		return false;
	}

}

//This boolean will decide whether to skip the next player's turn or not.
bool PlayerTurn(Player& A, Player& B, bool isWhite, int& WinorLose)
{	//Player A is the current player
	//need info on the opponent king so we can prevent their move generation
	kingToMove = A.getKing()->name;
	string p1, p2;
	bool CheckRollback = false;
	displayBoard();

	if (A.getKing()->isChecked()) {
		cout << "Check!!!\n";
		A.LegalMovesInCheck();
	}

	cout << "What would you like to do?\n" << "1.Make a move" << endl << "2.Undo a Move" << endl << "3.Give up" << endl << endl << "Your choice: ";
	do
	{
		cin >> p1;
	} while (p1 != "1" && p1 != "2" && p1 != "3");

	if (p1 == "3")
	{
		if (isWhite)
			WinorLose = 2;
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
			A.undo();
			B.undo();


			//Undo for the Opponent
			A.undo();
			B.undo();
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

		} while (!validPosition(p1));

		//This checks if the piece chosen is actually the player's
		if (!isvalid(isWhite, p1))
			continue;

		do
		{
			cout << "Enter move: ";
			cin >> p2;

		} while (!validPosition(p2));


		if (board[p1[1] - '1'][p1[0] - 'a']->validMoves(p2))//, board[p1[1] - '1'][p1[0] - 'a']->name))
		{
			//Pushing the enemy piece onto the Undo Stack
			if (board[p2[1] - '1'][p2[0] - 'a'] != NULL)
				B.Captured(board[p2[1] - '1'][p2[0] - 'a']);

			//En Passant requires this in order to work. Must be repeated for the opponent
			//In case they use en passant as well
			else if (Enpassant && isWhite)
				B.Captured(board[p2[1] - '1' - 1][p2[0] - 'a']);
			else if (Enpassant)
				B.Captured(board[p2[1] - '1' + 1][p2[0] - 'a']);
			//Adding in NULL to make sure Undo's work properly
			else
				B.pushPiece(NULL);
			Enpassant = false;


			//Pushing previous Piece and moving it
			A.pushPiece(board[p1[1] - '1'][p1[0] - 'a']);

			//Moving the piece on the board
			board[p1[1] - '1'][p1[0] - 'a']->move(p2);

			//Promotion in case it happens
			A.Promotion(board[p2[1] - '1'][p2[0] - 'a']);
		}
		else
		{
			cout << "Invalid Move" << endl;
			continue;
		}

		//Finding the King in the Player's pieces
		/*typename list<Pieces*>::iterator iterPieces = A.getPieces().begin();
		for (int i = 0; i < A.getPieces().size(); i++)
		{
			if ((*iterPieces)->name[1] == 'K')
			{
				//if it's checked, undoing the player's last move
				if ((*iterPieces)->isChecked())
				{
					A.undo();
					B.undo();

					CheckRollback = true;
					cout << endl << "Your King is Checked. You cannot make that move." << endl << endl;
				}
				else
					break;
			}

			iterPieces++;
		}

		if (CheckRollback == true)
		{
			CheckRollback = false;
			continue;
		}
		*/
		system("CLS");
		break;
	}

	//Checking if the Enemy king is Checkmated
	/*typename list<Pieces*>::iterator iterPiecesB = B.getPieces().begin();
	for (int i = 0; i < B.getPieces().size(); i++)
	{
		if ((*iterPiecesB)->name[1] == 'K' && (*iterPiecesB)->Checkmate())
		{
			cout << "The enemy king has been checkmated" << endl;
			displayBoard();

			if (isWhite)
				WinorLose = 1;
			else
				WinorLose = 2;
		}

		iterPiecesB++;
	}*/

	return 0;
}

void AITurn(Player& AI)
{
	//making it do a random move
	//generating pseudo-legal moves
	list<Pieces*>::iterator p = AI.getPieces().begin();

	/*for (string move : (*p)->pseudoLegalMoves())
		cout << move << " ";*/
		/*while (p != AI.getPieces().end()) {
			/*if (rand() % 12 == 0)
				break;
		}*/

}

void vsAIGame()
{
	system("CLS");
	bool isWhite = rand() % 2;
	bool CheckRollback = false;

	//0 means the game is going
	//1 means you won
	//2 means you lost
	int WinorLose = 0;

	if (isWhite)
		cout << "You are the White Player." << endl;
	else
		cout << "You are the Black Player." << endl;

	//The user will always be Player A
	Player A(isWhite);
	Player B(!isWhite);

	//Main gameplay loop is here
	do {
		string p1, p2;
		displayBoard();

		cout << "What would you like to do?\n" << "1.Make a move" << endl << "2.Undo a Move" << endl << "3.Give up" << endl << endl << "Your choice: ";
		do
		{
			cin >> p1;
		} while (p1 != "1" && p1 != "2" && p1 != "3");

		if (p1 == "3")
			break;

		if (p1 == "2")
		{
			//Undo because of Player
			A.undo();
			B.undo();


			//Undo for the Opponent
			//A.undo();
			//B.undo();
			system("CLS");
			continue;
		}

		//Loop for the player
		while (true)
		{
			//User's turn
			do
			{
				cout << "Enter current position of piece: ";
				cin >> p1;

			} while (!validPosition(p1));

			//This checks if the piece chosen is actually the player's
			if (!isvalid(isWhite, p1))
				continue;

			do
			{
				cout << "Enter move: ";
				cin >> p2;

			} while (!validPosition(p2));


			if (board[p1[1] - '1'][p1[0] - 'a']->validMoves(p2))//, board[p1[1] - '1'][p1[0] - 'a']->name))
			{
				//Pushing the enemy piece onto the Undo Stack
				if (board[p2[1] - '1'][p2[0] - 'a'] != NULL)
					B.Captured(board[p2[1] - '1'][p2[0] - 'a']);

				//En Passant requires this in order to work. Must be repeated for the opponent
				//In case they use en passant as well
				else if (Enpassant && isWhite)
					B.Captured(board[p2[1] - '1' - 1][p2[0] - 'a']);
				else if (Enpassant)
					B.Captured(board[p2[1] - '1' + 1][p2[0] - 'a']);
				//Adding in NULL to make sure Undo's work properly
				else
					B.pushPiece(NULL);
				Enpassant = false;


				//Pushing previous Piece and moving it
				A.pushPiece(board[p1[1] - '1'][p1[0] - 'a']);

				//Moving the piece on the board
				board[p1[1] - '1'][p1[0] - 'a']->move(p2);

				//Promotion in case it happens
				A.Promotion(board[p2[1] - '1'][p2[0] - 'a']);
			}
			else
			{
				cout << "Invalid Move" << endl;
				continue;
			}

			break;
		}

		//Checking if the Enemy king is Checkmated
		typename list<Pieces*>::iterator iterPiecesB = B.getPieces().begin();
		for (int i = 0; i < B.getPieces().size(); i++)
		{
			if ((*iterPiecesB)->name[1] == 'K' && (*iterPiecesB)->Checkmate())
			{
				cout << "The enemy king has been checkmated" << endl;
				displayBoard();
				WinorLose = 1;
			}

			iterPiecesB++;
		}

		//Finding the King in the Player's pieces
		typename list<Pieces*>::iterator iterPieces = A.getPieces().begin();
		for (int i = 0; i < A.getPieces().size(); i++)
		{
			if ((*iterPieces)->name[1] == 'K')
			{
				//if it's checked, undoing the player's last move
				if ((*iterPieces)->isChecked())
				{
					A.undo();
					B.undo();

					system("CLS");
					CheckRollback = true;
					cout << endl << "Your King is Checked. You cannot make that move." << endl << endl;
				}
				else
					break;
			}

			iterPieces++;
		}

		//return to the Player's turn if it's king was checked because of the last move.
		if (CheckRollback)
		{
			CheckRollback = false;
			continue;
		}

		//AI's turn
		while (true)
		{

			//Stuff added down here

			//B.pushPiece(board[p1[1] - '1'][p1[0] - 'a']);
			//board[p1[1] - '1'][p1[0] - 'a']->move(p2);
			//system("CLS");
			break;
		}

	} while (WinorLose == 0);


	if (WinorLose == 1)
		cout << endl << "Congratulations! You've won the game!";
	else
		cout << endl << "Sorry, the AI has won the game!";

	_getch();
}

void vsPlayerGame()
{
	system("CLS");
	bool isWhite = rand() % 2;
	bool CheckRollback = false;
	bool skipPlayer = false;
	int WinorLose = 0;
	//setting new players
	Player A(isWhite);
	Player B(!isWhite);

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

	if (WinorLose == 1)
		cout << endl << "The White Player has won the game!";
	else
		cout << endl << "The Black Player has won the game!";

	_getch();
}

int main() {
	srand(unsigned(time(NULL)));

	while (true)
	{
		//Clearing the board for the next game
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				board[i][j] = NULL;

		//Clearing the screen of clutter
		system("CLS");

		int choice = 0;
		cout << "\t\t         CHESS" << endl;
		cout << "\t\tWhat kind of game would you like to play?" << endl;
		cout << "\t\t1. vs AI" << endl << "\t\t2. vs Player" << endl << "\t\t3.Quit" << endl;
		cout << "\t\tYour choice: ";
		while (choice != 1 && choice != 2 && choice != 3)
			cin >> choice;

		switch (choice)
		{
		case 1:
			vsAIGame();
			break;
		case 2:
			vsPlayerGame();
			break;
		case 3:
			break;
		}

		if (choice == 3)
			break;
	}
	return 0;
}