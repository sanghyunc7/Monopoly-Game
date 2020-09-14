#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

class Building;
class Property;
class NonProperty;
class Bank;

class Player {
    std::string name;
    std::string piece;
    std::map<std::string, std::shared_ptr<Property>> myProperties;

    long long cash;
    int currentLocation;
    int timsCups;
    int timesInJail;

    // bankruptcy methods
    bool oweBank;
    bool owePlayer;
    std::shared_ptr<Player> creditor;
    int debtAmount;

   public:
    Player(std::string name, std::string piece);

    Player(std::string const &name, std::string &piece,
           std::vector<std::shared_ptr<Property>> &myProperties, long long cash,
           int currentLocation, int timsCups, int timesInJail, bool oweBank,
           std::shared_ptr<Player> creditor, int debtAmount);

    ~Player();

    // Rules for payment:
    // 1. Players cannot trade cash for cash
    // 2. Bank can take Players's cash and give Players cash
    // 3. Players can trade cash for Property and Property for cash
    // 4. Players need to pay cash to other Players for tuition
    // 5. Players need to receive cash from other Players

    // pay a player cash, for external use
    bool payPlayer(std::shared_ptr<Player> p,
                   std::shared_ptr<Property> property);
    bool payPlayer(std::shared_ptr<Player> p, int paymentAmount);
    bool payBank(int paymentAmount);
    bool payBank(std::shared_ptr<Property> property);
    bool trade(std::shared_ptr<Player> other, int give,
               std::shared_ptr<Property> receive);
    bool trade(std::shared_ptr<Player> other, std::shared_ptr<Property> give,
               int receive);
    bool trade(std::shared_ptr<Player> other, std::shared_ptr<Property> give,
               std::shared_ptr<Property> receive);

    // add/delete properties in myProperties/Cash
    bool decProperty(std::shared_ptr<Property> property);
    bool incProperty(std::shared_ptr<Property> pay);
    bool decCash(int amount);
    bool incCash(int amount);

    // Bankruptcy methods used only when cash < debt
    void declareBankruptcy();
    bool promptBankruptcy();

    void incTimesInJail();
    void decTimsCup();

    void setCreditorToBank(int amount);
    void setCreditorToPlayer(std::shared_ptr<Player> p, int amount);
    void setLocation(int location);

    // get members
    std::map<std::string, std::shared_ptr<Property>> getProperties();
    int getLocation();
    std::string getName();
    std::string getPiece();
    long long getCash();
    int getTimesInJail();
    int getTimsCups();
    void resetTimesInJail();

    bool ownProperty(std::shared_ptr<Property>);
    bool hasCash(int amount);

    // Does not work if the player is deciding how to pay Tuition.
    void printAssets();
};