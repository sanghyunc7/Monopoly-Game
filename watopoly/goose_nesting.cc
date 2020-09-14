#include "goose_nesting.h"
#include <iostream>

GooseNesting::GooseNesting()
	: NonProperty{ "Goose_Nesting"} {
}

void GooseNesting::accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
	, std::shared_ptr<Building> self) {
	addPlayerToTile(player);
	std::cout << "You got attacked by a gaggle of geese!" << std::endl;
}