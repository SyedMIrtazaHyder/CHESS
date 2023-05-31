#include "board.h"
#include "pieces.h"
using namespace std;

vector< vector<Pieces*> > Board::board(8, vector<Pieces*>(8, NULL));

void Board::displayBoard() {
	cout << "   ";
	for (int i = 0; i < 8; i++)
		cout << "____ ";
	cout << endl;
	for (int i = 0; i < 8; i++) {
		cout << i + 1 << " ";
		for (int j = 0; j < 8; j++) {
			cout << "| ";
			if (board[i][j] != NULL)
				cout << Board::board[i][j]->name;
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
int* Board::decodePosition(string pos) {
	int* position = new int[2];
	position[0] = pos[0] - 'a';
	position[1] = pos[1] - '1';

	return position;
}

string Board::toMove(int x, int y) {
	return string(1, (char)('a' + x)) + string(1, (char)('1' + y));
}

//Input validation function
bool Board::isvalid(bool isWhite, string space)
{
	if (board[space[1] - '1'][space[0] - 'a'] == NULL)
	{
		cout << "That is not a selectable Piece." << endl;
		return false;
	}

	else if ((board[space[1] - '1'][space[0] - 'a']->isWhite() && !isWhite) || (board[space[1] - '1'][space[0] - 'a']->name[0] == 'b' && isWhite))
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
bool Board::validPosition(string Space)
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
