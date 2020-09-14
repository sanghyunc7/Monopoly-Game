#pragma once

#include <map>

#include "building.h"

class Player;
class Bank;
class Dice;

class Property : public Building {
    std::string faculty;  // monopoly block

    int buildingPrice;
    int upgradeLevel;  // default is zero
    int improvementCost;

    // this is the upgrade plan
    // key is upgrade level, value is the rent people have to pay when they land
    std::map<int, int> improvements;

    // people don't pay rent when they land on mortgaged properties
    bool mortgaged;

    // add more private methods as necessary

    // this method completes the constructor call by initializing the class
    // fields with the given arguments
    void create(int price, int icost, const std::string& fac, int zero, int one,
                int two, int three, int four, int five);

    // this method finds the relevant upgrade plan based on the name of the
    // property and calls the create(...) method with the right arguments
    void findUpgradePlan(const std::string name);

   public:
    // default when doing new game
    // invariant: the given names should only be those given from the watopoly
    // rules book
    Property(std::string name);

    // from saved game state
    Property(std::string name, std::string owner, int upgradeLevel,
             bool mortgaged);

    Property(std::string name, int upgradeLevel, bool mortgaged);

    // returns information such as the name, mortgage state, and improvements
    std::string printBuilding() override;

    // returns the potential cash owner can extract by selling all improvements
    // and mortgaging it
    int getPropertyValue() override;

    int getPrice();
    int getUpgrade();
    bool isMort();

    void upgrade();

    void sellUpgrade();

    std::string getFaculty();
    // mortgages the property
    // owner of this property gets 50% of the cash paid for "buildingPrice"
    void mortgage();

    // owner must pay 60% back to unmortgage this property
    // players that land on this property will resume paying rent
    void unMortgage();

    // we landed, check if someone already owns it
    // pay rent if someone other than the bank owns it
    // buy/or put it up for auction
    void accept(std::shared_ptr<Player> player, std::shared_ptr<Dice> dice,
                std::shared_ptr<Building> self) override;
};
