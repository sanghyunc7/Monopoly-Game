#pragma once

#include <map>
#include <memory>
#include <string>

class Building;
class Property;
class NonProperty;
class Player;

class Bank {
    std::string piece;
    std::map<std::string, std::shared_ptr<Property>> myProperties;

   public:
    Bank();
    ~Bank();

    // no receive method
    bool payPlayer(std::shared_ptr<Player> p, int give);
    bool payPlayer(std::shared_ptr<Player> p, std::shared_ptr<Property> give);

    // Bank trades players, players only trade with other players
    bool trade(std::shared_ptr<Player> other, int give,
               std::shared_ptr<Property> receive);
    bool trade(std::shared_ptr<Player> other, std::shared_ptr<Property> give,
               int receive);

    // add/delete properties in myProperties
    bool incProperty(std::shared_ptr<Property> get);
    bool decProperty(std::shared_ptr<Property> pay);

    // auction methods
    void printAuctionRules();
    void beginAuction(std::string);  // need to change this in header

    // get members
    std::map<std::string, std::shared_ptr<Property>> getProperties();
    bool ownProperty(std::shared_ptr<Property>);  // TODO check before pay
    std::string getPiece();
    void initProperties(
        std::map<std::string, std::shared_ptr<Property>> &myProperties);
    void clear();
};