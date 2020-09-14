#pragma once

#include <vector>
#include <string>
#include "non_property.h"

class Player;

//use template method
//this is an abstract base class
class SLC : public NonProperty {
	int card;
	virtual void drawCard(int& card) = 0; //template method to override
	void playCard(std::shared_ptr<Player> player, 
		int& card, std::shared_ptr<Dice> dice);
protected:
	std::vector<std::string> cardMessage;
	std::vector<std::shared_ptr<Building>> action;
public:
	
	//use this constructor to initialize deck of Cards
	SLC(std::string name, 
		std::shared_ptr<Building> back3,
		std::shared_ptr<Building> back2, 
		std::shared_ptr<Building> back1,
		std::shared_ptr<Building> forward1,
		std::shared_ptr<Building> forward2,
		std::shared_ptr<Building> forward3,
		std::shared_ptr<Building> tims,
		std::shared_ptr<Building> osap);

	//a wrapper to call drawCard, then playCard, etc
	void accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
		, std::shared_ptr<Building> self) override;
};