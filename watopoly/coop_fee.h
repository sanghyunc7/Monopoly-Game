#pragma once

#include "non_property.h"
class Player;
class Bank;
class Dice;

class CoopFee : public NonProperty {
public:
	CoopFee();
	void accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
		, std::shared_ptr<Building> self) override;
};