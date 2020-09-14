#pragma once

#include <memory>
#include "non_property.h"
class Player;

class GoToTims : public NonProperty {
	std::shared_ptr<Building> tims;
public:
	GoToTims(std::shared_ptr<Building> tims);
	void accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
		, std::shared_ptr<Building> self) override; //this method should include the line tims->accept(player);
};