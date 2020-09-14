#include "tuition.h"
#include "player.h"
#include "bank.h"
#include <iostream>


Tuition::Tuition()
	:NonProperty{ "Tuition" } {
}

void Tuition::accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice, 
	std::shared_ptr<Building> self) {
	addPlayerToTile(player);
	int netWorth = player->getCash() * 0.1;
	std::cout << "You landed on Tuition!" << std::endl;
	std::cout << "Pay either $300 or 10% of your net worth: $" + std::to_string(netWorth) << std::endl;

	std::string ans1 = "";
	std::cin >> ans1;
	do {
		std::cout << "Answer either [300/networth] only." << std::endl;
		if (ans1 == "300") {
			player->payBank(300);

		}
		else if (ans1 == "networth") {
			player->payBank(netWorth);
		}
	} while (ans1 != "300" && ans1 != "networth");
}