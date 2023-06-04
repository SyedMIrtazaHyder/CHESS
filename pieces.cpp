#include "board.h"
#include "pieces.h"
using namespace std;

//global vars
Pieces* getEnPassant = NULL;
Pieces* checkEnPassant;
string kingToMove;//0 = white, 1 = black
int castling = 0;//-1 for queen Side, 1 for king side, 0 for not possible
string castlingMoves[4] = { "g8", "g1", "c8", "c1" };

Moves::Moves(string move, int value) : value(value), move(move) {}
bool Moves::operator < (const Moves& alt) const {
	//cout << value << " & " << alt.value << endl;
	return this->value < alt.value;
}
bool Moves::operator <= (const Moves& alt) const {
	//cout << value << " & " << alt.value << endl;
	return this->value <= alt.value;
}
bool Moves::operator > (const Moves& alt) const {
	//cout << value << " & " << alt.value << endl;
	return this->value > alt.value;
}
Moves::Moves() : value(-1), move("NULL") {}


Pieces::Pieces() :x(0), y(0), value(0), name("NULL") {}
Pieces::Pieces(string pos, string name, int value) :x(pos[0] - 'a'), y(pos[1] - '1'), value(value), name(name) {
	Board::board[y][x] = this;

	//pushes original position of piece onto stack.
	prevX.push_front(x);
	prevY.push_front(y);
}
bool Pieces::isWhite() {
	return name[0] == 'w';
}
bool Pieces::isOpponent(Pieces& Opp) {
	return name[0] != Opp.name[0];
}
void Pieces::resetMoves() {
	while (!bestMoves.empty())
		bestMoves.pop();
	possibleMoves.clear();
	captureMoves.clear();
}
bool Pieces::validMoves(string pos) {
	checkEnPassant = getEnPassant;
	pseudoLegalMoves();//EnPassant decided after this line
	cout << "Moves: ";
	for (string move : this->possibleMoves)
		cout << move << " ";

	cout << "\nCaptures: ";
	for (string move : this->captureMoves)
		cout << move << " ";

	cout << endl;
	return find(possibleMoves.begin(), possibleMoves.end(), pos) != possibleMoves.end();
}
Pieces* Pieces::move(string pos) {
	//Pushes the new position of the piece onto the stack.
	//Pieces* checkEnPassant = getEnPassant;
	//for (string move : this->possibleMoves)
	//	cout << move << " ";
	Pieces* capturedPiece = NULL;

	if (getEnPassant != NULL && getEnPassant == this && (//for enPassant
		(Board::toMove(x, y + 2) != pos && this->isWhite()) ||
		(Board::toMove(x, y - 2) != pos && this->name[0] == 'b')
		)) {
		getEnPassant = NULL;
	}

	if (checkEnPassant == getEnPassant && getEnPassant != NULL)//may have sm bugs but idk not gonna deep test dis
	{
		if ((pos == Board::toMove(getEnPassant->x, getEnPassant->y - 1) && getEnPassant->isWhite()) ||
			(pos == Board::toMove(getEnPassant->x, getEnPassant->y + 1) && getEnPassant->name[0] == 'b'))
		{
			capturedPiece = Board::board[getEnPassant->y][getEnPassant->x];
			Board::board[getEnPassant->y][getEnPassant->x] = NULL;
			//capturedPiece->name = "\0";
		}
		getEnPassant = NULL;

	}

	if (castling == 1) {// && isCastlingMove(move)) {//code this new function
		if (find(castlingMoves, castlingMoves + 4, pos) == castlingMoves + 4)//failsafe for invalid castling moves
			castling = 0;

		if (pos[0] == 'g')//king side castle
		{
			castling = 0;//need to reset to prevent rook to make infinte loop when it calls move function again
			Board::board[this->y][7]->move(Board::toMove(5, this->y));
		}
		else if (pos[0] == 'c') {//queen side castle
			castling = 0;
			Board::board[this->y][0]->move(Board::toMove(3, this->y));
		}
	}


	if (Board::board[pos[1] - '1'][pos[0] - 'a'] != NULL) {
		capturedPiece = Board::board[pos[1] - '1'][pos[0] - 'a'];
		//capturedPiece->name = "\0";
	}

	//system("pause");
	prevX.push_front(x);
	prevY.push_front(y);


	Board::board[y][x] = NULL;
	x = pos[0] - 'a';
	y = pos[1] - '1';
	Board::board[y][x] = this;

	return capturedPiece;
}
void Pieces::undoMove()
{
	int oldx = prevX.front();
	int oldy = prevY.front();

	Board::board[y][x] = NULL;
	x = oldx;
	y = oldy;
	Board::board[y][x] = this;

	prevX.pop_front();
	prevY.pop_front();
}
bool Pieces::verticalPin() {//so piece cannot move right or left
	//searching a whole file (up and down the piece)
	bool kingInFile = false;
	bool threatInFile = false;

	int file = this->x;
	int rank = this->y;
	for (int i = rank - 1; i >= 0; i--)
		if (Board::board[i][file] != NULL) {
			if (!Board::board[i][file]->isOpponent(*this)) {//checking if king in file
				if (Board::board[i][file]->name[1] == 'K')
					kingInFile = true;
				break;
			}
			else {
				if (!Board::board[i][file]->name[1] == 'Q' || Board::board[i][file]->name[1] == 'R')
					threatInFile = true;
				break;
			}
		}

	for (int i = rank + 1; i < 8; i++)//finding king
		if (Board::board[i][file] != NULL) {
			if (!Board::board[i][file]->isOpponent(*this)) {//checking if king in file
				if (Board::board[i][file]->name[1] == 'K')
					kingInFile = true;
				break;//if any other white piece in way still should break
			}
			else {
				if (Board::board[i][file]->name[1] == 'Q' || Board::board[i][file]->name[1] == 'R')
					threatInFile = true;
				break;
			}
		}

	return kingInFile && threatInFile;
}
bool Pieces::horizontalPin() {//have Enpassanat variation
	//searching a whole rank (left and right of the pieces)
	bool kingInRank = false;
	bool threatInRank = false;

	int rank = this->y;
	int file = this->x;
	for (int i = file + 1; i < 8; i++)
		if (Board::board[rank][i] != NULL) {
			string pieceName = Board::board[rank][i]->name;
			if (pieceName[0] == this->name[0])//checking if king in file
			{
				if (pieceName[1] == 'K')
					kingInRank = true;
				break;
			}

			else {
				if (pieceName[1] == 'Q' || pieceName[1] == 'R')
					threatInRank = true;
				break;
			}
		}

	for (int i = file - 1; i >= 0; i--)
		if (Board::board[rank][i] != NULL) {
			string pieceName = Board::board[rank][i]->name;
			if (pieceName[0] == this->name[0])//checking if king in file immediately
			{
				if (pieceName[1] == 'K')
					kingInRank = true;
				break;
			}

			else {
				if (pieceName[1] == 'Q' || pieceName[1] == 'R')
					threatInRank = true;
				break;
			}
		}
	return kingInRank && threatInRank;
}
bool Pieces::rightDiagonalPin() {
	bool diagonalThreat = false;
	bool kingInDiagonal = false;

	int rank = this->y;
	int file = this->x;

	//NE -> SW
	int i = file + 1; int j = rank + 1;
	for (; i < 8 && j < 8; i++, j++) //NE
		if (Board::board[j][i] != NULL) {
			string pieceName = Board::board[j][i]->name;
			if (pieceName[0] == this->name[0])//checking if king in file immediately
			{
				if (pieceName[1] == 'K')
					kingInDiagonal = true;
				break;
			}

			else {
				if (pieceName[1] == 'Q' || pieceName[1] == 'B')
					diagonalThreat = true;
				break;
			}
		}

	i = file - 1; j = rank - 1;
	for (; i >= 0 && j >= 0; i--, j--)//SW
		if (Board::board[j][i] != NULL) {
			string pieceName = Board::board[j][i]->name;
			if (pieceName[0] == this->name[0])//checking if king in file immediately
			{
				if (pieceName[1] == 'K')
					kingInDiagonal = true;
				break;
			}

			else {
				if (pieceName[1] == 'Q' || pieceName[1] == 'B')
					diagonalThreat = true;
				break;
			}
		}
	return diagonalThreat && kingInDiagonal;
}
bool Pieces::leftDiagonalPin() {
	bool diagonalThreat = false;
	bool kingInDiagonal = false;
	int rank = this->y;
	int file = this->x;

	//NW -> SE
	int i = file - 1; int j = rank + 1;
	for (; i >= 0 && j < 8; i--, j++)//NW
		if (Board::board[j][i] != NULL) {
			string pieceName = Board::board[j][i]->name;
			if (pieceName[0] == this->name[0])//checking if king in file immediately
			{
				if (pieceName[1] == 'K')
					kingInDiagonal = true;
				break;
			}
			else {
				if (pieceName[1] == 'Q' || pieceName[1] == 'B')
					diagonalThreat = true;
				break;
			}
		}

	i = file + 1; j = rank - 1;
	for (; i < 8 && j >= 0; i++, j--)//SE
		if (Board::board[j][i] != NULL) {
			string pieceName = Board::board[j][i]->name;
			if (pieceName[0] == this->name[0])//checking if king in file immediately
			{
				if (pieceName[1] == 'K')
					kingInDiagonal = true;
				break;
			}

			else {
				if (pieceName[1] == 'Q' || pieceName[1] == 'B')
					diagonalThreat = true;
				break;
			}
		}
	return diagonalThreat && kingInDiagonal;
}

bool Pieces::operator==(Pieces& p)
{
	if (this->x == p.x && this->y == p.y && this->value == p.value && this->name == name)
		return true;
	return false;
}

Pawn::Pawn(string pos, string color) :Pieces(pos, color + "P", 30) {}
set<string> Pawn::pseudoLegalMoves() {//generating a rough lookup-table
	resetMoves();
	//for white
	bool Hpin = horizontalPin();
	bool Vpin = verticalPin();
	bool LDpin = leftDiagonalPin();
	bool RDpin = rightDiagonalPin();

	if (isWhite()) {
		if (x > 0 && y < 7 && Board::board[y + 1][x - 1] != NULL && Board::board[y + 1][x - 1]->name[0] == 'b' && !Vpin && !Hpin && !RDpin)//capture left
		{
			possibleMoves.insert(Board::toMove(x - 1, y + 1));
			captureMoves.insert(Board::toMove(x - 1, y + 1));
		}

		if (x < 7 && y < 7 && Board::board[y + 1][x + 1] != NULL && Board::board[y + 1][x + 1]->name[0] == 'b' && !Vpin && !Hpin && !LDpin)//capture right
		{
			possibleMoves.insert(Board::toMove(x + 1, y + 1));
			captureMoves.insert(Board::toMove(x + 1, y + 1));
		}

		if (this->y == 1 && Board::board[y + 2][x] == NULL && Board::board[y + 1][x] == NULL && !Hpin && !LDpin && !RDpin)//double push so chance to enpassant
		{
			possibleMoves.insert(Board::toMove(x, y + 2));
			if ((x > 0 && Board::board[y + 2][x - 1] != NULL && Board::board[y + 2][x - 1]->name == "bP") ||
				(x < 7 && Board::board[y + 2][x + 1] != NULL && Board::board[y + 2][x + 1]->name == "bP") //checking if pawn on right or left
				)//if this is the move made then update enPassant var
				getEnPassant = this;
		}

		if (y < 7 && Board::board[y + 1][x] == NULL && !Hpin && !LDpin && !RDpin)//single push l8r with promotion
			possibleMoves.insert(Board::toMove(x, y + 1));//check for promotion later

		if (y == 4 && !Vpin && !Hpin) {//enPassant baybee, need to check for both horizontal and vertical pins smfh

			Pieces* west = NULL;
			Pieces* east = NULL;
			if (x > 0)
				west = Board::board[y][x - 1];
			if (x < 7)
				east = Board::board[y][x + 1];

			if (west != NULL && west == getEnPassant && !RDpin)
			{
				possibleMoves.insert(Board::toMove(x - 1, y + 1));
				captureMoves.insert(Board::toMove(x - 1, y));
			}
			if (east != NULL && east == getEnPassant && !LDpin)
			{
				possibleMoves.insert(Board::toMove(x + 1, y + 1));
				captureMoves.insert(Board::toMove(x + 1, y));
			}

		}
	}
	else {
		if (x > 0 && y > 0 && Board::board[y - 1][x - 1] != NULL && Board::board[y - 1][x - 1]->isWhite() && !Vpin && !Hpin && !LDpin)//capture left
		{
			possibleMoves.insert(Board::toMove(x - 1, y - 1));
			captureMoves.insert(Board::toMove(x - 1, y - 1));
		}

		if (x < 7 && y > 0 && Board::board[y - 1][x + 1] != NULL && Board::board[y - 1][x + 1]->isWhite() && !Vpin && !Hpin && !RDpin)//capture right
		{
			possibleMoves.insert(Board::toMove(x + 1, y - 1));
			captureMoves.insert(Board::toMove(x + 1, y - 1));
		}

		if (this->y == 6 && Board::board[y - 2][x] == NULL && Board::board[y - 1][x] == NULL && !Hpin && !LDpin && !RDpin) {//Double push
			possibleMoves.insert(Board::toMove(x, y - 2));
			if (
				(x > 0 && Board::board[y - 2][x - 1] != NULL && Board::board[y - 2][x - 1]->name == "wP") ||
				(x < 7 && Board::board[y - 2][x + 1] != NULL && Board::board[y - 2][x + 1]->name == "wP")
				)//checking if pawn can be enpassanted
				getEnPassant = this;
		}

		if (y > 0 && Board::board[y - 1][x] == NULL && !Hpin && !LDpin && !RDpin)//single push
			possibleMoves.insert(Board::toMove(x, y - 1));//promotion logic exists so will make AI most likely only choose to promote queen ???

		if (y == 3 && !Hpin && !Vpin) {//lmao get enPassanted 
			Pieces* west = NULL;
			Pieces* east = NULL;
			if (x > 0)
				west = Board::board[y][x - 1];
			if (x < 7)
				east = Board::board[y][x + 1];;

			if (west != NULL && west == getEnPassant && !LDpin)
			{
				possibleMoves.insert(Board::toMove(x - 1, y - 1));
				captureMoves.insert(Board::toMove(x - 1, y));
			}
			if (east != NULL && east == getEnPassant && !RDpin)
			{
				possibleMoves.insert(Board::toMove(x + 1, y - 1));
				captureMoves.insert(Board::toMove(x + 1, y));
			}

		}
	}
	return possibleMoves;
}
Moves Pawn::BM() {
	pseudoLegalMoves();
	if (possibleMoves.empty()) {
		return Moves("NULL", -1);
	}
	for (string moves : possibleMoves) {
		int* pos = Board::decodePosition(moves);
		int attackX = pos[0], attackY = pos[1];
		if (this->isWhite())
			attackY += 1;
		else
			attackY -= 1;
		//if (Board::board[attackY][attackX]->value >= this->value)//pawn can capture other pawn
		if (prevY.size() > 1 && (
			abs(this->y - 7) == 1 || abs(this->y - 0) == 1))//prefer promotion move
			bestMoves.push(*(new Moves(moves, 500)));

		if (attackX < 7 && attackY < 7 && attackY > 0 && Board::board[attackY][attackX + 1] != NULL) { //pawn protect and attack right variation
			if (!Board::board[attackY][attackX + 1]->isOpponent(*this)) {
				int val = 10;
				if ((attackY == 4 || attackY == 5) && (attackX == 4 || attackX == 5))
					val += 5;
				bestMoves.push(*(new Moves(moves, val)));
			}
			else//attacking piece
				bestMoves.push(*(new Moves(moves, Board::board[attackY][attackX + 1]->value)));
		}

		if (attackX > 0 && attackY < 7 && attackY > 0 && Board::board[attackY][attackX - 1] != NULL) { //pawn protect and attack right variation
			if (!Board::board[attackY][attackX - 1]->isOpponent(*this)) {
				int val = 10;
				if ((attackY == 4 || attackY == 5) && (attackX == 4 || attackX == 5))
					val += 5;
				bestMoves.push(*(new Moves(moves, val)));
			}
			else//attacking piece
				bestMoves.push(*(new Moves(moves, Board::board[attackY][attackX - 1]->value)));
		}

		else
			bestMoves.push(*(new Moves(*possibleMoves.begin(), 6)));//so at least there is 1 element in priority queue
	}

	for (string moves : captureMoves) {
		int* pos = Board::decodePosition(moves);
		int attackX = pos[0], attackY = pos[1];
		bestMoves.push(*(new Moves(moves, (Board::board[attackY][attackX]->value / this->value) * 40)));
	}
	return bestMoves.top();
}

Knight::Knight(string pos, string color) :Pieces(pos, color + "N", 55) {}
set<string> Knight::pseudoLegalMoves() {
	resetMoves();
	//moving like an octopus smh
	//if horse pinned it cannot moved anywhere
	bool pin = horizontalPin() || verticalPin() || leftDiagonalPin() || rightDiagonalPin();
	if (pin)//return empty set as horse has no valid moves
		return possibleMoves;

	//--|
	if (this->x + 2 < 8 && this->y + 1 < 8 && (Board::board[this->y + 1][this->x + 2] == NULL || Board::board[this->y + 1][this->x + 2]->isOpponent(*this)))
	{
		possibleMoves.insert(Board::toMove(this->x + 2, this->y + 1));
		if (Board::board[this->y + 1][this->x + 2] != NULL && Board::board[this->y + 1][this->x + 2]->isOpponent(*this))
			captureMoves.insert(Board::toMove(this->x + 2, this->y + 1));
	}
	if (this->x + 2 < 8 && this->y - 1 >= 0 && (Board::board[this->y - 1][this->x + 2] == NULL || Board::board[this->y - 1][this->x + 2]->isOpponent(*this)))
	{
		possibleMoves.insert(Board::toMove(this->x + 2, this->y - 1));
		if (Board::board[this->y - 1][this->x + 2] != NULL && Board::board[this->y - 1][this->x + 2]->isOpponent(*this))
			captureMoves.insert(Board::toMove(this->x + 2, this->y - 1));

	}

	//|--
	if (this->x - 2 >= 0 && this->y + 1 < 8 && (Board::board[this->y + 1][this->x - 2] == NULL || Board::board[this->y + 1][this->x - 2]->isOpponent(*this)))
	{
		possibleMoves.insert(Board::toMove(this->x - 2, this->y + 1));
		if (Board::board[this->y + 1][this->x - 2] != NULL && Board::board[this->y + 1][this->x - 2]->isOpponent(*this))
			captureMoves.insert(Board::toMove(this->x - 2, this->y + 1));
	}
	if (this->x - 2 >= 0 && this->y - 1 >= 0 && (Board::board[this->y - 1][this->x - 2] == NULL || Board::board[this->y - 1][this->x - 2]->isOpponent(*this)))
	{
		possibleMoves.insert(Board::toMove(this->x - 2, this->y - 1));
		if (Board::board[this->y - 1][this->x - 2] != NULL && Board::board[this->y - 1][this->x - 2]->isOpponent(*this))
			captureMoves.insert(Board::toMove(this->x - 2, this->y - 1));
	}

	// |
	//-|
	// |
	if (this->x + 1 < 8 && this->y + 2 < 8 && (Board::board[this->y + 2][this->x + 1] == NULL || Board::board[this->y + 2][this->x + 1]->isOpponent(*this)))
	{
		possibleMoves.insert(Board::toMove(this->x + 1, this->y + 2));
		if (Board::board[this->y + 2][this->x + 1] != NULL && Board::board[this->y + 2][this->x + 1]->isOpponent(*this))
			captureMoves.insert(Board::toMove(this->x + 1, this->y + 2));
	}
	if (this->x + 1 < 8 && this->y - 2 >= 0 && (Board::board[this->y - 2][this->x + 1] == NULL || Board::board[this->y - 2][this->x + 1]->isOpponent(*this)))
	{
		possibleMoves.insert(Board::toMove(this->x + 1, this->y - 2));
		if (Board::board[this->y - 2][this->x + 1] != NULL && Board::board[this->y - 2][this->x + 1]->isOpponent(*this))
			captureMoves.insert(Board::toMove(this->x + 1, this->y - 2));
	}

	//|
	//|-
	//|
	if (this->x - 1 >= 0 && this->y + 2 < 8 && (Board::board[this->y + 2][this->x - 1] == NULL || Board::board[this->y + 2][this->x - 1]->isOpponent(*this)))
	{
		possibleMoves.insert(Board::toMove(this->x - 1, this->y + 2));
		if (Board::board[this->y + 2][this->x - 1] != NULL && Board::board[this->y + 2][this->x - 1]->isOpponent(*this))
			captureMoves.insert(Board::toMove(this->x - 1, this->y + 2));
	}
	if (this->x - 1 >= 0 && this->y - 2 >= 0 && (Board::board[this->y - 2][this->x - 1] == NULL || Board::board[this->y - 2][this->x - 1]->isOpponent(*this)))
	{
		possibleMoves.insert(Board::toMove(this->x - 1, this->y - 2));
		if (Board::board[this->y - 2][this->x - 1] != NULL && Board::board[this->y - 2][this->x - 1]->isOpponent(*this))
			captureMoves.insert(Board::toMove(this->x - 1, this->y - 2));
	}

	return possibleMoves;
}
Moves Knight::BM() {
	pseudoLegalMoves();
	if (possibleMoves.empty()) {
		return Moves("NULL", -1);
	}
	for (string moves : captureMoves) {
		int* pos = Board::decodePosition(moves);
		int attackX = pos[0], attackY = pos[1];
		bestMoves.push(*(new Moves(moves, (Board::board[attackY][attackX]->value / this->value) * 35)));
		possibleMoves.erase(possibleMoves.find(moves));
	}

	priority_queue<Moves> temp = bestMoves;//as bestMoves cleared due to reset function
	const set<string> copy = possibleMoves;
	for (string moves : copy) {
		move(moves);//future attacks
		pseudoLegalMoves();
		int val = 15;
		if (this->x == 0 || this->x == 7 || this->y == 0 || this->y == 7)//reducing chances of going to the rim
			val -= 15;
		temp.push(*(new Moves(moves, captureMoves.size() * 6 + val)));
		undoMove();
	}

	vector<Moves> v;
	for (int i = 0; i < 3 && !temp.empty(); i++) {
		if (v.size() > 0 && abs(v[0].value - temp.top().value) > 15)
			break;
		v.push_back(temp.top());
		temp.pop();
	}

	srand(unsigned(time(NULL)));
	int pos = rand() % v.size();
	return v[pos];
}

Bishop::Bishop(string pos, string color) :Pieces(pos, color + "B", 55) {}
int Bishop::rays(bool isNorth, bool isEast) {//lasers generator
	int skewer = 10;
	int dirX, dirY;
	if (isNorth)
		dirY = 1;
	else
		dirY = -1;

	if (isEast)
		dirX = 1;
	else
		dirX = -1;

	int i, j;
	for (i = this->y + dirY, j = this->x + dirX; i >= 0 && i < 8 && j < 8 && j >= 0; i += dirY, j += dirX)//se
	{
		if (Board::board[i][j] != NULL)
		{
			if (Board::board[i][j]->isOpponent(*this))
			{
				switch (Board::board[i][j]->name[1]) {
				case 'K':
					skewer += 60;
					break;
				case 'Q':
					skewer += 85;
					break;
				case 'R':
					skewer += 53;
					break;
				case 'B':
					skewer -= 30;
					break;
				case 'N':
					skewer += 35;
					break;

				case 'P':
					skewer -= 30;
					break;
				}
			}

			else
				skewer -= 60;

			if (abs(i - this->y) == 1 && abs(j - this->x) == 1 && Board::board[i][j]->isOpponent(*this)
				&& Board::board[i][j]->name[1] == 'K' || Board::board[i][j]->name[1] == 'Q')//if right next to the piece move not so good
				skewer -= 15;
		}
	}

	return skewer;
}
set<string> Bishop::pseudoLegalMoves() {
	bool Hpin = horizontalPin();
	bool Vpin = verticalPin();
	bool LDpin = leftDiagonalPin();
	bool RDpin = rightDiagonalPin();

	int i = 0; int j = 0;
	resetMoves();

	//if bishop binned vertically or horizontally then it cannot move
	if (Hpin || Vpin)
		return possibleMoves;

	for (i = this->y - 1, j = this->x - 1; i >= 0 && j >= 0 && !LDpin; i--, j--) {//sw
		if (Board::board[i][j] != NULL)
		{
			if (Board::board[i][j]->isOpponent(*this))
			{
				possibleMoves.insert(Board::toMove(j, i));//adding the possible capture/attacked piece
				captureMoves.insert(Board::toMove(j, i));//adding the possible capture/attacked piece
			}
			break;
		}
		possibleMoves.insert(Board::toMove(j, i));
	}

	for (i = this->y - 1, j = this->x + 1; i >= 0 && j < 8 && !RDpin; i--, j++)//se
	{
		if (Board::board[i][j] != NULL)
		{
			if (Board::board[i][j]->isOpponent(*this))
			{
				possibleMoves.insert(Board::toMove(j, i));//adding the possible capture/attacked piece
				captureMoves.insert(Board::toMove(j, i));//adding the possible capture/attacked piece
			}
			break;
		}
		possibleMoves.insert(Board::toMove(j, i));

	}

	for (i = this->y + 1, j = this->x - 1; i < 8 && j >= 0 && !RDpin; i++, j--)//nw
	{
		if (Board::board[i][j] != NULL)
		{
			if (Board::board[i][j]->isOpponent(*this))
			{
				possibleMoves.insert(Board::toMove(j, i));//adding the possible capture/attacked piece
				captureMoves.insert(Board::toMove(j, i));//adding the possible capture/attacked piece
			}
			break;
		}
		possibleMoves.insert(Board::toMove(j, i));

	}

	for (i = this->y + 1, j = this->x + 1; i < 8 && j < 8 && !LDpin; i++, j++)//ne
	{
		if (Board::board[i][j] != NULL)
		{
			if (Board::board[i][j]->isOpponent(*this))
			{
				possibleMoves.insert(Board::toMove(j, i));//adding the possible capture/attacked piece
				captureMoves.insert(Board::toMove(j, i));//adding the possible capture/attacked piece
			}
			break;
		}
		possibleMoves.insert(Board::toMove(j, i));

	}

	return possibleMoves;
}
Moves Bishop::BM() {
	pseudoLegalMoves();
	if (possibleMoves.empty()) {
		return Moves("NULL", -1);
	}
	for (string moves : captureMoves) {
		int* pos = Board::decodePosition(moves);
		int attackX = pos[0], attackY = pos[1];
		bestMoves.push(*(new Moves(moves, (Board::board[attackY][attackX]->value / this->value) * 35)));
		possibleMoves.erase(possibleMoves.find(moves));
	}
	const set<string> copy = possibleMoves;
	for (string moves : copy) { //laser moves --- to pin pieces
		move(moves);//future attacks
		bestMoves.push(*(new Moves(moves, (rays(0, 0) + rays(0, 1) + rays(1, 0) + rays(1, 1)) / 3)));//average attack of diagonal
		undoMove();
	}
	vector<Moves> v;
	for (int i = 0; i < 3 && !bestMoves.empty(); i++) {
		if (v.size() > 0 && abs(v[0].value - bestMoves.top().value) > 15)
			break;
		v.push_back(bestMoves.top());
		bestMoves.pop();
	}

	srand(unsigned(time(NULL)));
	int pos = rand() % v.size();
	return v[pos];

}

Rook::Rook(string pos, string color) :Pieces(pos, color + "R", 70) {}
int Rook::lasers(bool isVertical, bool increment) {//lasers generator
	int skewer = 10;
	int dirX, dirY;
	if (isVertical)
	{
		dirX = 0;
		dirY = pow(-1, increment + 1);
	}
	else
	{
		dirY = 0;
		dirX = pow(-1, increment + 1);
	}


	int i, j;
	for (i = this->y + dirY, j = this->x + dirX; i >= 0 && i < 8 && j < 8 && j >= 0; i += dirY, j += dirX)//se
	{
		if (Board::board[i][j] != NULL)
		{
			if (Board::board[i][j]->isOpponent(*this))
			{
				switch (Board::board[i][j]->name[1]) {
				case 'K':
					skewer += 80;
					break;
				case 'Q':
					skewer += 65;
					break;
				case 'R':
					skewer -= 60;
					break;
				case 'B':
					skewer += 55;
					break;
				case 'N':
					skewer += 55;
					break;

				case 'P':
					skewer += 5;
					break;
				}
			}

			else
				skewer -= 60;

			if (abs(i - this->y) <= 1 && abs(j - this->x) <= 1 && Board::board[i][j]->isOpponent(*this)
				&& Board::board[i][j]->name[1] == 'K' || Board::board[i][j]->name[1] == 'Q')
				skewer -= 15;
		}

		else {
			if (i < 7 && j < 7 && i > 0 && j > 0) {
				float mul = 0;
				if (i < 6 && j < 6 && i > 1 && j > 1)
					mul = 1;
				if (Board::board[i + 1][j + 1] == NULL)
					mul += 0.5;
				if (Board::board[i + 1][j - 1] == NULL)
					mul += 0.5;
				if (Board::board[i - 1][j + 1] == NULL)
					mul += 0.5;;
				if (Board::board[i - 1][j - 1] == NULL)
					mul += 0.5;;
				if (Board::board[i][j + 1] == NULL)
					mul += 0.5;;
				if (Board::board[i][j - 1] == NULL)
					mul += 0.5;;
				if (Board::board[i + 1][j] == NULL)
					mul += 0.5;;
				if (Board::board[i - 1][j] == NULL)
					mul += 0.5;
				skewer += (int)mul;
			}

		}
	}
	return skewer;
}
set<string> Rook::pseudoLegalMoves() {
	bool Hpin = horizontalPin();
	bool Vpin = verticalPin();
	bool LDpin = leftDiagonalPin();
	bool RDpin = rightDiagonalPin();

	resetMoves();

	if (LDpin || RDpin)
		return possibleMoves;

	for (int i = this->y - 1; i >= 0 && !Hpin; i--)//north
	{
		if (Board::board[i][this->x] != NULL)
		{
			if (Board::board[i][this->x]->isOpponent(*this))
			{
				possibleMoves.insert(Board::toMove(this->x, i));//adding the possible capture/attacked piece
				captureMoves.insert(Board::toMove(this->x, i));//adding the possible capture/attacked piece
			}
			break;
		}
		possibleMoves.insert(Board::toMove(this->x, i));
	}

	for (int i = this->y + 1; i < 8 && !Hpin; i++)//south
	{
		if (Board::board[i][this->x] != NULL)//going till it is blocked
		{
			if (Board::board[i][this->x]->isOpponent(*this))
			{
				possibleMoves.insert(Board::toMove(this->x, i));//adding the possible capture/attacked piece
				captureMoves.insert(Board::toMove(this->x, i));//adding the possible capture/attacked piece
			}
			break;
		}
		possibleMoves.insert(Board::toMove(this->x, i));
	}


	for (int j = this->x - 1; j >= 0 && !Vpin; j--)//west
	{
		if (Board::board[this->y][j] != NULL)
		{
			if (Board::board[this->y][j]->isOpponent(*this))
			{
				possibleMoves.insert(Board::toMove(j, this->y));
				captureMoves.insert(Board::toMove(j, this->y));//adding the possible capture/attacked piece
			}
			break;
		}
		possibleMoves.insert(Board::toMove(j, this->y));
	}

	for (int j = this->x + 1; j < 8 && !Vpin; j++)//east
	{
		if (Board::board[this->y][j] != NULL)
		{
			if (Board::board[this->y][j]->isOpponent(*this))
			{
				possibleMoves.insert(Board::toMove(j, this->y));
				captureMoves.insert(Board::toMove(j, this->y));//adding the possible capture/attacked piece
			}
			break;
		}

		possibleMoves.insert(Board::toMove(j, this->y));
	}

	return possibleMoves;
}
Moves Rook::BM() {
	pseudoLegalMoves();
	if (possibleMoves.empty()) {
		return Moves("NULL", -1);
	}
	for (string moves : captureMoves) {
		int* pos = Board::decodePosition(moves);
		int attackX = pos[0], attackY = pos[1];
		bestMoves.push(*(new Moves(moves, (Board::board[attackY][attackX]->value / this->value) * 35)));
		possibleMoves.erase(possibleMoves.find(moves));
	}
	const set<string> copy = possibleMoves;
	for (string moves : copy) { //laser moves --- to pin pieces
		move(moves);//future attacks
		bestMoves.push(*(new Moves(moves, (lasers(0, 0) + lasers(0, 1) + lasers(1, 0) + lasers(1, 1)) / 3)));//average attack of diagonal
		undoMove();
	}

	vector<Moves> v;
	for (int i = 0; i < 3 && !bestMoves.empty(); i++) {
		if (v.size() > 0 && abs(v[0].value - bestMoves.top().value) > 15)
			break;
		v.push_back(bestMoves.top());
		bestMoves.pop();
	}

	srand(unsigned(time(NULL)));
	int pos = rand() % v.size();
	return v[pos];
}

Queen::Queen(string pos, string color) :Pieces(pos, color + "Q", 100) {}
set<string> Queen::pseudoLegalMoves() {
	resetMoves();
	Bishop bisMoves = *((Bishop*)this);
	Rook rookMoves = *((Rook*)this);

	set<string> diagonals = bisMoves.pseudoLegalMoves();//downcasting piece as bishop and storing its possible moves
	possibleMoves = rookMoves.pseudoLegalMoves();//downcasting piece as rook and storing its possible moves
	set<string> diagonalCapture = bisMoves.captureMoves;
	captureMoves = rookMoves.captureMoves;

	//both above functions clear the possibleMoves list by default so hence need to save in seperate variables

	for (string potentialPos : diagonals)
		possibleMoves.insert(potentialPos);
	for (string potentialCapture : diagonalCapture)
		captureMoves.insert(potentialCapture);
	return possibleMoves;
}
Moves Queen::BM() {
	pseudoLegalMoves();
	if (possibleMoves.empty()) {
		return Moves("NULL", -1);
	}
	Bishop tempB = *((Bishop*)this);
	Rook tempR = *((Rook*)this);

	bestMoves.push(tempB.BM());
	bestMoves.push(tempR.BM());
	//bestMoves.push(*(new Moves(*possibleMoves.begin(), 0)));

	Board::board[this->y][this->x] = this;
	return bestMoves.top();
}

bool Pieces::isProtected() {//mainly for king capturing opposing/checking pieces
	//diagonals
	//chaging color to get pieces of same color using capture space
	bool safe = false;
	if (this->isWhite())
		this->name[0] = 'b';
	else
		this->name[0] = 'w';

	Queen capturedThreat = *((Queen*)this);
	capturedThreat.pseudoLegalMoves();
	//capturedThreat->pseudoLegalMoves();//generating captures for if the piece was the opposite color queen
	/*if (captureMoves.empty())
		return false;
	//cout << "Protected by: ";
	*/
	for (string pos : capturedThreat.captureMoves) {//ensure that only same color pieces in this list TESTING REQUIRED
		int* move = Board::decodePosition(pos);
		string pieceName = Board::board[move[1]][move[0]]->name;
		if ((move[1] == this->y || move[0] == this->x) &&//Protected vertically or horizontally
			(pieceName[1] == 'R' ||
				pieceName[1] == 'Q'))
			safe = true;

		else if (pieceName[1] == 'B' || pieceName[1] == 'Q')//otherwise protected by long diagonal
			safe = true;

		else if (pieceName[1] == 'P') {//pawn protection
			if (pieceName[0] == 'w' &&
				move[1] == this->y - 1 && (move[0] == this->x - 1 || move[0] == this->x + 1))
				safe = true;
			else if (move[1] == this->y + 1 && (move[0] == this->x - 1 || move[0] == this->x + 1))
				safe = true;
		}

		else if (pieceName[1] == 'K')//King protection
		{
			if ((move[0] == this->x - 1 || move[0] == this->x || move[0] == this->x + 1) &&
				(move[1] == this->y - 1 || move[1] == this->y || move[0] == this->y + 1))
				safe = true;
		}
	}

	Knight _capturedLThreat = *((Knight*)this);
	_capturedLThreat.pseudoLegalMoves();
	for (string captures : _capturedLThreat.captureMoves) {
		int* Lpos = Board::decodePosition(captures);
		if (Board::board[Lpos[1]][Lpos[0]]->name[1] == 'N')//capture only gives us same color pieces as the protected piece on the L position
			safe = true;
	}

	//resetting kings color
	if (this->isWhite())
		this->name[0] = 'b';
	else
		this->name[0] = 'w';

	//cout << endl;
	return safe;
}

King::King(string pos, string color) :Pieces(pos, color + "K", 999), opponent(NULL) {}
bool King::isChecked() {
	list<string> checked = checkedList();
	return !checked.empty();
}
list<string> King::checkedList() {//logic for checking if piece checked
	checks.clear();
	Rook LinearThreats = *((Rook*)this);
	Bishop DiagonalThreats = *((Bishop*)this);
	Knight LThreats = *((Knight*)this);
	Pawn pawnThreats = *((Pawn*)this);

	LinearThreats.pseudoLegalMoves();//this covers all horizontal and vertical threats the king may face
	LThreats.pseudoLegalMoves();//this covers Knight threats
	DiagonalThreats.pseudoLegalMoves();//covers diagonals
	pawnThreats.pseudoLegalMoves();//covers smaller siagonal with a pawn

	//merging all the threats
	for (string threat : LinearThreats.captureMoves) {
		int* pos = Board::decodePosition(threat);
		Pieces* opPiece = Board::board[pos[1]][pos[0]];
		if (opPiece->isOpponent(*this) &&//linear check
			(opPiece->name[1] == 'R' ||
				opPiece->name[1] == 'Q')
			)
			checks.push_back(threat);
	}

	for (string threat : DiagonalThreats.captureMoves) {
		int* pos = Board::decodePosition(threat);
		Pieces* opPiece = Board::board[pos[1]][pos[0]];
		if (opPiece->isOpponent(*this) &&//diagonal check
			(opPiece->name[1] == 'B' ||
				opPiece->name[1] == 'Q')
			)
			checks.push_back(threat);
	}

	for (string threat : pawnThreats.captureMoves) {
		int* pos = Board::decodePosition(threat);
		Pieces* opPiece = Board::board[pos[1]][pos[0]];
		if (opPiece->isOpponent(*this) && opPiece->name[1] == 'P') //Checking for Pawn
			checks.push_back(threat);
	}

	for (string threat : LThreats.captureMoves) {
		int* pos = Board::decodePosition(threat);
		Pieces* opPiece = Board::board[pos[1]][pos[0]];
		if (opPiece->isOpponent(*this) && opPiece->name[1] == 'N') //Checking for Knight 
			checks.push_back(threat);
	}

	for (string threat : checks) {
		cout << threat << " ";
	}
	cout << endl;
	return checks;

}
set<string> King::pseudoLegalMoves() {
	resetMoves();
	bool castlingFlag = false;
	castling = 0;
	//N
	if (y > 0 && (Board::board[this->y - 1][this->x] == NULL || Board::board[this->y - 1][this->x]->isOpponent(*this)))
		possibleMoves.insert(Board::toMove(this->x, this->y - 1));
	//S
	if (y < 7 && (Board::board[this->y + 1][this->x] == NULL || Board::board[this->y + 1][this->x]->isOpponent(*this)))
		possibleMoves.insert(Board::toMove(this->x, this->y + 1));
	//E
	if (x < 7 && (Board::board[this->y][this->x + 1] == NULL || Board::board[this->y][this->x + 1]->isOpponent(*this)))
		possibleMoves.insert(Board::toMove(this->x + 1, this->y));
	//W
	if (x > 0 && (Board::board[this->y][this->x - 1] == NULL || Board::board[this->y][this->x - 1]->isOpponent(*this)))
		possibleMoves.insert(Board::toMove(this->x - 1, this->y));
	//NW
	if (y > 0 && x > 0 && (Board::board[this->y - 1][this->x - 1] == NULL || Board::board[this->y - 1][this->x - 1]->isOpponent(*this)))
		possibleMoves.insert(Board::toMove(this->x - 1, this->y - 1));
	//SW
	if (y < 7 && x > 0 && (Board::board[this->y + 1][this->x - 1] == NULL || Board::board[this->y + 1][this->x - 1]->isOpponent(*this)))
		possibleMoves.insert(Board::toMove(this->x - 1, this->y + 1));
	//SE
	if (y < 7 && x < 7 && (Board::board[this->y + 1][this->x + 1] == NULL || Board::board[this->y + 1][this->x + 1]->isOpponent(*this)))
		possibleMoves.insert(Board::toMove(this->x + 1, this->y + 1));
	//NE
	if (y > 0 && x < 7 && (Board::board[this->y - 1][this->x + 1] == NULL || Board::board[this->y - 1][this->x + 1]->isOpponent(*this)))
		possibleMoves.insert(Board::toMove(this->x + 1, this->y - 1));

	//checking for castling
	// add color of rook as well just to prevent any bugs in custom positions
	//Queen side castling hence west
	if (Board::board[this->y][0] != NULL &&//checking if piece in that location exists
		Board::board[this->y][0]->name[1] == 'R' && Board::board[this->y][0]->prevX.size() == 1 &&//the square has same coloured unmoved Rook
		Board::board[this->y][1] == NULL && Board::board[this->y][2] == NULL && Board::board[this->y][3] == NULL &&//empty spaces between left rook and king, filter out by legal moves
		this->prevX.size() == 1 && this->x == 4 && !isChecked())//king has not moved from its own position
	{
		possibleMoves.insert(Board::toMove(this->x - 2, this->y));//Castling possible, write castling logic below
		castlingFlag = true;
	}

	//King side castling hence east
	if (Board::board[this->y][7] != NULL &&//checking if piece in that location exists
		Board::board[this->y][7]->name[1] == 'R' && Board::board[this->y][7]->prevX.size() == 1 &&//the square has same coloured unmoved Rook
		Board::board[this->y][6] == NULL && //automatically gets catered
		this->prevX.size() == 1 && this->x == 4 && !isChecked())//king has not moved from its own position
	{
		possibleMoves.insert(Board::toMove(this->x + 2, this->y));//Castling possible, write castling logic below
		castlingFlag = true;
	}

	if (kingToMove != this->name)
		return possibleMoves;

	if (kingToMove == this->name)//filtering so king only has legal moves.
	{
		legalMoves();
		set<string> tempCopy(possibleMoves);
		for (string moves : possibleMoves) {
			int* coord = Board::decodePosition(moves);
			if (Board::board[coord[1]][coord[0]] != NULL && Board::board[coord[1]][coord[0]]->isOpponent(*this))
				if (!Board::board[coord[1]][coord[0]]->isProtected())
					captureMoves.insert(moves);
				else
					tempCopy.erase(tempCopy.find(moves));
		}
		possibleMoves = tempCopy;
	}

	//castling checks
	if (castlingFlag && kingToMove == this->name) {//need to revamp logic here as this should only generate moves not do them
		if (possibleMoves.find(Board::toMove(5, this->y)) != possibleMoves.end() && possibleMoves.find(Board::toMove(6, this->y)) != possibleMoves.end())//king side castling only possible if no rays at the kings final and initial position
			castling = 1;
		//Board::board[this->y][7]->move(Board::toMove(5, this->y));//put this logic somewhere else
		else {
			set<string>::iterator cond2 = possibleMoves.find(Board::toMove(6, this->y));
			if (cond2 != possibleMoves.end())
				possibleMoves.erase(cond2);
		}
		if (possibleMoves.find(Board::toMove(3, this->y)) != possibleMoves.end() && possibleMoves.find(Board::toMove(2, this->y)) != possibleMoves.end())//same as above
			castling = 1;
		//Board::board[this->y][0]->move(Board::toMove(3, this->y));

		else {//otherwise removing the moves from the legal moves entirely
			set<string>::iterator cond1 = possibleMoves.find(Board::toMove(2, this->y));
			if (cond1 != possibleMoves.end())
				possibleMoves.erase(cond1);
		}
	}

	return possibleMoves;
}//change return to legalMoves function
Moves King::BM() {
	pseudoLegalMoves();
	if (possibleMoves.empty()) {
		return Moves("NULL", -1);
	}
	for (string moves : possibleMoves) {
		int* pos = Board::decodePosition(moves);
		int attackX = pos[0], attackY = pos[1];
		//if (Board::board[attackY][attackX]->value >= this->value)//pawn can capture other pawn
		if (prevY.size() == 1 && (find(castlingMoves, castlingMoves + 4, moves) != castlingMoves + 4))//prefer castling at first possible moment
		{
			bestMoves.push(*(new Moves(moves, 50)));
			continue;
		}

		if (//if in the open try to take cover
			(attackX < 7 && Board::board[attackY][attackX + 1] != NULL && !Board::board[attackY][attackX + 1]->isOpponent(*this)) || //protect fellow pieces
			(attackX > 0 && Board::board[attackY][attackX - 1] != NULL && !Board::board[attackY][attackX - 1]->isOpponent(*this))) {
			int val = 6;
			if ((this->isWhite() && (attackY == 0 || attackY == 1)) ||
				(this->name[0] == 'b' && (attackY == 6 || attackY == 7)))
				val += 6;
			bestMoves.push(*(new Moves(moves, val)));
		}

		else
			bestMoves.push(*(new Moves(*possibleMoves.begin(), 0)));
	}
	for (string moves : captureMoves) {
		int* pos = Board::decodePosition(moves);
		int attackX = pos[0], attackY = pos[1];
		bestMoves.push(*(new Moves(moves, Board::board[attackY][attackX]->value)));
	}

	//adding slight randomness
	vector<Moves> v;
	for (int i = 0; i < 3 && !bestMoves.empty(); i++) {
		v.push_back(bestMoves.top());
		bestMoves.pop();
	}

	srand(unsigned(time(NULL)));
	int pos = rand() % v.size();
	return v[pos];
}
