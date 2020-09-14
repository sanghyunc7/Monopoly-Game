#pragma once

#include "non_property.h"
#include <memory>
class Player;

class GooseNesting : public NonProperty {
	//private method that does some kind of goose attack
public:
	GooseNesting();
	void accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice, 
		std::shared_ptr<Building> self) override; //wrapper for calling goose attack method

};