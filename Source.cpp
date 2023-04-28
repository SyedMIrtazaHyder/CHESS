#include <iostream>
#include <list>
#include <stack>
#include <vector>
using namespace std;

vector<vector<Pieces>> board;

//Module 1-Pieces
class Pieces {
public:
	int x, y, value;
	//This stack is useful
	stack<int[2]> prevMoves;
	string name;
	Pieces(string pos, string name, int value) :x(pos[0] - 'a'), y(pos[1] - '0' - 1), value(value), name(name) {
		board[y][x] = name;

		//pushes original position of piece onto stack.
		prevMoves.push({ y,x });
	}

	virtual bool validMoves(string pos) = 0;

	void move(string pos) {

		board[y][x] = "..";
		x = pos[0] - 'a';
		y = pos[1] - '0' - 1;
		board[y][x] = name;

		//Pushes the new position of the piece onto the stack.
		prevMoves.push({ y,x });
	}
};

class Pawn :public Pieces {
public:
	Pawn(string pos, string color) :Pieces(pos, color + "P", 1) {}
	
	bool validMoves(string pos) {
		
	}
};

class King :public Pieces {
public:
	King(string pos, string color) :Pieces(pos, color + "K", 1) {}
	bool validMoves(string pos) { return false; }
};
class Queen :public Pieces {
public:
	Queen(string pos, string color) :Pieces(pos, color + "Q", 1) {}
	bool validMoves(string pos) { return false; }
};
class Knight :public Pieces {
public:
	Knight(string pos, string color) :Pieces(pos, color + "N", 1) {}
	bool validMoves(string pos) { return false; }
};
class Bishop :public Pieces {
public:
	Bishop(string pos, string color) :Pieces(pos, color + "B", 1) {}
	bool validMoves(string pos) { return false; }
};
class Rook :public Pieces {
public:
	Rook(string pos, string color) :Pieces(pos, color + "R", 1) {}
	bool validMoves(string pos) { return false; }
};


//Module 2-Player
class Player {
	bool isWhite;
	list<Pieces*> pieces;
	//These are the pieces that have been moved
	stack<Pieces> movedPieces;
	int counter;
public:

	Player(bool isWhite) : isWhite(isWhite) {
		//The counter keeps track of how many undo's the player can do
		counter = 0;
		
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
			pieces.push_back(new Rook("h8", "b"));
			pieces.push_back(new Rook("a8", "b"));
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
			cout << "| " << board[i][j] + " ";
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
}

int* decodePosition(string pos) {
	int* position = new int[2];
	position[0] = pos[0] - 'a';
	position[1] = pos[1] - '0' - 1;

	return position;
}

int main() {

	//initializing board
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board[j][i] = "..";

	Player B(false);
	Player A(true);
	displayBoard();
	return 0;
}