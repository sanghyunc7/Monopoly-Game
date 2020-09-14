#pragma once

#include "needles_hall.h"

class Player;

class NeedlesHallNormal : public NeedlesHall {
public:
	void drawCard(int& card) override;
	NeedlesHallNormal();
};