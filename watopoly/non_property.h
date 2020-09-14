#pragma once

#include "building.h"
class Bank;
class Player;
class Dice;

//abstract base class
class NonProperty : public Building {
public:
	NonProperty(std::string name);
	std::string getFaculty() override;
	std::string printBuilding() override;
	int getPropertyValue() override;
	virtual void accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice, 
		std::shared_ptr<Building> self) = 0;
	//virtual ~NonProperty() = 0; //pure virtual destructor
};
