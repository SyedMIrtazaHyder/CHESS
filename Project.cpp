#include "board.h"
#include "pieces.h"
#include "player.h"
using namespace std;

int main() {
	srand(unsigned(time(NULL)));

	while (true)
	{
		//Clearing the board for the next game
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				Board::board[i][j] = NULL;

		//Clearing the screen of clutter
		system("CLS");

		int choice = 0;
		cout << "\t\t         CHESS" << endl;
		cout << "\t\tWhat kind of game would you like to play?" << endl;
		cout << "\t\t1. vs Bot" << endl << "\t\t2. vs Player" << endl << "\t\t3. Bot Vs Bot" << endl << "\t\t4. Quit" << endl;
		cout << "\t\tYour choice: ";
		while (choice < 1 || choice > 4)
			cin >> choice;

		switch (choice)
		{
		case 1:
			vsPlayerGame(false, true);
			break;
		case 2:
			vsPlayerGame(false, false);
			break;
		case 3:
			vsPlayerGame(true, true);
			break;
		case 4:
			break;
		}

		if (choice == 4)
			break;
	}
	return 0;
}
