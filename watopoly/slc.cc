#include "slc.h"
#include <iostream>

//use this constructor to initialize deck of Cards
SLC::SLC(std::string name,
	std::shared_ptr<Building> back3,
	std::shared_ptr<Building> back2,
	std::shared_ptr<Building> back1,
	std::shared_ptr<Building> forward1,
	std::shared_ptr<Building> forward2,
	std::shared_ptr<Building> forward3,
	std::shared_ptr<Building> tims,
	std::shared_ptr<Building> osap)
	: NonProperty{ name }, card{ 0 } {
	action.emplace_back(back3);
	action.emplace_back(back2);
	action.emplace_back(back1);
	action.emplace_back(forward1);
	action.emplace_back(forward2);
	action.emplace_back(forward3);
	action.emplace_back(tims);
	action.emplace_back(osap);

	cardMessage.emplace_back("back3");
	cardMessage.emplace_back("back2");
	cardMessage.emplace_back("back1");
	cardMessage.emplace_back("forward1");
	cardMessage.emplace_back("forward2");
	cardMessage.emplace_back("forward3");
	cardMessage.emplace_back("tims");
	cardMessage.emplace_back("osap");
}

void SLC::playCard(std::shared_ptr<Player> player, int& card, std::shared_ptr<Dice> dice) {
	this->removePlayerFromTile(player);
	action[card]->addPlayerToTile(player);
	action[card]->accept(player, dice, action[card]);
}


//a wrapper to call drawCard, then playCard, etc
void SLC::accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice, 
	std::shared_ptr<Building> self) {
	std::cout << "You landed on SLC. A card will be drawn for you." << std::endl;
	drawCard(card); //template
	std::cout << "You drew " + cardMessage[card] + "!" << std::endl;
	playCard(player, card, dice); //common step
}