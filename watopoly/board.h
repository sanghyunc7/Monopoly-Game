#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <string>
using namespace std;

class Building;

//class Board used to print out the board
class Board {

	/////////////////
	//private fields
	/////////////////
	const int width = 89;   //width of board
	const int win = 71;     //inside width of board
	const int wbox = 7;     //inside width of one box
	const int bspace = 12;  //border space around watopoly logo
	const int inspace = 45; //size of watologo 
	
	//list of the indexes of properties, used only as membership check
	const vector<int> propertyIndex = { 1, 3, 6, 8, 9, 11,
									   13, 14, 16, 18, 19,
									   21, 23, 24, 26, 27,
									   29, 31, 32, 34, 37,
									   39 };
	vector<shared_ptr<Building>> buildings;  //buildings
	
	///////////////////////////////
	//helper functions for printing
	///////////////////////////////

	//printPropertyUG(index) prints the upgrades for the property at index
	string printPropertyUG(int index);

	//printPropertyName(index) prints the name of the property at index
	// and if there are 8 players on the tile, prints the last player on index
	string printPropertyName(int index);

	//printPlayersOnProperty(index) prints the pieces of the players on
	//  the building at index
	string printPlayersOnBuilding(int index);

	//printSelectedName(name) prints the name in propert output format
	string printSelectedName(string name);
	
	//printNonPropertyName(index) prints the name of the building at index
	string printNonPropertyName(int index);

	//printLastPlayerOnNP(index) prints the last player on the nonproperty at index
	string printLastPlayerOnNP(int index);

	//printMidLineGap() prints the bottom line for a building
	void printMidLineGap();

	//printEndLineGap(c) prints the bottom line for a building, with inside space
	//   filled with c
	void printEndLineGap(char c);

	//printBoardTop() prints the top row of the board
	void printBoardTop();

	//printBoardGenSide(left, right, c) prints properties left and right from board
	void printBoardGenSide(int left, int right, char c);
	
	//printBoardSLCs(left, right) prints the two SLCs that are across from eachother
	void printBoardSLCs(int left, int right);

	//printBoardWatoTop(left, right) prints the buildings left, right, and the top
	//   of the Watoploy logo
	void printBoardWatoTop(int left, int right);

	//printBoardWatoSide(left, right) prints the buildings left, right, and the body
	//   of the Watoploy logo
	void printBoardWatoSide(int left, int right);

	//printBoardWatoBottom(left, right) prints the buildings left, right, and the bottom
    //of the Watoploy logo
	void printBoardWatoBottom(int left, int right);

	//printBoardPACCOOP(left, right) prints PAC and COOPFEE
	void printBoardPACCOOP(int left, int right);

	//printBoardBottom() prints the bottom of the board
	void printBoardBottom();

public:
	//constructor
	Board(vector<shared_ptr<Building>> buildings);

	//getBuildings() returns the buildings of board
	vector<shared_ptr<Building>> getBuildings();

	//printBoard() prints the board
	void printBoard();
};


