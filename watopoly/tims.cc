#include "tims.h"
#include <iostream>


Tims::Tims()
	:NonProperty{ "DC_Tims_Line"} {

}

//check in main if the player is already in Tims before rolling dice
//also prompt in main how the player wants to play (roll double, tim cup, pay)
//this method just acknowledges the first time a player landed on Tims
void Tims::accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice, std::shared_ptr<Building> self) {
	addPlayerToTile(player);
	std::cout << "You landed in the DC Tims line!" << std::endl;
}