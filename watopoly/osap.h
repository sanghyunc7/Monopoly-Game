#pragma once

#include <memory>
#include "non_property.h"

class Player;

class OSAP : public NonProperty {
public: 
	OSAP();
	//bank pays 200 to player
	void accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
		, std::shared_ptr<Building> self) override;
};