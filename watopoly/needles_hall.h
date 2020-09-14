#pragma once

#include <vector>
#include <string>
#include "non_property.h"

class Player;

//use template method
//this is an abstract base class
class NeedlesHall : public NonProperty {
	int card;
	virtual void drawCard(int& card) = 0; //template method to override
	void playCard(std::shared_ptr<Player> player,
		int& card, std::shared_ptr<Dice> dice);
protected:
	std::vector<std::string> cardMessage;
	std::vector<int> action;
public:
	NeedlesHall(std::string name); //use this constructor to initialize deck of Cards
	//a wrapper to call drawCard, then playCard, etc
	void accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
		, std::shared_ptr<Building> self) override;
};