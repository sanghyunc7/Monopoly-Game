#include "needles_hall.h"
#include "bank.h"
#include "player.h"
#include <iostream>

NeedlesHall::NeedlesHall(std::string name)
	: NonProperty{ name}, card{ 0 } {
	for (int i = 0; i < 1; i++) {
		action.emplace_back(-200);
		cardMessage.emplace_back(std::to_string(-200));
		action.emplace_back(200);
		cardMessage.emplace_back(std::to_string(200));
	}

	for (int i = 0; i < 2; i++) {
		action.emplace_back(-100);
		cardMessage.emplace_back(std::to_string(-100));
		action.emplace_back(100);
		cardMessage.emplace_back(std::to_string(100));
	}
	for (int i = 0; i < 3; i++) {
		action.emplace_back(-50);
		cardMessage.emplace_back(std::to_string(-50));
		action.emplace_back(50);
		cardMessage.emplace_back(std::to_string(50));
	}	
	for (int i = 0; i < 6; i++) {
		action.emplace_back(25);
		cardMessage.emplace_back(std::to_string(25));
	}
}

void NeedlesHall::playCard(std::shared_ptr<Player> player,
	int& card, std::shared_ptr<Dice> dice) {
	player->payBank(action[card]);
}


void NeedlesHall::accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
	, std::shared_ptr<Building> self) {
	addPlayerToTile(player);
	std::cout << "You landed on Needles Hall. A card will be drawn for you." << std::endl;
	drawCard(card); //template
	std::cout << "You drew " + cardMessage[card] + "!" << std::endl;
	std::cout << "Payment will be made automatically to the bank." << std::endl;
	playCard(player, card, dice); //common step
}