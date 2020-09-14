#include "property.h"

#include <iostream>

#include "bank.h"
#include "building.h"
#include "dice.h"
#include "player.h"

extern Bank g_bank;
extern std::map<std::string, std::shared_ptr<Player>> g_playersInGame;

void Property::create(int price, int icost, const std::string& fac, int zero,
                      int one, int two, int three, int four, int five) {
    buildingPrice = price;
    faculty = fac;
    improvementCost = icost;

    improvements.insert(std::pair<int, int>(0, zero));
    improvements.insert(std::pair<int, int>(1, one));
    improvements.insert(std::pair<int, int>(2, two));
    improvements.insert(std::pair<int, int>(3, three));
    improvements.insert(std::pair<int, int>(4, four));
    improvements.insert(std::pair<int, int>(5, five));
}

void Property::findUpgradePlan(const std::string name) {
    if (name == "AL") {
        create(40, 50, "Arts1", 2, 10, 30, 90, 160, 250);
    } else if (name == "ML") {
        create(60, 50, "Arts1", 4, 20, 60, 180, 320, 450);
    } else if (name == "ECH") {
        create(100, 50, "Arts2", 6, 30, 90, 270, 400, 550);
    } else if (name == "PAS") {
        create(100, 50, "Arts2", 6, 30, 90, 270, 400, 550);
    } else if (name == "HH") {
        create(120, 50, "Arts2", 8, 40, 100, 300, 450, 750);
    } else if (name == "RCH") {
        create(140, 100, "Eng", 10, 50, 150, 450, 625, 750);
    } else if (name == "DWE") {
        create(140, 100, "Eng", 10, 50, 150, 450, 625, 750);
    } else if (name == "CPH") {
        create(160, 100, "Eng", 12, 60, 180, 500, 700, 900);
    } else if (name == "LHI") {
        create(180, 100, "Health", 14, 70, 200, 550, 750, 950);
    } else if (name == "BMH") {
        create(180, 100, "Health", 14, 70, 200, 550, 750, 950);
    } else if (name == "OPT") {
        create(200, 100, "Health", 16, 80, 220, 600, 800, 1000);
    } else if (name == "EV1") {
        create(220, 150, "Env", 18, 90, 250, 700, 875, 1050);
    } else if (name == "EV2") {
        create(220, 150, "Env", 18, 90, 250, 700, 875, 1050);
    } else if (name == "EV3") {
        create(240, 150, "Env", 20, 100, 300, 750, 925, 1100);
    } else if (name == "PHYS") {
        create(260, 150, "Sci1", 22, 110, 330, 800, 975, 1150);
    } else if (name == "B1") {
        create(260, 150, "Sci1", 22, 110, 330, 800, 975, 1150);
    } else if (name == "B2") {
        create(280, 150, "Sci1", 24, 120, 360, 850, 1025, 1200);
    } else if (name == "EIT") {
        create(300, 200, "Sci2", 26, 130, 390, 900, 1100, 1275);
    } else if (name == "ESC") {
        create(300, 200, "Sci2", 26, 130, 390, 900, 1100, 1275);
    } else if (name == "C2") {
        create(320, 200, "Sci2", 28, 150, 450, 1000, 1200, 1400);
    } else if (name == "MC") {
        create(350, 200, "Math", 35, 175, 500, 1100, 1300, 1500);
    } else if (name == "DC") {
        create(400, 200, "Math", 50, 200, 600, 1400, 1700, 2000);
    } else if (name == "MKV" || name == "UWP" || name == "V1" ||
               name == "REV") {
        create(200, -1, "Res", -1, 25, 50, 100, 200,
               -1);  // index represents how many Res owner has
    } else if (name == "PAC" || name == "CIF") {
        create(150, -1, "Gym", -1, 4, 10, -1, -1, -1);
    } else {
        create(-1, -1, "Unofficial", -1, -1, -1, -1, -1, -1);
        std::cout << "Given name is not from the Watopoly Rule Book" << name
                  << std::endl;
        return;
    }
}

// constructor call when NOT loading a game
Property::Property(std::string name)
    : Building{name, true}, upgradeLevel{0}, mortgaged{false} {
    // we now consider cases for the unique improvement plans
    findUpgradePlan(name);
}
// constructor call when loading a game
Property::Property(std::string name, std::string owner, int upgradeLevel,
                   bool mortgaged)
    : Building{name, owner}, upgradeLevel{upgradeLevel}, mortgaged{mortgaged} {
    findUpgradePlan(name);
}
// constructor call when loading game with bank as owner
Property::Property(std::string name, int upgradeLevel, bool mortgaged)
    : Building{name, true}, upgradeLevel{upgradeLevel}, mortgaged{mortgaged} {
    findUpgradePlan(name);
}

// we landed, check if someone already owns it
// pay rent if someone other than the bank owns it
// buy/or put it up for auction
void Property::accept(std::shared_ptr<Player> player,
                      std::shared_ptr<Dice> dice, std::shared_ptr<Building> self) {
    addPlayerToTile(player);
    // if a playing player owns this property and is not mortgaged
    if (!ownerisbank && !mortgaged) {
        // player must pay the owner
        std::cout << "You landed on someone else's property!" << std::endl;
        int rent = improvements[upgradeLevel];  // default

        if (faculty == "Gym") {
            Rolls recentRoll = dice->getSavedRoll();
            int sum = recentRoll.dice1 + recentRoll.dice2;

            upgradeLevel = 0;
            auto playerOwner = g_playersInGame.find(owner);
            for (auto it = (playerOwner->second)->getProperties().begin();
                 it != (playerOwner->second)->getProperties().end(); ++it) {
                if ((it->second)->getFaculty() == "Gym") upgradeLevel++;
            }
            rent = improvements[upgradeLevel] * sum;
            upgradeLevel =
                0;  // does not actually have upgrades that are visible
        } else if (faculty == "Res") {
            upgradeLevel = 0;

            auto playerOwner = g_playersInGame.find(owner);
            for (auto it = (playerOwner->second)->getProperties().begin();
                 it != (playerOwner->second)->getProperties().end(); ++it) {
                if ((it->second)->getFaculty() == "Res") upgradeLevel++;
            }
            rent = improvements[upgradeLevel];
            upgradeLevel = 0;
        }
        std::cout << "You must pay " + rent << std::endl;
        auto playerOwner = g_playersInGame.find(owner);
        player->payPlayer(playerOwner->second, rent);
    } else if (ownerisbank) {  // default owner is bank
        
        std::string ans1;
        do {
            std::cout << "Do you want to buy " + this->name + " for "
            << this->buildingPrice << " [Y/N]?" << std::endl;
            std::cin >> ans1;
            if (ans1 == "Y") {
                g_bank.trade(player, std::dynamic_pointer_cast<Property>(self), 
                    this->buildingPrice);
                self->ownerisbank = false;
            } else if (ans1 == "N") {
                g_bank.beginAuction(name);
                // call auction 
            } else {
                std::cout << "Sorry, please give a Y for yes, or N for no to "
                             "the question.";
            }
        } while (ans1 != "Y" && ans1 != "N");
    }
}

void Property::mortgage() {
    if (ownerisbank) {
        std::cout << "This is not your property" << std::endl;
    }
    if (upgradeLevel > 0) {
        std::cout << "You cannot mortgage an upgraded property. Please remove "
                     "them first."
                  << std::endl;
        return;
    }
    if (mortgaged) {
        std::cout << name + " is already on mortgage" << std::endl;
        return;
    }
    auto playerOwner = g_playersInGame.find(owner);
    if (g_bank.payPlayer(playerOwner->second, buildingPrice * 0.5)) {
        mortgaged = true;
        upgradeLevel = -1;
        std::cout << "For mortgaging " + name + ", player received " +
                         std::to_string(buildingPrice / 2)
                  << std::endl;
    }
}

int Property::getPrice() { return buildingPrice; }

bool Property::isMort() { return mortgaged; }

void Property::unMortgage() {
    if (!mortgaged) {
        std::cout << name + " is already not on mortgage" << std::endl;
        return;
    }
    int price = buildingPrice * 0.6;
    auto playerOwner = g_playersInGame.find(owner);
    if (playerOwner->second->payBank(price)) {
        mortgaged = false;
        upgradeLevel = 0;
        std::cout << "For unmortgaging " + name + ", player paid " +
                         std::to_string(price)
                  << std::endl;
    }
}

std::string Property::printBuilding() {
    std::string info = "";
    info += "Name: " + name + ", UpgradeLevel: " + std::to_string(upgradeLevel);
    return info;
}

int Property::getPropertyValue() {
    // add mortgage cost
    // improvement cost
    return upgradeLevel * improvementCost * 0.5 + buildingPrice * 0.5;
}

int Property::getUpgrade() { return upgradeLevel; }

// cannot upgrade Res, cannot upgrade mortgaged
// cannot upgrade if this property is not owner's monopoly

void Property::upgrade() {
    // cannot upgrade Res, cannot upgrade mortgaged
    // cannot upgrade if this property is not owner's monopoly

    if (faculty == "Res" || faculty == "Gym") {
        std::cout << "You cannot upgrade this type of property." << std::endl;
        return;
    }
    if (mortgaged) {
        std::cout << "You cannot upgrade a mortgaged property." << std::endl;
    }

    int sameFaculty = 0;
    int required = 0;
    if (faculty == "Arts1" || faculty == "Math")
        required = 2;
    else
        required = 3;

    auto playerOwner = g_playersInGame.find(owner);
    for (auto it = (playerOwner->second)->getProperties().begin();
         it != (playerOwner->second)->getProperties().end(); ++it) {
        if ((it->second)->getFaculty() == faculty) sameFaculty++;
    }

    if (sameFaculty < required) {
        std::cout << "You cannot upgrade a property that is not part of a "
                     "monopoly."
                  << std::endl;
        return;
    }
    if (upgradeLevel >= 5) {
        std::cout << "You already maxed out upgrading this property."
                  << std::endl;
        return;
    }
    playerOwner = g_playersInGame.find(owner);
    if ((playerOwner->second)
            ->payBank(improvementCost)) {  // pay method will be bool
        upgradeLevel++;
        std::cout << "Upgrade success!" << std::endl;
    }
}

void Property::sellUpgrade() {
    if (upgradeLevel <= 0) {
        std::cout << "You do not have any upgrades on this property"
                  << std::endl;
        return;
    }
    std::cout << "There are " + std::to_string(upgradeLevel) +
                     " upgrades on this property"
              << std::endl;
    std::cout << "How many would you like to sell? Enter a whole number."
              << std::endl;
    int ans1;
    while (true) {
        ans1 = 0;
        if (!(std::cin >> ans1)) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Please enter a whole number only." << std::endl;
        } else if (ans1 < 0 || ans1 > upgradeLevel) {
            std::cout << "Please enter a realistic number of properties you "
                         "wish to sell."
                      << std::endl;
        } else {
            auto playerOwner = g_playersInGame.find(owner);
            if (g_bank.payPlayer(playerOwner->second,
                                 ans1 * improvementCost * 0.5)) {
                upgradeLevel -= ans1;
                std::cout << "Selling upgrades done." << std::endl;
            }
            break;
        }
    }
}

std::string Property::getFaculty() { return faculty; }
