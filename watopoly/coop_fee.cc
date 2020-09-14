#include "coop_fee.h"
#include "player.h"
#include "bank.h"
#include "dice.h"
#include <iostream>


CoopFee::CoopFee()
	: NonProperty{ "CoopFee"} {
}

void CoopFee::accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
	, std::shared_ptr<Building> self) {
	addPlayerToTile(player);
	std::cout << "You landed on Coop Fee. 1500 dollars will be paid to the school automatically." << std::endl;
	
	player->payBank(150);
}