#ifndef __TIMS_H__
#define __TIMS_H__

#include <iostream>
using namespace std;

//class TimsCups keeps track of active TimsCups
class TimsCup {
	static int cupsActive;  //number of active tim cups
public:

	//resetCount() resets the active count to 0
	void resetCount();

	//getCupsActive() returns the number of active cups
	int getCupsActive(); 

	//addCup() adds another active cup
	void addCup();

	//subCups(cups) removes <cups> number of cups from the active count
	void subCups(int cups);

};

#endif
