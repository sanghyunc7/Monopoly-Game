#include "board.h"
#include "property.h"
#include "player.h"
#include <algorithm>

/////////////////////////////////////////////////////
//helper methods
/////////////////////////////////////////////////////

//printPropertyUG(index)
//see board.h for details
string Board::printPropertyUG(int index) {
	string retval = "|";
	shared_ptr<Property> p = dynamic_pointer_cast<Property> (buildings[index]);

	retval += string(p->getUpgrade(), 'I') +
		string((wbox - 1) - p->getUpgrade(), ' ');

	//check if property is mortgaged
	if (p->isMort()) {
		retval += 'm';
	}
	else {
		retval += ' ';
	}
	return retval + "|";
}

//printPropertyName(index)
//see board.h for details
string Board::printPropertyName(int index) {
	string retval = "|";
	int np = buildings[index]->getNumPlayers();
	string name = buildings[index]->getName();
	retval += name;

	//if > 7 players, print last players as well
	if (np > wbox) {
		retval += string(wbox - name.length() - (np - wbox), ' ');
		for (int j = wbox; j < np; ++j) {
			retval += buildings[index]->getPlayersOnTile()[j]->getPiece();
		}
	}
	else {
		retval += string(wbox - name.length(), ' ');
	}
	return retval;
}

//printPlayersOnBuilding(index)
//see board.h for details
string Board::printPlayersOnBuilding(int index) {
	string retval = "";
	int np = buildings[index]->getNumPlayers();
	if (np > wbox) {
		for (int j = 0; j < wbox; ++j) {
			retval += buildings[index]->getPlayersOnTile()[j]->getPiece();
		}
	}
	else {
		for (int j = 0; j < np; ++j) {
			retval += buildings[index]->getPlayersOnTile()[j]->getPiece();
		}
		retval += string(wbox - np, ' ');
	}
	return retval;
}

//printSelectedName(name)
//see board.h for details
string Board::printSelectedName(string name) {
	return "|" + name + string(wbox - name.length(), ' ') + "|";
}

//printNonPropertyName(index)
//see board.h for details
string Board::printNonPropertyName(int index) {
	string retval = "|";
	string name = buildings[index]->getName();
	retval += name;
	retval += string(wbox - name.length(), ' ');
	retval += "|";
	return retval;
}

//printLastPlayerOnNP(index)
//see board.h for details
string Board::printLastPlayerOnNP(int index) {
	string retval = "";
	int np = buildings[index]->getNumPlayers();

	//if there are >7 players, print last ones
	if (np > wbox) {
		retval += string(wbox - (np - wbox), ' ');
		for (int j = wbox; j < np; ++j) {
			retval += buildings[index]->getPlayersOnTile()[j]->getPiece();
		}
	}
	//if there are <= 7, print blank space
	else {
		retval += string(wbox, ' ');
	}
	return retval;
}

//printMidLineGap()
//see board.h for details
void Board::printMidLineGap() {
	cout << "|" << string(wbox, '-') << "|" << string(win, ' ');
	cout << "|" << string(wbox, '-') << "|" << endl;
}

//printEndLineGap()
//see board.h for details
void Board::printEndLineGap(char c) {
	cout << "|" << string(wbox, '_') << "|" << string(win, c);
	cout << "|" << string(wbox, '_') << "|" << endl;
}

//printBoardTop()
//see board.h for detail
void Board::printBoardTop() {
	//building indexes
	const int goose = 20;
	const int needles = 22;
	const int v1 = 25;
	const int cif = 28;
	const int gttims = 30;
	//prints 6 lines of output
	int start = 20;
	int pastEnd = 31;

	//line1
	cout << string(width, '_') << endl;

	//line2, names or upgrade level
	for (int i = start; i < pastEnd; ++i) {
		cout << "|";
		auto it = find(propertyIndex.begin(), propertyIndex.end(), i);
		if (it != propertyIndex.end()) {  //see if buildings[i] is property
			shared_ptr<Property> p = dynamic_pointer_cast<Property> (buildings[i]);
			cout << string(p->getUpgrade(), 'I') +
				string((wbox - 1) - p->getUpgrade(), ' ');
			if (p->isMort()) {  //check if mortgaged
				cout << "m";
			}
			else {
				cout << " ";
			}
		}
		else { //print name of non-property
			switch (i) {

			case goose:
				cout << "Goose  ";
				continue;
			case needles:
				cout << "NEEDLES";
				continue;
			case v1:
				cout << "V1     ";
				continue;
			case cif:
				cout << "CIF    ";
				continue;
			case gttims:
				cout << "GO TO  |" << endl;
				continue;
			}
		}
	}

	//line3, name or hor.line
	for (int i = start; i < pastEnd; ++i) {
		cout << "|";
		auto it = find(propertyIndex.begin(), propertyIndex.end(), i);
		if (it != propertyIndex.end()) {
			cout << string(wbox, '-');
		}
		else { //print second half of name
			switch (i) {

			case goose:
				cout << "Nesting";
				continue;
			case needles:
				cout << "HALL   ";
				continue;
			case v1:
				cout << "       ";
				continue;
			case cif:
				cout << "       ";
				continue;
			case gttims:
				cout << "TIMS   |" << endl;
				continue;
			}
		}
	}

	//line4, property name and last players
	for (int i = start; i < pastEnd; ++i) {  //if property, print name
		auto it = find(propertyIndex.begin(), propertyIndex.end(), i);
		if (it != propertyIndex.end()) {
			cout << printPropertyName(i);  //print last player on property			
		}
		else {
			cout << "|" + printLastPlayerOnNP(i);  //print last player on np
		}
	}
	cout << "|" << endl;

	//line5, print rest of players
	for (int i = start; i < pastEnd; ++i) {
		cout << "|" + printPlayersOnBuilding(i);
	}

	cout << "|" << endl;

	//line6, end of box
	for (int i = 0; i < width; ++i) {
		if (i % (wbox + 1) == 0) {
			cout << "|";
		}
		else {
			cout << "_";
		}
	}

	cout << endl;
}


//printBoardGenSide(left, right, c)
//see board.h for details
void Board::printBoardGenSide(int left, int right, char c) {
	//prints 5 lines

	//line1, upgrades
	cout << printPropertyUG(left) + string(win, ' ')
		+ printPropertyUG(right) << endl;

	//line2, border
	printMidLineGap();

	//line3, name and last player
	cout << printPropertyName(left) + "|" << string(win, ' ')
		+ printPropertyName(right) + "|" << endl;

	//line4, rest of players
	cout << "|" + printPlayersOnBuilding(left) + "|" << string(win, ' ')
		+ "|" + printPlayersOnBuilding(right) + "|" << endl;

	//line5 end border
	printEndLineGap(c);
}


//printBoardSLCs(left, right)
//see board.h for details
void Board::printBoardSLCs(int left, int right) {
	//prints 5 lines
	cout << printNonPropertyName(left) + string(win, ' ')
		+ printNonPropertyName(right) << endl;
	cout << printSelectedName("") + string(win, ' ')
		+ printSelectedName("") << endl;
	cout << "|" + printLastPlayerOnNP(left) + "|" + string(win, ' ')
		+ "|" + printLastPlayerOnNP(right) + "|" << endl;

	cout << "|" + printPlayersOnBuilding(left) + "|" << string(win, ' ')
		+ "|" + printPlayersOnBuilding(right) + "|" << endl;
	printEndLineGap(' ');
}

//printBoardWatoTop(left, right)
//see board.h for details
void Board::printBoardWatoTop(int left, int right) {
	cout << printPropertyUG(left) + string(win, ' ')
		+ printPropertyUG(right) << endl;
	printMidLineGap();
	cout << printPropertyName(left) + "|" << string(win, ' ')
		+ printPropertyName(right) + "|" << endl;

	//top border of wato logo
	cout << "|" + printPlayersOnBuilding(left) + "|" + string(bspace + 1, ' ') +
		string(inspace, '_') + string(bspace + 1, ' ')
		+ "|" + printPlayersOnBuilding(right) + "|" << endl;
	cout << "|" << string(wbox, '_') << "|" + string(bspace, ' ')
		+ "|" << string(inspace, ' ') << "|" << string(bspace, ' ')
		+ "|" + string(wbox, '_') + "|" << endl;
}

//printBoardWatoSide(left, right)
//see board.h for details
void Board::printBoardWatoSide(int left, int right) {
	cout << printNonPropertyName(left) + string(bspace, ' ')
		+ "| #   #  ##  #####  ###  ###   ###  #   #   # |" + string(bspace, ' ')
		+ printNonPropertyName(right) << endl;

	cout << "|" << string(wbox, ' ') << "|" << string(bspace, ' ');
	cout << "| #   # #  #   #   #   # #  # #   # #   #   # |";
	cout << string(bspace, ' ') << "|" << string(wbox, ' ') << "|" << endl;


	cout << "|" + printLastPlayerOnNP(left) + "|" + string(bspace, ' ')
		+ "| # # # ####   #   #   # ###  #   # #    # #  |"
		+ string(bspace, ' ') << "|" + printLastPlayerOnNP(right)
		+ "|" << endl;

	cout << "|" + printPlayersOnBuilding(left) + "|" << string(bspace, ' ')
		+ "| # # # #  #   #   #   # #    #   # #     #   |"
		+ string(bspace, ' ')
		+ "|" + printPlayersOnBuilding(right) + "|" << endl;


	cout << "|" << string(wbox, '_') << "|";
	cout << string(bspace, ' ');
	cout << "| ##### #  #   #    ###  #     ###  ## #  #   |";
	cout << string(bspace, ' ');
	cout << "|" << string(wbox, '_') << "|" << endl;
}

//printBoardWatoBottom(left, right)
//see board.h for details
void Board::printBoardWatoBottom(int left, int right) {
	cout << printPropertyUG(left) + string(bspace, ' ') + "|"
		+ string(inspace, '_') + "|"
		+ string(bspace, ' ') + printSelectedName("NEEDLES") << endl;

	cout << printSelectedName(string(wbox, '-')) + string(win, ' ')
		+ printSelectedName("HALL") + '\n';

	cout << printPropertyName(left) + "|" + string(win, ' ') + "|"
		+ printLastPlayerOnNP(right) + "|" << endl;

	cout << "|" + printPlayersOnBuilding(left) + "|" + string(win, ' ')
		+ "|" + printPlayersOnBuilding(right) + "|" << endl;

	printEndLineGap(' ');


}

//printBoardPACCOOP(left, right)
//see board.h for details
void Board::printBoardPACCOOP(int left, int right) {
	cout << printPropertyName(left) + "|" + string(win, ' ')
		+ printSelectedName("COOP") << endl;
	cout << printSelectedName("") + string(win, ' ')
		+ printSelectedName("FEE") << endl;

	cout << "|" + printLastPlayerOnNP(left) + "|" + string(win, ' ')
		+ "|" + printLastPlayerOnNP(right) + "|" << endl;

	cout << "|" + printPlayersOnBuilding(left) + "|" + string(win, ' ')
		+ "|" + printPlayersOnBuilding(right) + "|" << endl;

	printEndLineGap(' ');
}

//printBoardBottom()
//see board.h for details
void Board::printBoardBottom() {

	//building indexes
	const int tims = 10;
	const int needles = 7;
	const int mkv = 5;
	const int tuition = 4;
	const int slc = 2;
	const int osap = 0;

	//prints 5 lines of output
	int start = 10;
	int pastEnd = -1;

	//line1, names or upgrades
	for (int i = start; i > pastEnd; --i) {
		cout << "|";
		auto it = find(propertyIndex.begin(), propertyIndex.end(), i);
		if (it != propertyIndex.end()) {  //check if property
			shared_ptr<Property> p = dynamic_pointer_cast<Property> (buildings[i]);
			cout << string(p->getUpgrade(), 'I')
				+ string((wbox - 1) - p->getUpgrade(), ' ');
			if (p->isMort()) {   //check if mortgaged
				cout << "m";
			}
			else {
				cout << " ";
			}
		}
		else { //print first part of name for nonproperty
			switch (i) {

			case tims:
				cout << "DC Tims";
				continue;
			case needles:
				cout << "NEEDLES";
				continue;
			case mkv:
				cout << "MVK    ";
				continue;
			case tuition:
				cout << "TUITION";
				continue;
			case slc:
				cout << "SLC    ";
				continue;
			case osap:
				cout << "COLLECT|" << endl;
				continue;
			}
		}
	}

	//line2, name, border
	for (int i = start; i > pastEnd; --i) {
		cout << "|";
		auto it = find(propertyIndex.begin(), propertyIndex.end(), i);
		if (it != propertyIndex.end()) {
			cout << string(wbox, '-');  //border
		}
		else {
			//second part of name, if nonproperty
			switch (i) {

			case tims:
				cout << "Line   ";
				continue;
			case needles:
				cout << "HALL   ";
				continue;
			case mkv:
				cout << "       ";
				continue;
			case tuition:
				cout << "       ";
				continue;
			case slc:
				cout << "       ";
				continue;
			case osap:
				cout << "OSAP   |" << endl;
				continue;
			}
		}
	}

	//line3, last player, name of property
	for (int i = start; i > pastEnd; --i) {
		auto it = find(propertyIndex.begin(), propertyIndex.end(), i);
		if (it != propertyIndex.end()) {
			cout << printPropertyName(i);
		}
		else {
			cout << "|" + printLastPlayerOnNP(i);
		}
	}
	cout << "|" << endl;

	//line4 rest of players
	for (int i = start; i > pastEnd; --i) {
		cout << "|" + printPlayersOnBuilding(i);
	}

	cout << "|" << endl;

	//line5 border
	for (int i = 0; i < width; ++i) {
		if (i % (wbox + 1) == 0) {
			cout << "|";
		}
		else {
			cout << "_";
		}
	}

	cout << endl;
}



/////////////////////////////////////////////////////
//public methods
/////////////////////////////////////////////////////


//constructor
//see board.h for details
Board::Board(vector<shared_ptr<Building>> buildings)
	: buildings{ buildings } {}

//getBuildings
//see board.h for details
vector<shared_ptr<Building>> Board::getBuildings() {
	return buildings;
}

//printBoard()
//see board.h for details
void Board::printBoard() {
	const int rch = 11;
	const int pac = 12;
	const int dwe = 13;
	const int cph = 14;
	const int uwp = 15;
	const int lhi = 16;
	const int slc1 = 17;
	const int bmh = 18;
	const int opt = 19;
	const int eit = 31;
	const int esc = 32;
	const int slc2 = 33;
	const int c2 = 34;
	const int rev = 35;
	const int needles = 36;
	const int mc = 37;
	const int coop = 38;
	const int dc = 39;

	printBoardTop();
	printBoardGenSide(opt, eit, ' ');
	printBoardGenSide(bmh, esc, ' ');
	printBoardSLCs(slc1, slc2);
	printBoardWatoTop(lhi, c2);
	printBoardWatoSide(uwp, rev);
	printBoardWatoBottom(cph, needles);
	printBoardGenSide(dwe, mc, ' ');
	printBoardPACCOOP(pac, coop);
	printBoardGenSide(rch, dc, '_');
	printBoardBottom();
}
