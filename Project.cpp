#include <iostream>
#include <list>
#include <stack>
#include <vector>
using namespace std;
class Pieces;

vector< vector<Pieces*> > board(8, vector<Pieces*>(8, NULL));
//Module 1-Pieces
class Pieces {
public:
	int x, y, value;
	string name;
	Pieces():x(0),y(0),value(0), name("NULL") {}
	Pieces(string pos, string name, int value):x(pos[0] - 'a' + 1), y(pos[1] - '0'), value(value), name(name) {
		board[y - 1][x - 1] = this;
	}

	virtual void validMoves(string pos) = 0;

	void move(string pos) {
		board[y - 1][x - 1] = NULL;
		x = pos[0] - 'a' + 1;
		y = pos[1] - '0';
		board[y - 1][x - 1] = this;
	}
};

class Pawn :public Pieces {
public:
	Pawn(string pos, string color):Pieces(pos, color + "P", 1) {}
	void validMoves(string pos){}
};

class King :public Pieces {
public:
	King(string pos, string color) :Pieces(pos, color + "K", 1) {}
	void validMoves(string pos) {}
};
class Queen :public Pieces {
public:
	Queen(string pos, string color) :Pieces(pos, color + "Q", 1) {}
	void validMoves(string pos) {}
};
class Knight :public Pieces {
public:
	Knight(string pos, string color) :Pieces(pos, color + "N", 1) {}
	void validMoves(string pos) {}
};
class Bishop :public Pieces {
public:
	Bishop(string pos, string color) :Pieces(pos, color + "B", 1) {}
	void validMoves(string pos) {}
};
class Rook :public Pieces {
public:
	Rook(string pos, string color) :Pieces(pos, color + "R", 1) {}
	void validMoves(string pos) {}
};


//Module 2-Player
class Player {
	bool isWhite;
	list<Pieces*> pieces;
public:

	Player(bool isWhite): isWhite(isWhite){
		//placing pawns
		for (char i = 'a'; i < 'i'; i++)
		{
			if (isWhite)
				pieces.push_back(new Pawn(string(1,i) + "2", "w"));
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

int main() {
	Player B(false);
	Player A(true);
	do {
		string p1, p2;
		displayBoard();
		cout << "Enter current position of piece: ";
		cin >> p1;
		cout << "Enter move: ";
		cin >> p2;
		board[p1[1] - '1'][p1[0] - 'a']->move(p2);
	} while (true);
	return 0;
}