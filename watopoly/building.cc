#include "building.h"

#include <algorithm>

#include "bank.h"
#include "player.h"

Building::Building(std::string name, bool bankOwns)
    : name{name}, owner{"BANK"}, ownerisbank{true} {}

Building::Building(std::string name, std::string owner)
    : name{name}, owner{owner}, ownerisbank{false} {}

void Building::addPlayerToTile(std::shared_ptr<Player> player) {
    playersOnTile.emplace_back(player);
}

void Building::removePlayerFromTile(std::shared_ptr<Player> player) {
    std::remove(playersOnTile.begin(), playersOnTile.end(), player);
}

std::vector<std::shared_ptr<Player>> Building::getPlayersOnTile() {
    return playersOnTile;
}

int Building::getNumPlayers() { return playersOnTile.size(); }

std::string Building::getName() { return name; }

void Building::setOwner(std::string own) {
    ownerisbank = true;
    owner = own;
}

std::string Building::getOwner() { return owner; }
