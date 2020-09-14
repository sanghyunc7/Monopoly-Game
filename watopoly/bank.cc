#include "bank.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "player.h"
#include "property.h"

extern std::map<std::string, std::shared_ptr<Player>> g_playersInGame;
static bool DEBUG = true;

Bank::Bank() : piece("Bank") {}
Bank::~Bank() { myProperties.clear(); }

bool Bank::payPlayer(std::shared_ptr<Player> player, int paymentAmount) {
    if (paymentAmount < 0) {
        std::cout << "Bank to " << player->getName() << " $" << paymentAmount
                  << " payment failed. " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!player->incCash(paymentAmount)) {
        if (DEBUG) {
            std::cout << "The bank's payment of $" << paymentAmount << " to "
                      << player->getName() << " was not successful."
                      << std::endl;
        }
        return false;
    }
    if (DEBUG) {
        std::cout << "The Bank's payment of $" << paymentAmount << " to "
                  << player->getName() << " was successful." << std::endl;
    }
    return true;
}
bool Bank::payPlayer(std::shared_ptr<Player> player,
                     std::shared_ptr<Property> property) {
    if (!ownProperty(property)) {
        std::cout << "Bank to " << player->getName() << " "
                  << property->getName() << " pay failed, does not own. "
                  << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!decProperty(property)) {
        if (DEBUG) {
            std::cout << "The bank's payment of " << property->getName()
                      << " to " << player->getName() << " was not successful."
                      << std::endl;
        }
        return false;
    }
    if (!player->incProperty(property)) {
        if (DEBUG) {
            std::cout << "The bank's payment of " << property->getName()
                      << " to " << player->getName() << " was not successful."
                      << std::endl;
        }
        incProperty(property);
        return false;
    }
    property->setOwner(player->getName());  // TODO must change owner
    if (DEBUG) {
        std::cout << "The Bank's payment of " << property->getName() << " to "
                  << player->getName() << " was successful." << std::endl;
    }
    return true;
}

bool Bank::trade(std::shared_ptr<Player> other, int give,
                 std::shared_ptr<Property> receive) {
    if (give < 0) {
        std::cout << "Bank payment"
                  << " to " << other->getName() << " $" << give
                  << " trade failed. " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->ownProperty(receive)) {
        std::cout << other->getName() << " to the Bank " << receive->getName()
                  << " trade failed, does not own. " << __FILE__ << __LINE__
                  << std::endl;
        return false;
    }
    // Avoid call payPlayer cach, may trigger bankrupt
    if (!other->incCash(give)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!incProperty(receive)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!other->decProperty(receive)) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        return false;
    }
    if (!payPlayer(other, give)) {
        payPlayer(other, receive);
        std::cout << "Sorry you do not have enough funds to support this trade."
                  << std::endl;
        return false;
    }
    receive->setOwner("BANK");
    if (DEBUG) {
        std::cout << "Bank receive " << receive->getName() << " from and give $"
                  << give << " to " << other->getName() << " succeeded."
                  << std::endl;
    }
    std::cout << "Successful transaction!" << std::endl;
    return true;
}

bool Bank::trade(std::shared_ptr<Player> other, std::shared_ptr<Property> give,
                 int receive) {
    if (receive < 0) {
        std::cout << other->getName() << " to "
                  << "the Bank"
                  << " $" << receive << " trade failed. " << __FILE__
                  << __LINE__ << std::endl;
        return false;
    }
    if (!ownProperty(give)) {
        std::cout << "The Bank"
                  << " to " << other->getName() << " " << give->getName()
                  << " trade failed, does not own. " << __FILE__ << __LINE__
                  << std::endl;
        return false;
    }
    if (!other->hasCash(receive)) {
        std::cout << other->getName() << " does not have enough money to pay $"
                  << receive << " to "
                  << "BANK" << std::endl;
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
    give->setOwner(other->getName());
    if (DEBUG) {
        std::cout << "The Bank"
                  << " give " << give->getName() << " to and receive $"
                  << receive << " from " << other->getName() << " succeeded."
                  << std::endl;
    }
    std::cout << "Successful transaction!" << std::endl;
    return true;
}

// add/delete properties in myProperties
bool Bank::incProperty(std::shared_ptr<Property> property) {
    auto res = myProperties.emplace(property->getName(), property);
    if (!res.second) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        exit(1);
    }
    return true;
}

bool Bank::decProperty(std::shared_ptr<Property> property) {
    int ret = myProperties.erase(property->getName());
    if (1 != ret) {
        std::cout << "SHOULD NOT REACH " << __FILE__ << __LINE__ << std::endl;
        exit(1);
    }
    return true;
}

// auction methods
void Bank::printAuctionRules() {
    std::cout << "The rules are that the highest bidder wins. You'll take "
              << "turns bidding. To bid, type amount of cash you would like to "
              << "bid. To exit the auction, type any integer amount less "
              << "than the current bid." << std::endl;
}

void Bank::beginAuction(std::string propertyName) {
    printAuctionRules();
    // std::cin.exceptions(std::ios::eofbit | std::ios::failbit |
    //                     std::ios::badbit);
    std::string cmd;

    std::cout << "Type 'auction-info' to get rules of the auction and what "
              << "commands you can use to bid." << std::endl;

    auto prop = myProperties.find(propertyName);  // find property in bank
    if (prop == myProperties.end()) return;
    int highestBid = (prop->second)->getPrice();

    // needs to be global or passed in
    std::map<std::string, std::shared_ptr<Player>> playersInAuction =
        g_playersInGame;

    std::string winner;
    while (playersInAuction.size() > 1) {
        int money = 0;
        for (auto it = playersInAuction.begin();
             it != playersInAuction.end();) {
            if (winner == it->first) {
                it++;
                continue;
            }  // highest bidder's turn
            std::cout << "It's " << it->first
                      << "'s turn to bid. Can you match the highest bid of "
                      << highestBid << "?" << std::endl;

            while (!(std::cin >> money)) {
                std::cout
                    << "Sorry, that wasn't an integer! Please enter again!"
                    << std::endl;
                std::cin.clear();
                std::cin.ignore(256, '\n');
                money = 0;
            }

            if (money > (it->second)->getCash()) {
                std::cout << (it->second)->getName()
                          << " doesn't have enough money to bid $" << highestBid
                          << ". Skip turn." << std::endl;
                ++it;
                continue;
            }

            if (money <= highestBid) {
                std::cout << "Sorry that bid isn't high enough. "
                          << (it->second)->getName() << " have "
                          << "exited the auction." << std::endl;
                it = playersInAuction.erase(it);
                continue;
            }
            highestBid = money;
            winner = (it->second)->getName();
            std::cout << (it->second)->getName()
                      << " has successfully raised the bid!" << std::endl;
            if (playersInAuction.size() == 1) break;
            ++it;
        }
    }
    // enforce the transaction of the player who bid
    if (winner.empty()) {
        std::cout << "There was no winner of the auction." << std::endl;
        return;
    }
    auto property = myProperties.find(propertyName);
    if (property == myProperties.end()) {
        std::cout << "Unreachable: " << __LINE__
                  << "Bank doesn't own this property" << std::endl;
        return;
    }
    // (property->second)->setOwner(winner);
    trade(g_playersInGame.find(winner)->second,
          myProperties.find(propertyName)->second, highestBid);
    std::cout << winner << " has won the auction for " << propertyName
              << " for $" << highestBid << std::endl;
}

// get members
std::map<std::string, std::shared_ptr<Property>> Bank::getProperties() {
    return myProperties;
}

std::string Bank::getPiece() { return piece; }

void Bank::initProperties(
    std::map<std::string, std::shared_ptr<Property>>& mp) {
    myProperties = mp;
}

void Bank::clear() { myProperties.clear(); }

bool Bank::ownProperty(std::shared_ptr<Property> property) {
    return (myProperties.find(property->getName()) != myProperties.end());
}