#include "timsCup.h"

//set static int to 0
int TimsCup::cupsActive = 0;

//see timsCups.h for details
void TimsCup::resetCount() {
	cupsActive = 0;
}

//see timsCups.h for details
int TimsCup::getCupsActive() {
	return cupsActive;
}

//see timsCups.h for details
void TimsCup::addCup() {
	if (cupsActive < 4) {
		++cupsActive;
	}
	else {
		cout << "There are already " << cupsActive;
		cout << " Roll-Up-the-Rim Cups active" << endl;
	}
}

//see timsCups.h for details
void TimsCup::subCups(int cups) {
	if (cups <= cupsActive) {
		cupsActive -= cups;
	}
	else {
		cout << "Cannot subtract " << cups << " cups from " << cupsActive << " active cups"<< endl;
	}
}
