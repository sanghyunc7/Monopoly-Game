#pragma once

#include "non_property.h"
#include <memory>
class Player;
class Bank;

class Tuition : public NonProperty {
	void payFee(Player& player); //the player pays the school $300 or 10% of total worth
public:
	Tuition();
	void accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
		, std::shared_ptr<Building> self) override;

};