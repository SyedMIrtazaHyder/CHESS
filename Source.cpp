#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <math.h>
#include <conio.h>

using namespace std;
class Pieces;
int* decodePosition(string pos);
bool validPosition(string Space);


bool Enpassant = false;
vector< vector<Pieces*> > board(8, vector<Pieces*>(8, NULL));

//Module 1-Pieces
class Pieces {
public:
	int x, y, value;
	string name;
	stack<int> prevX;
	stack<int> prevY;
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
				if (board[i][j] == NULL || board[this->y][this->x]->name[0] == board[i][j]->name[0])
					continue;

				if ((board[i][j])->validMoves(PiecePos, (board[i][j])->name))
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
				if (validPosition(isvalidSpace) && validMoves(isvalidSpace, name))
					validSpaces.push_back(isvalidSpace);
			}


		//Checking if the King is checked
		if (isChecked())
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{

					//if the pieces have the same colour, it continues to the next piece on the board
					if (board[i][j] == NULL || board[this->y][this->x]->name[0] == board[i][j]->name[0])
						continue;

					//This is to check if there is more than one piece checking the king
					if (board[i][j]->isChecked() && chance && board[i][j]->validMoves(PiecePos, board[i][j]->name))
					{
						chance = false;
						continue;
					}


					if (board[i][j]->validMoves(PiecePos, board[i][j]->name))
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

	bool validMoves(string pos, string PieceType)
	{
		int* position = decodePosition(pos);

		if (board[position[1]][position[0]] != NULL && board[position[1]][position[0]]->name[0] == name[0])
			return false;

		switch (PieceType[1]) {
		case 'P':
			if (name[0] == 'w')
			{
				//The pawn's logic for each movement type is complicated, therefore the multiple if statements
				if ((position[1] - y) == 1 && (x - position[0]) == 0 && board[position[1]][x] == NULL)
					return true;
				if ((position[1] - y) == 2 && (x - position[0]) == 0 && board[position[1]][x] == NULL && board[position[1] - 1][x] == NULL && y == 1)
					return true;
				if ((position[1] - y) == 1 && abs(x - position[0]) == 1 && board[position[1]][position[0]] != NULL && board[position[1]][position[0]]->name[0] == 'b')
					return true;

				//Condition for en passant for White
				if ((position[1] - y) == 1 && abs(x - position[0]) == 1)
					if (board[position[1] - 1][position[0]] != NULL && board[position[1] - 1][position[0]]->name[0] != name[0] && (board[position[1] - 1][position[0]]->prevY.top() - board[position[1] - 1][position[0]]->y) == 2)
					{
						Enpassant = true;
						return true;
					}
			}
			else
			{
				if ((y - position[1]) == 1 && (x - position[0]) == 0 && board[position[1]][x] == NULL)
					return true;
				if ((y - position[1]) == 2 && (x - position[0]) == 0 && board[position[1]][x] == NULL && board[position[1] + 1][x] == NULL && y == 6)
					return true;
				if ((y - position[1]) == 1 && abs(x - position[0]) == 1 && board[position[1]][position[0]] != NULL && board[position[1]][position[0]]->name[0] == 'w')
					return true;

				//Condition for en passant for Black
				if ((y - position[1]) == 1 && abs(x - position[0]) == 1)
					if (board[position[1] + 1][position[0]] != NULL && board[position[1] + 1][position[0]]->name[0] != name[0] && (board[position[1] + 1][position[0]]->y - board[position[1] + 1][position[0]]->prevY.top()) == 2)
					{
						Enpassant = true;
						return true;
					}
			}

			break;

		case 'K':
			if ((abs(y - position[1]) == 1 || abs(x - position[0]) == 1) && (board[position[1]][position[0]] == NULL || board[position[1]][position[0]]->name[0] != name[0]))
				return true;

			break;

		case 'Q':
			return validMoves(pos, string(1,name[0]) + "B") || validMoves(pos, string(1,name[0]) + "R");
			break;

		case 'N':
			if (((abs(y - position[1]) == 1 && abs(x - position[0]) == 2) || (abs(y - position[1]) == 2 && abs(x - position[0]) == 1)) && (board[position[1]][position[0]] == NULL || board[position[1]][position[0]]->name[0] != name[0]))
				return true;

			break;

		case 'B':
			if (position[1] != y && position[0] != x && (abs(y - position[1]) == abs(x - position[0])))
			{
				int signy = (y - position[1]) / abs(y - position[1]);
				int signx = (x - position[0]) / abs(x - position[0]);

				for (int i = 1; i < abs(y - position[1]); i++)
				{
					if (board[y - i * signy][x - i * signx] == NULL)
						continue;
					else
						return false;
				}

				return true;
			}

			break;

		case 'R':
			if (position[1] == y && position[0] != x)
			{
				//This is a bad way to get the sign of a number, but it works
				int sign = (x - position[0]) / (abs(x - position[0]));

				for (int i = 1; i < abs(x - position[0]); i++)
				{
					if (board[y][x - (i * sign)] == NULL)
						continue;
					else
						return false;
				}

				return true;
			}

			else if (position[1] != y && position[0] == x)
			{
				int sign = (y - position[1]) / (abs(y - position[1]));


				for (int i = 1; i < abs(y - position[1]); i++)
				{
					if (board[y - (i * sign)][x] == NULL)
						continue;
					else
						return false;
				}

				return true;
			}

			break;
		}

		return false;
	}

	void move(string pos) {
		//Pushes the new position of the piece onto the stack.
		prevX.push(x);
		prevY.push(y);
		
		board[y][x] = NULL;
		x = pos[0] - 'a';
		y = pos[1] - '1';
		board[y][x] = this;
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
};

class Pawn :public Pieces {
public:
	Pawn(string pos, string color) :Pieces(pos, color + "P", 1) {}
};

class King :public Pieces {
public:
	King(string pos, string color) :Pieces(pos, color + "K", 1) {}
	
};

class Queen :public Pieces {
public:
	Queen(string pos, string color) :Pieces(pos, color + "Q", 1) {}
};

class Knight :public Pieces {
public:
	Knight(string pos, string color) :Pieces(pos, color + "N", 1) {}
};

class Bishop :public Pieces {
public:
	Bishop(string pos, string color) :Pieces(pos, color + "B", 1) {}
};

class Rook :public Pieces {
public:
	Rook(string pos, string color) :Pieces(pos, color + "R", 1) {}
};

//Module 2-Player
class Player {
	bool isWhite;
	list<Pieces*> pieces;
	//These are the pieces that have been moved/captured
	stack<Pieces*> movedPieces;
	int counter;
public:

	Player(bool isWhite) : isWhite(isWhite) {
		//The counter keeps track of how many undo's the player can do
		counter = 0;
		
		//Adding a buffer element so that the stack does not give errors
		movedPieces.push(NULL);

		//placing pawns
		for (char i = 'a'; i < 'i'; i++)
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
		}
	}

	list<Pieces*>& getPieces() {
		return pieces;
	}

	void pushPiece(Pieces* pushed) {
		if (counter < 3)
		{
			counter++;
		}

		movedPieces.push(pushed);
	}

	void undo() {
		if (movedPieces.top() == NULL && movedPieces.size() > 1)
			movedPieces.pop();

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

		board[piece->y][piece->x] = NULL;

		this->pushPiece(piece);
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
				pieces.push_back(new Knight(piecePosition, string(1,piece->name[0])));
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
bool isvalid(bool playerColour, string space)
{
	if (board[space[1] - '1'][space[0] - 'a'] == NULL)
	{
		cout << "That is not a selectable Piece." << endl;
		return false;
	}

	else if (board[space[1] - '1'][space[0] - 'a']->name[0] == 'w' && !playerColour)
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

bool PlayerTurn(Player &Playing, Player &Enemy)
{
	return false;
}

void AITurn()
{
	
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

			
			if (board[p1[1] - '1'][p1[0] - 'a']->validMoves(p2, board[p1[1] - '1'][p1[0] - 'a']->name))
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
			system("CLS");
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

	if (isWhite)
		cout << "Player 1 will Control White and Player 2 will Control Black." << endl;
	else
		cout << "Player 1 will Control Black and Player 2 will Control White." << endl;

	Player A(isWhite);
	Player B(!isWhite);

}

int main() {
	srand (unsigned(time(NULL)));

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