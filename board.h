#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include <iostream>

//AI turn undefined
class Pieces;
namespace Board {
	extern std::vector< std::vector<Pieces*> > board; //trying to define a global variable

	void displayBoard();
	//Used to decode the position of the spaces from string to integers
	int* decodePosition(std::string pos);

	std::string toMove(int x, int y);

	//Input validation function
	bool isvalid(bool isWhite, std::string space);

	//This function checks if the user's input is validated
	bool validPosition(std::string Space);
}
#endif 
