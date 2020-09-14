#pragma once

#include <memory>
#include <string>
#include <vector>

class Player;
class Dice;
class Bank;

// abstract base class
class Building {
    std::vector<std::shared_ptr<Player>> playersOnTile;

   protected:
    std::string name;
    std::string owner;
    std::shared_ptr<Bank> bank;

   public:
    bool ownerisbank = false;
    // could be players or a Bank (for NonProperty)
    // constructor for bank as a owner of the building
    Building(std::string name, bool bankOwns);
    // constructor for player as a owner of the building
    Building(std::string name, std::string owner);

    int getNumPlayers();

    void addPlayerToTile(
        std::shared_ptr<Player>
            player);  // add the player piece or name to the vector string

    void removePlayerFromTile(
        std::shared_ptr<Player>
            player);  // removes the player, does nothing if no player found
    std::vector<std::shared_ptr<Player>> getPlayersOnTile();

    virtual std::string getOwner();
    virtual std::string getFaculty() = 0;
    std::string getName();
    virtual int getPropertyValue() = 0;
    virtual std::string printBuilding() = 0;
    virtual void accept(std::shared_ptr<Player> player,
        std::shared_ptr<Dice> dice, std::shared_ptr<Building> self) = 0;

    void setOwner(std::string owner);
    // add Observer vector and attach/detach/notifyAll methods

    //virtual ~Building() = 0;
};
