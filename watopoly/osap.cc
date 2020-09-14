#include "osap.h"
#include "player.h"
#include "bank.h"
#include "dice.h"

OSAP::OSAP()
	:NonProperty{ "OSAP"} {
}

void OSAP::accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice
	, std::shared_ptr<Building> self) {
	addPlayerToTile(player);
	bank->payPlayer(player, 200);
}