#include "player.h"

#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "bank.h"
#include "building.h"
#include "property.h"
#include "timsCup.h"

extern Bank g_bank;
extern std::map<std::string, std::shared_ptr<Player>> g_playersInGame;
static bool DEBUG = false;
extern int g_timsCups;
// static int g_timsCups = 1;

Player::Player(std::string name, std::string piece)
    : name(name),
      piece(piece),
      cash(1500),
      currentLocation(0),
      timsCups(0),
      timesInJail(0),
      oweBank(false),
      owePlayer(false),
      debtAmount(0) {}

Player::Player(std::string const &name, std::string &piece,
               std::vector<std::shared_ptr<Property>> &myProperties,
               long long cash, int currentLocation, int timsCups,
               int timesInJail, bool oweBank, std::shared_ptr<Player> creditor,
               int debtAmount) {}

Player::~Player() {}

bool Player::payPlayer(std::shared_ptr<Player> other, int amount) {
    if (amount < 0) {
        std::cout << name << " to " << other->getName() << " $" << amount
                  << " pay failed. " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!decCash(amount)) {
        // first time bankrupt
        setCreditorToPlayer(other, amount);
        promptBankruptcy();
        return false;
    }
    if (!other->incCash(amount)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (DEBUG) {
        std::cout << name << "'s payment of $" << amount << " to "
                  << other->getName() << " was successful." << std::endl;
    }
    return true;
}

bool Player::payPlayer(std::shared_ptr<Player> other,
                       std::shared_ptr<Property> property) {
    if (!ownProperty(property)) {
        std::cout << name << " to " << other->getName() << " "
                  << property->getName() << " pay failed, does not own. "
                  << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!decProperty(property)) {
        // TODO should not bankrupt
        // setCreditorToPlayer(other, property->getPrice());
        // promptBankruptcy();
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->incProperty(property)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (DEBUG) {
        std::cout << name << " to " << other->getName() << " "
                  << property->getName() << " pay succeeded." << std::endl;
    }
    property->setOwner(other->getName());  // TODO transfer owner
    return true;
}

bool Player::trade(std::shared_ptr<Player> other, int give,
                   std::shared_ptr<Property> receive) {
    if (give < 0) {
        std::cout << name << " to " << other->getName() << " $" << give
                  << " trade failed. " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->ownProperty(receive)) {
        std::cout << other->getName() << " to " << name << " "
                  << receive->getName() << " trade failed, does not own. "
                  << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (receive->getUpgrade() > 0) {
        std::cout << other->getName() << " to " << name << " "
                  << receive->getName() << " trade failed, has upgrade. "
                  << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!hasCash(give)) {
        std::cout << name << " to " << other->getName() << " $" << give
                  << " trade failed, not enough cach. " << __FILE__ << __LINE__
                  << std::endl;
        return false;
    }
    // Avoid call payPlayer cach, may trigger bankrupt
    if (!other->incCash(give)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!decCash(give)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    // Cannot make shared_ptr out of self, inc/dec property
    if (!incProperty(receive)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->decProperty(receive)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (DEBUG) {
        std::cout << name << " receive " << receive->getName()
                  << " from and give $" << give << " to " << other->getName()
                  << " succeeded." << std::endl;
    }
    receive->setOwner(name);
    return true;
}

bool Player::trade(std::shared_ptr<Player> other,
                   std::shared_ptr<Property> give, int receive) {
    if (receive < 0) {
        std::cout << other->getName() << " to " << name << " $" << receive
                  << " trade failed. " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!ownProperty(give)) {
        std::cout << name << " to " << other->getName() << " "
                  << give->getName() << " trade failed, does not own. "
                  << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (give->getUpgrade() > 0) {
        std::cout << name << " to " << other->getName() << " "
                  << give->getName() << " trade failed, has improvement. "
                  << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->hasCash(receive)) {
        std::cout << other->getName() << " does not have enough money to pay $"
                  << receive << " to " << name << std::endl;
        return false;
    }
    // Avoid call payPlayer cach, may trigger bankrupt
    if (!incCash(receive)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->decCash(receive)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!decProperty(give)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->incProperty(give)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (DEBUG) {
        std::cout << name << " give " << give->getName() << " to and receive $"
                  << receive << " from " << other->getName() << " succeeded."
                  << std::endl;
    }
    give->setOwner(other->getName());
    return true;
}

bool Player::trade(std::shared_ptr<Player> other,
                   std::shared_ptr<Property> give,
                   std::shared_ptr<Property> receive) {
    if (give->getUpgrade() > 0 || receive->getUpgrade() > 0) {
        std::cout << name << " and " << other->getName()
                  << " trade property failed, has improvement. " << __FILE__
                  << __LINE__ << std::endl;
        return false;
    }
    if (!ownProperty(give)) {
        std::cout << name << " to " << other->getName() << " "
                  << give->getName() << " trade failed, does not own. "
                  << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->ownProperty(receive)) {
        std::cout << other->getName() << " to " << name << " "
                  << receive->getName() << " trade failed, does not own. "
                  << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!decProperty(give)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->incProperty(give)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    // Cannot make shared_ptr out of self, inc/dec property
    if (!incProperty(receive)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->decProperty(receive)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (DEBUG) {
        std::cout << name << " give " << give->getName() << " to and receive "
                  << receive->getName() << " from " << other->getName()
                  << " succeeded." << std::endl;
    }
    give->setOwner(other->getName());
    receive->setOwner(name);
    return true;
}

bool Player::incProperty(std::shared_ptr<Property> property) {
    auto res = myProperties.emplace(property->getName(), property);
    if (!res.second) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        exit(1);
    }
    return true;
}

bool Player::decProperty(std::shared_ptr<Property> property) {
    int ret = myProperties.erase(property->getName());
    if (1 != ret) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        exit(1);
    }
    return true;
}

bool Player::incCash(int amount) {
    if (amount < 0) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        exit(1);
    }
    cash += amount;
    return true;
}

bool Player::decCash(int amount) {
    if (amount < 0) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        exit(1);
    }
    if (cash < amount) {
        std::cout << "Invalid transaction: " << name
                  << "'s current net worth of $" << cash
                  << " is insufficient to pay the amount of $" << amount
                  << std::endl;
        return false;
    }
    cash -= amount;
    return true;
}

void Player::incTimesInJail() { timesInJail++; }
int Player::getTimesInJail() { return timesInJail; }

// Does not work if the player is deciding how to pay Tuition.
void Player::printAssets() {
    std::cout << this->name << "' s assets:" << std::endl;
    std::cout << "Cash: " << this->cash << std::endl;
    std::cout << "Tim Cups: " << this->timsCups << std::endl;
    for (auto it = myProperties.begin(); it != myProperties.end(); it++) {
        std::cout << it->second->printBuilding() << std::endl;
    }
}

// get members
std::map<std::string, std::shared_ptr<Property>> Player::getProperties() {
    return myProperties;
}
int Player::getLocation() { return currentLocation; }
std::string Player::getName() { return name; }
std::string Player::getPiece() { return piece; }
long long Player::getCash() { return cash; }

// Rules for payment:
// 1. Players cannot trade cash for cash
// 2. Bank can take Players's money and give Players cash
// 3. Players can trade cash for Property and Property for cash
// 4. Players need to pay cash to other Players for tuition
// 5. Players need to receive cash from other Players

bool Player::payBank(int paymentAmount) {
    if (paymentAmount < 0) {
        std::cout << name << " to bank $" << paymentAmount << " pay failed. "
                  << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!decCash(paymentAmount)) {
        // bandrupt
        setCreditorToBank(paymentAmount);
        promptBankruptcy();
        return false;
    }
    if (DEBUG) {
        std::cout << name << " to bank $" << paymentAmount << " pay succeeded. "
                  << std::endl;
    }
    return false;
}

bool Player::payBank(std::shared_ptr<Property> property) {
    if (!ownProperty(property)) {
        std::cout << name << " to bank " << property->getName()
                  << " pay failed, does not own. " << __FILE__ << __LINE__
                  << std::endl;
        return false;
    }
    if (!decProperty(property)) {
        // TODO should not bankrupt
        // setCreditorToBank(property->getPropertyValue());
        // promptBankruptcy();
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!g_bank.incProperty(property)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (DEBUG) {
        std::cout << name << " to bank " << property->getName()
                  << " pay succeeded." << std::endl;
    }
    return true;
}

// Bankruptcy methods used only when cash < debt
void Player::declareBankruptcy() {
    if (oweBank) {
        payBank(cash);
        while (!myProperties.empty()) {
            auto it = myProperties.begin();
            it->second->unMortgage();  // must first unmortgage property
            myProperties.begin()->second->setOwner("BANK");
            payBank(myProperties.begin()->second);
        }
    } else {
        payPlayer(creditor, cash);
        while (!myProperties.empty()) {
            myProperties.begin()->second->setOwner(creditor->getName());
            payPlayer(creditor, myProperties.begin()->second);
        }
    }
    //  // todo: delete self from playersInGame
    oweBank = false;
    owePlayer = false;
    std::cout << name << " has successfully declared bankruptcy!" << std::endl;
    // need to sub cups
    g_timsCups--;
    // g_playersInGame.erase(piece);
}

bool Player::promptBankruptcy() {
    if (cash >= debtAmount) return false;

    int totalAssets = cash;
    for (auto it = myProperties.begin(); it != myProperties.end(); it++) {
        totalAssets += (it->second)->getPropertyValue();
    }

    if (creditor.get() == nullptr) return false;

    std::cout << name << " is in danger of bankrupcty. " << name
              << "'s two options are to declare bankruptcy or sell the "
              << "improvements on your buildings and mortgage them, "
              << "which would give you $" << totalAssets + cash
              << " to pay off your debt of $" << debtAmount << " to "
              << (oweBank ? "the Bank" : creditor->getName())
              << ". Do you want to sell the improvements on your buildings "
              << "and then mortgage them? (Y/N)" << std::endl;

    std::string response;
    while (true) {
        if (!(std::cin >> response)) {
            std::cin.clear();
            std::cin.ignore(256, '\n');
            response.clear();
            std::cout << name
                      << " , please type 'Y' if you would like to sell your "
                      << "stuff and not go bankrupt or 'N' if you would rather "
                      << "declare bankruptcy and exit the game (Y/N)"
                      << std::endl;
            continue;
        }
        if (response == "Y" || response == "N") {
            break;
        }
        std::cout << "Must type Y/N" << std::endl;
        response.clear();
    }

    // player chooses to sell their property's improvements and mortgage their
    // properties.
    if (response == "Y") {
        std::cout << name << "'s improvements will be sold and " << name
                  << "'s' buildings will be mortgaged." << std::endl;

        long long totalAssets = 0;
        for (auto &e : myProperties) {
            totalAssets += e.second->getPropertyValue();
        }
        if ((totalAssets + cash) < debtAmount) {
            std::cout << "GG" << std::endl;
            declareBankruptcy();
            return true;
        }
        if ((totalAssets + cash) >= debtAmount) {
            for (auto e = myProperties.begin(); e != myProperties.end(); e++) {
                // player's cash gets deducted in property.cc
                e->second->sellUpgrade();
                e->second->mortgage();
            }
            if (!hasCash(debtAmount)) declareBankruptcy();
            // if the creditor is the bank
            if (oweBank) {
                payBank(debtAmount);
            } else if (owePlayer) {
                // if the creditor is another player
                payPlayer(creditor, debtAmount);
            }
            owePlayer = false;
            oweBank = false;
            std::cout << name << " has avoided bankruptcy. Congratulations!"
                      << std::endl;
            return false;
        }
        std::cout << "After selling all improvements and mortgaging " << name
                  << "'s properties, " << name << " had $" << totalAssets
                  << " which is still $" << debtAmount - (totalAssets + cash)
                  << " short. " << std::endl;
    } else {  // player responded that they want to go bankrupt immediately
        std::cout << name << " has chosen to declare bankruptcy." << std::endl;
    }
    std::cout << "GG" << std::endl;
    declareBankruptcy();
    return true;
}
void Player::setCreditorToBank(int amount) {
    bool oweBank = true;
    debtAmount = amount;
}
void Player::setCreditorToPlayer(std::shared_ptr<Player> p, int amount) {
    owePlayer = true;
    creditor = p;
    debtAmount = amount;
}
void Player::setLocation(int location) { currentLocation = location; }

void Player::decTimsCup() { timsCups--; }

int Player::getTimsCups() { return timsCups; }

void Player::resetTimesInJail() { timesInJail = 0; }

bool Player::ownProperty(std::shared_ptr<Property> property) {
    return (myProperties.find(property->getName()) != myProperties.end());
}

bool Player::hasCash(int amount) { return cash >= amount; }