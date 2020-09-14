#ifndef __DICE_H__
#define __DICE_H__

#include <iostream>
#include <string>
using namespace std;

//Rolls stores the numbers rolled
struct Rolls {
	int dice1;
	int dice2;
};

//class Dice rolls 2 numbers
class Dice {
	Rolls savedRoll;  // previous roll
	int savedTotal;   //previous total
	Rolls curRoll;    // current roll
	int curTotal;     // current total
	int numDoubles;   //number of doubles rolled
public:
	
	//constructor
	Dice();

	//setSavedRoll() returns the previous Roll
	Rolls getSavedRoll();

	//getSavedTotal() returns the previous total
	int getSavedTotal();

	//getCurRoll() returns the current roll
	Rolls getCurRoll();

	//getCurTotal() returns the current total
	int getCurTotal();

	//roll() rolls the dice
	int roll();

	//roll(int, int) rolls as if first and second where the numbers rolled
	int roll(int first, int second);

	//getNumDoubles() returns the number of doubles rolled
	int getNumDoubles();

	//setNumDoubles(n) sets the number of doubles rolled to n
	void setNumDoubles(int n);
};

#endif
