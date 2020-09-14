#include "go_to_tims.h"

#include <iostream>

#include "bank.h"
#include "dice.h"
#include "player.h"

GoToTims::GoToTims(std::shared_ptr<Building> tims)
    : NonProperty{"GoToTims"}, tims{tims} {}

void GoToTims::accept(std::shared_ptr<Player> player,
                      std::shared_ptr<Dice> dice,
                      std::shared_ptr<Building> self) {
    std::cout << "You landed on the Go to Tims square! Your player will be "
                 "moved to the DC Tims Line."
              << std::endl;
    player->incTimesInJail();
    player->setLocation(10);
    tims->accept(player, dice,
                 tims);  // player moves immediately to the tims line
}