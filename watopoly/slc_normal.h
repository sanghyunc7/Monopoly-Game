#pragma once

#include "slc.h"

class Player;

class SLCNormal : public SLC {
public:
	void drawCard(int& card) override;
	SLCNormal(
		std::shared_ptr<Building> back3,
		std::shared_ptr<Building> back2,
		std::shared_ptr<Building> back1,
		std::shared_ptr<Building> forward1,
		std::shared_ptr<Building> forward2,
		std::shared_ptr<Building> forward3,
		std::shared_ptr<Building> tims,
		std::shared_ptr<Building> osap);
//does not need accept method
};