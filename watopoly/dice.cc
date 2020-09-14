#include "dice.h"
#include <cstdlib>
#include <ctime>

//constructor
Dice::Dice()
	: savedRoll{ Rolls{0,0} }, savedTotal{ 0 }, curRoll{ Rolls{0,0} },
	curTotal{ 0 }, numDoubles{ 0 } {}

//see dice.h for details
Rolls Dice::getSavedRoll() {
	return savedRoll;
}

//see dice.h for details
int Dice::getSavedTotal() {
	return savedTotal;
}

//see dice.h for details
Rolls Dice::getCurRoll() {
	return curRoll;
}

//see dice.h for details
int Dice::getCurTotal() {
	return curTotal;
}

//see dice.h for details
int Dice::roll() {
	//randomly generate int between 1 and 6 inclusive
	int d1 = rand() % 6 + 1;
	int d2 = rand() % 6 + 1;

	if (d1 == d2) {
		++numDoubles;
	}

	Rolls c{ d1, d2 };
	savedRoll = curRoll;
	savedTotal = curTotal;
	curRoll = c;
	curTotal = d1 + d2;
	return curTotal;
}

//see dice.h for details
int Dice::roll(int first, int second) {

	if (first == second) {
		++numDoubles;
	}

	Rolls c{ first, second };
	savedRoll = curRoll;
	savedTotal = curTotal;
	curRoll = c;
	curTotal = first + second;
	return curTotal;
}

//see dice.h for details
int Dice::getNumDoubles() {
	return numDoubles;
}

//see dice.h for details
void Dice::setNumDoubles(int n) {
	numDoubles = n;
}
