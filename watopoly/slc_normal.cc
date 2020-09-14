#include "slc_normal.h"
#include <ctime>
#include <cstdlib>


SLCNormal::SLCNormal(
	std::shared_ptr<Building> back3,
	std::shared_ptr<Building> back2,
	std::shared_ptr<Building> back1,
	std::shared_ptr<Building> forward1,
	std::shared_ptr<Building> forward2,
	std::shared_ptr<Building> forward3,
	std::shared_ptr<Building> tims,
	std::shared_ptr<Building> osap)
	:SLC{ "SLC", back3, back2, back1, forward1, forward2, forward3, tims, osap } {
}

void SLCNormal::drawCard(int& card) {
	srand((int)time(0));
	card = (rand() % action.size());
}