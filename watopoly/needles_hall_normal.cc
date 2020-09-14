#include "needles_hall_normal.h"
#include <ctime>
#include <cstdlib>

NeedlesHallNormal::NeedlesHallNormal()
	:NeedlesHall{ "NeedlesHall"} {
}

void NeedlesHallNormal::drawCard(int& card) {
	srand((int)time(0));
	card = (rand() % action.size());
}