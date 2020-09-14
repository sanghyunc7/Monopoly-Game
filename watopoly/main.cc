#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bank.h"
#include "board.h"
#include "building.h"
#include "coop_fee.h"
#include "dice.h"
#include "go_to_tims.h"
#include "goose_nesting.h"
#include "needles_hall_normal.h"
#include "non_property.h"
#include "osap.h"
#include "player.h"
#include "property.h"
#include "roll_up_rim.h"
#include "slc_normal.h"
#include "tims.h"
#include "tuition.h"

Bank g_bank;
std::map<std::string, std::shared_ptr<Player>> g_playersInGame;
int g_timsCups;
// add timscups global variable

void rollDice(bool testing, std::shared_ptr<Dice> dice) {
    if (testing) {
        int first;
        int second;
        std::cout << "Enter first roll:" << std::endl;
        while (!(std::cin >> first)) {
            std::cout << "Number only. Enter first roll:" << std::endl;
        }
        std::cout << "Enter second roll:" << std::endl;
        while (!(std::cin >> second)) {
            std::cout << "Number only. Enter second roll:" << std::endl;
        }
        dice->roll(first, second);
    } else {
        dice->roll();
    }
    Rolls curRoll = dice->getCurRoll();
    std::cout << "You rolled " << curRoll.dice1 << " and " << curRoll.dice2
              << std::endl;
}

int main(int argc, char* argv[]) {
    srand(time(0));
    // call constructor for bank and g_playersGame

    std::ifstream infile;

    bool load = false;
    bool testing = false;

    std::string options[10];

    for (int i = 1; i < argc;
         i++) {  // we start at i = 1 because i = 0 is just the program name
        options[i - 1] = argv[i];
    }

    if (argc - 1 >= 2) {  // first index of argv is just the program name
        if (options[0] == "-load") {
            infile = std::ifstream(options[1]);

            if (!infile.is_open()) {
                std::cout << "ERROR: " << options[1]
                          << " cannot be opened for input" << std::endl;
                std::cout << "A new game will be used." << std::endl;
                load = false;
                testing = false;
            } else {
                load = true;
                if (argc - 1 >= 3 && options[2] == "-testing") testing = true;
            }
        }
    }
    if (argc - 1 == 1 && options[0] == "-testing") testing = true;

    std::vector<std::shared_ptr<Property>> properties;
    // construct bank
    g_bank = Bank();

    if (load) {
        std::cout << "load" << std::endl;

        std::string line;
        int numPlayers;
        std::getline(infile, line);
        std::istringstream ss{line};
        ss >> numPlayers;

        for (int i = 0; i < numPlayers; i++) {
            std::getline(infile, line);
            ss = std::istringstream{line};

            std::string name;
            ss >> name;
            std::string piece;
            ss >> piece;
            int timCups;
            ss >> timCups;
            int cash;
            ss >> cash;
            int position;
            ss >> position;
            int jail;
            ss >> jail;
            int timesInJail = 0;
            if (jail > 0) ss >> timesInJail;

            // construct player with empty vector of buildings
            // todo
            auto player = std::make_shared<Player>(name, piece);
            g_playersInGame.emplace(name, player);

            // g_playersInGame.insert(std::pair<std::string,
            // std::shared_ptr<Player>> 	(name,
            // std::make_shared<Player>(name, piece,
            // std::vector<std::shared_ptr<Property>>(), 		cash,
            // position, timCups, timesInJail, false, nullptr, 0)));
        }

        // now construct properties
        while (std::getline(infile, line)) {
            ss = std::istringstream{line};
            std::string name;
            ss >> name;
            std::string owner;
            ss >> owner;
            int improvements;
            ss >> improvements;

            properties.emplace_back(
                std::make_shared<Property>(name, owner, improvements, false));

            // this takes care of setting the owner of a property and adding the
            // property to the player's vector of properties
            for (auto& p : g_playersInGame) {
                if (p.first == owner) {
                    p.second->incProperty(properties[properties.size() - 1]);
                }
            }
            // bank owns the property, not a player
            if (owner == "BANK") {
                g_bank.incProperty(properties[properties.size() - 1]);
            }
        }
        infile.close();
    } else {  // load == false
        // call constructors for default versions of Player and Properties
        std::vector<std::string> names{
            "AL",  "ML",  "MKV", "ECH", "PAS",  "HH",  "RCH",
            "PAC", "DWE", "CPH", "UWP", "LHI",  "BMH", "OPT",
            "EV1", "EV2", "EV3", "V1",  "PHYS", "B1",  "CIF",
            "B2",  "EIT", "ESC", "C2",  "REV",  "MC",  "DC"};

        for (std::string& name : names) {
            properties.emplace_back(std::make_shared<Property>(name));
            g_bank.incProperty(
                properties[properties.size() - 1]);  // bank owns this property
        }
        std::map<std::string, std::string> playerTypes = {
            {"Goose", "G"},     {"GRT_Bus", "B"}, {"Tim_Hortons_Doughnut", "D"},
            {"Professor", "P"}, {"Student", "S"}, {"Money", "$"},
            {"Laptop", "L"},    {"Pink_tie", "T"}};

        while (true) {
            std::cout << "Pick an available name of the following(s):"
                      << std::endl;
            for (auto& it : playerTypes) {
                std::cout << it.first << std::endl;
            }
            std::string ans1;
            std::cin >> ans1;
            std::map<std::string, std::string>::iterator it;
            it = playerTypes.find(ans1);
            if (it != playerTypes.end()) {
                // if this player is available in the map

                auto player = std::make_shared<Player>(ans1, it->second);
                g_playersInGame.emplace(ans1, player);  // piece, player

                playerTypes.erase(ans1);  // this piece is now taken
            } else {
                std::cout << "That piece is not available" << std::endl;
            }
            std::cout << "Are you done choosing players? [Y/N]" << std::endl;
            std::cin >> ans1;
            while (ans1 != "Y" && ans1 != "N") {
                std::cout << "[Y/N] only." << std::endl;
                std::cin >> ans1;
            }
            if (ans1 == "Y") {
                if (g_playersInGame.size() < 2) {
                    std::cout << "Not enough players in the game. Pick more."
                              << std::endl;
                } else {
                    break;
                }
            }
            if (ans1 == "N") continue;
        }
    }

    // new vector of buildings
    std::vector<std::shared_ptr<Building>> buildings;
    // call constructor for non properties
    int i = 0;
    buildings.emplace_back(std::make_shared<OSAP>());
    buildings.emplace_back(properties[i++]);  // AL

    // predefined for SLC constructor
    std::shared_ptr<Building> tuition = std::make_shared<Tuition>();
    std::shared_ptr<Building> tims = std::make_shared<Tims>();
    std::shared_ptr<Building> gooseNesting = std::make_shared<GooseNesting>();
    std::shared_ptr<Building> gototims = std::make_shared<GoToTims>(tims);

    // SLC #1
    buildings.emplace_back(std::make_shared<SLCNormal>(
        properties[properties.size() - 1],  // DC, backward 3
        buildings[0],                       // OSAP, backward 2
        properties[i - 1],                  // AL, backward 1
        properties[i],                      // ML, forward 1
        tuition,                            // tuition, forward 2
        properties[i + 1],                  // MKV, forward 3
        tims,                               // DC Tims Line
        buildings[0]));                     // OSAP

    buildings.emplace_back(properties[i++]);  // ML
    buildings.emplace_back(tuition);
    buildings.emplace_back(properties[i++]);  // MKV
    buildings.emplace_back(properties[i++]);  // ECH
    buildings.emplace_back(std::make_shared<NeedlesHallNormal>());
    buildings.emplace_back(properties[i++]);  // PAS
    buildings.emplace_back(properties[i++]);  // HH
    buildings.emplace_back(tims);

    for (int j = 0; j < 6; j++) {  // RCH PAC DWE CPH UWP LHI
        buildings.emplace_back(properties[i++]);
    }

    // SLC  #2
    buildings.emplace_back(
        std::make_shared<SLCNormal>(properties[i - 3],  // CPH, backward 3
                                    properties[i - 2],  // UWP, backward 2
                                    properties[i - 1],  // LHI, backward 1
                                    properties[i],      // BMH, forward 1
                                    properties[i + 1],  // OPT, forward 2
                                    gooseNesting,       // forward 3
                                    tims,               // DC Tims Line
                                    buildings[0]));     // OSAP

    buildings.emplace_back(properties[i++]);  // BMH
    buildings.emplace_back(properties[i++]);  // OPT
    buildings.emplace_back(gooseNesting);     // Goose Nesting
    buildings.emplace_back(properties[i++]);  // EV1
    buildings.emplace_back(std::make_shared<NeedlesHallNormal>());

    for (int j = 0; j < 7; j++) {  // EV2 EV3 V1 PHYS B1 CIF B2
        buildings.emplace_back(properties[i++]);
    }
    buildings.emplace_back(gototims);
    buildings.emplace_back(properties[i++]);  // EIT
    buildings.emplace_back(properties[i++]);  // ESC
                                              // SLC  #3
    buildings.emplace_back(
        std::make_shared<SLCNormal>(gototims,  // Go To Tims, backward 3
                                    properties[i - 2],  // EIT, backward 2
                                    properties[i - 1],  // ESC, backward 1
                                    properties[i],      // C2, forward 1
                                    properties[i + 1],  // REV, forward 2
                                    gooseNesting,       // MKV, forward 3
                                    tims,               // DC Tims Line
                                    buildings[0]));     // OSAP

    buildings.emplace_back(properties[i++]);  // C2
    buildings.emplace_back(properties[i++]);  // REV
    buildings.emplace_back(std::make_shared<NeedlesHallNormal>());
    buildings.emplace_back(properties[i++]);  // MC
    buildings.emplace_back(std::make_shared<CoopFee>());
    buildings.emplace_back(properties[i++]);  // DC

    // done all constructor calls
    // now implement game logic

    Board board = Board(buildings);
    // board.printBoard();

    std::vector<std::string> players;
    for (auto& it : g_playersInGame) {
        players.emplace_back(it.first);
    }

    std::map<std::string, std::shared_ptr<Player>> playersfromLastRound =
        g_playersInGame;

    int playerTurn = 0;
    bool rolled = false;

    while (true) {  // break loop if one player left
        board.printBoard();

        std::shared_ptr<Dice> dice = std::make_shared<Dice>();

        // this handles when a player leaves the game
        std::map<std::string, std::shared_ptr<Player>>::iterator it;
        /*
        if (playersfromLastRound.size() > g_playersInGame.size()) {
                for (unsigned int j = 0; j < playersfromLastRound.size(); j++) {
                        it = g_playersInGame.find(players[j]);
                        if (it == g_playersInGame.end()) {
                                //player does not exist anymore


                                players.erase(players.begin() + j);
                                break; //out of for loop
        }
        */

        if (players.size() > g_playersInGame.size()) {
            // remove the player that is not in g_playersInGame
            for (unsigned int j = 0; j < players.size(); j++) {
                it = g_playersInGame.find(players[j]);
                if (it == g_playersInGame.end()) {
                    // player does not exist anymore
                    // remove player from players vector

                    players.erase(players.begin() + j);
                    break;  // out of for loop
                }
            }
        }
        playerTurn = playerTurn % g_playersInGame.size();
        std::shared_ptr<Player> curPlayer =
            g_playersInGame[players[playerTurn]];

        // determine winner
        if (g_playersInGame.size() <= 1) {
            std::cout << "Game over! " << curPlayer->getName() << " has won!"
                      << std::endl;
            break;  // out of while loop
        }

        std::cout << "It is currently " << curPlayer->getName()
                  << "'s turn to play" << std::endl;

        if (curPlayer->getTimesInJail() >= 1) {
            curPlayer->incTimesInJail();
        }

        std::string action;
        std::cout << "Choose your command:" << std::endl;
        std::cout << "[roll] [next] [trade <name> <give> <receive>] [improve "
                     "<property> buy/sell] "
                  << "[mortgage <property>] [unmortgage <property>] [bankrupt] "
                     "[assets] [all] "
                  << "[save <filename>]" << std::endl;

        std::cin >> action;

        if (action == "roll") {
            if (rolled) {
                std::cout << "You have already rolled." << std::endl;
                continue;
            }
            rolled = true;
            if (curPlayer->getTimesInJail() <= 0) {  // not in jail
                do {
                    rollDice(testing, dice);
                    buildings[curPlayer->getLocation()]->removePlayerFromTile(
                        curPlayer);
                    if (dice->getNumDoubles() >
                        2) {  // rolled three doubles, go to jail
                        curPlayer->incTimesInJail();
                        buildings[10]->accept(curPlayer, dice, buildings[10]);
                        curPlayer->setLocation(10);
                        dice->setNumDoubles(0);
                    } else {
                        int square =
                            curPlayer->getLocation() + dice->getCurTotal();
                        curPlayer->setLocation(square);
                        buildings[square]->accept(curPlayer, dice,
                                                  buildings[square]);
                    }
                    board.printBoard();
                } while (dice->getNumDoubles() > 0);
                continue;
            }

            // player wishes to roll but is in jail
            std::cout << "You have been in jail (including this turn) for "
                      << curPlayer->getTimesInJail() << std::endl;

            std::cout << "Get out by either [A] rolling double."
                      << "[B] pay $50, or [C] use a Roll Up the Rim cup."
                      << std::endl;
            std::string ans1;
            std::cin >> ans1;
            while (ans1 != "A" && ans1 != "B" && ans1 != "C") {
                std::cout << "Please answer only with characters [A/B/C]"
                          << std::endl;
                std::cin >> ans1;
            }
            if (ans1 == "A") {  // try to roll double
                rollDice(testing, dice);
                if (dice->getNumDoubles() > 0) {
                    buildings[10]->removePlayerFromTile(curPlayer);
                    curPlayer->resetTimesInJail();
                    int square = 10 + dice->getCurTotal();
                    curPlayer->setLocation(square);
                    buildings[square]->accept(curPlayer, dice,
                                              buildings[square]);
                    continue;
                } else {
                    std::cout << "Did not roll a double." << std::endl;
                    if (curPlayer->getTimesInJail() >= 3) ans1 = "B";
                }
            }
            if (ans1 == "C") {  // attempt to pay with tim cups
                if (curPlayer->getTimsCups() > 0) {
                    curPlayer->decTimsCup();
                    curPlayer->resetTimesInJail();
                    std::cout << "You can roll again." << std::endl;
                    rolled = false;
                    continue;
                } else {
                    std::cout << "You do not have any Tim Cups." << std::endl;
                    ans1 = "B";
                }
            }
            if (ans1 == "B") {  // pay $50
                std::cout << "You must pay $50" << std::endl;
                curPlayer->payBank(50);
                curPlayer->resetTimesInJail();
                std::cout << "You can roll again." << std::endl;
                rolled = false;
            }
            continue;  // end of roll action
        } else if (action == "next") {
            rolled = false;
            playerTurn++;
            continue;  // end of turn for current player
        } else if (action == "trade") {
            std::string name;
            std::string give;
            std::string receive;
            std::cin >> name >> give >> receive;

            if (g_playersInGame.find(name) == g_playersInGame.end()) {
                std::cout << "No such player exists" << std::endl;
                continue;
            }
            if (give == "0" || receive == "0") {
                std::cout << "You cannot receive/give something for free."
                          << std::endl;
                std::cout << "This is a capitalist's game." << std::endl;
                continue;
            }
            if (give == receive) {
                std::cout << "You cannot give and receive the same thing"
                          << std::endl;
                continue;
            }
            std::cout << "Does " + name + " [accept/reject]?" << std::endl;
            std::string ans1;
            std::cin >> ans1;
            while (ans1 != "accept" && ans1 != "reject") {
                std::cout << "Answer [accept/reject] only" << std::endl;
                std::cin >> ans1;
            }
            if (ans1 == "reject") continue;  // abort trade

            std::shared_ptr<Property> receiveP = nullptr;
            std::shared_ptr<Property> giveP = nullptr;
            int receiveI = 0;
            int giveI = 0;

            for (auto& p : properties) {
                if (p->getName() == give) {  // found
                    giveP = p;
                }
                if (p->getName() == receive) {
                    receiveP = p;
                }
            }

            if (receiveP == nullptr) {
                // try to get int
                try {
                    receiveI = std::stoi(receive);
                } catch (std::invalid_argument& ia) {
                    std::cout << "No such receive possible" << std::endl;
                    continue;
                }
            }
            if (giveP == nullptr) {
                try {
                    giveI = std::stoi(give);
                } catch (std::invalid_argument& ia) {
                    std::cout << "No such receive possible" << std::endl;
                    continue;
                }
            }
            if (receiveI && giveI) {
                std::cout << "You cannot trade money for money" << std::endl;
                continue;
            }
            if ((receiveI && receiveI <= 0) || (giveI && giveI <= 0)) {
                std::cout << "Money must be postive" << std::endl;
                continue;
            }
            if (receiveP && giveP) {
                curPlayer->trade(g_playersInGame[name], giveP, receiveP);
            } else if (receiveP && giveI) {
                curPlayer->trade(g_playersInGame[name], giveI, receiveP);
            } else if (receiveI && giveP) {
                curPlayer->trade(g_playersInGame[name], giveP, receiveI);
            } else {
                std::cout << "Invalid trade." << std::endl;
            }
            continue;
        } else if (action == "improve") {
            std::string property;
            std::string buyOrSell;
            std::cin >> property >> buyOrSell;

            // check if player owns property and buyorsell is "buy" or "sell"
            std::shared_ptr<Property> propP = nullptr;
            for (auto& p : properties) {
                if (p->getName() == property &&
                    p->getOwner() == curPlayer->getName()) {  // found
                    propP = p;
                    break;  // out of for loop
                }
            }
            if (propP && buyOrSell == "buy") {
                propP->upgrade();
            } else if (propP && buyOrSell == "sell") {
                propP->sellUpgrade();
            } else {
                std::cout << "Invalid command or you don't own this property."
                          << std::endl;
            }
            continue;
        } else if (action == "mortgage" || action == "unmortgage") {
            std::string property;
            std::cin >> property;
            std::shared_ptr<Property> propP = nullptr;
            for (auto& p : properties) {
                if (p->getName() == property &&
                    p->getOwner() == curPlayer->getName()) {  // found
                    propP = p;
                    break;  // out of for loop
                }
            }
            if (propP && action == "mortgage")
                propP->mortgage();
            else if (propP && action == "unmortgage")
                propP->unMortgage();
            else
                std::cout << "Invalid command or you don't own this property."
                          << std::endl;
            continue;
        } else if (action == "bankrupt") {
            bool temp = curPlayer->promptBankruptcy();
            if (temp) {  // player successful at going
                         // bankrupt
                buildings[curPlayer->getLocation()]->removePlayerFromTile(
                    curPlayer);
                curPlayer->declareBankruptcy();
                g_playersInGame.erase(curPlayer->getName());
                break;
            }
            std::cout << curPlayer->getName() << " cannot declare bankruptcy"
                      << std::endl;
            // players.erase(players.begin() + playerTurn);
            continue;
        } else if (action == "assets") {
            curPlayer->printAssets();
        } else if (action == "all") {
            for (unsigned int j = 0; j < players.size(); j++) {
                g_playersInGame[players[j]]->printAssets();
            }
        } else if (action == "save") {
            std::string filename;
            std::cin >> filename;

            std::ofstream outdata;
            outdata.open(filename);
            if (!outdata) {  // file cannot be opened
                std::cout << "Error, file cannot be opened." << std::endl;
                continue;
            }

            // save players
            outdata << g_playersInGame.size() << std::endl;
            for (unsigned int j = 0; j < g_playersInGame.size(); j++) {
                std::shared_ptr<Player> p = g_playersInGame[players[j]];

                outdata << p->getName() << " " << p->getPiece() << " "
                        << p->getTimsCups() << " " << p->getCash() << " "
                        << p->getLocation() << " ";
                if (p->getTimesInJail() <= 0) {
                    outdata << "0" << std::endl;
                } else {
                    outdata << "1 " << p->getTimesInJail() << std::endl;
                }
            }

            // save Properties
            for (auto& property : properties) {
                outdata << property->getName() << " " << property->getOwner()
                        << " " << property->getUpgrade() << std::endl;
            }

            outdata.close();
        }
    }
}
