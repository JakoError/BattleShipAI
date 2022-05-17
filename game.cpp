#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "game.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

BattleShip::game::game(char *path) {
    player1 = player2 = nullptr;
    //读取文件
    string line;
    ifstream configfile(path);
    getline(configfile, line);
    row_len = (int) strtol(line.c_str(), nullptr, 10);
    getline(configfile, line);
    col_len = (int) strtol(line.c_str(), nullptr, 10);
    getline(configfile, line);
    ship_num = (int) strtol(line.c_str(), nullptr, 10);

    for (int i = 0; i < ship_num; ++i) {
        getline(configfile, line);
        vector<string> strs = split(line, " ");
        BattleShip::ship a_ship = BattleShip::ship(strs[0][0], (int) strtol(strs[1].c_str(), nullptr, 10));
        ships.push_back(a_ship);
    }
    configfile.close();
    //sort ship by character
    std::sort(ships.begin(), ships.end(), [](auto &&l, auto &&r) { return l.getType() < r.getType(); });
}

int BattleShip::game::ask_get_ai_type() {
    while (true) {
        string input;
        cout << "What AI do you want?" << endl
             << "1. Cheating AI" << endl
             << "2. Random AI" << endl
             << "3. Hunt Destroy AI" << endl
             << "Your choice: ";
        std::getline(cin, input);
        switch (strtol(input.c_str(), nullptr, 10)) {
            case CHEATING_AI:
                return CHEATING_AI;
            case RANDOM_AI:
                return RANDOM_AI;
            case HUNT_DESTROY_AI:
                return HUNT_DESTROY_AI;
            default:
                break;
        }
    }
}

BattleShip::player *BattleShip::game::generate_player(int player_type) {
    switch (player_type) {
        case HUMAN: {
            while (true) {
                string input;
                cout << "Player 1 please enter your name: ";
                std::getline(cin, input);
                if (!input.empty() && input.length() <= 20) {
                    return new BattleShip::player(row_len, col_len, ship_num, ships, input);
                }
            }
        }
        case CHEATING_AI: {
            return new BattleShip::cheat_ai_player(row_len, col_len, ship_num, ships);
        }
        case RANDOM_AI: {
            return new BattleShip::random_ai_player(row_len, col_len, ship_num, ships);
        }
        case HUNT_DESTROY_AI: {
            return new BattleShip::hunt_destroy_ai_player(row_len, col_len, ship_num, ships);
        }
    }
    return nullptr;
}


void BattleShip::game::setup() {

    while (true) {
        string input;
        cout << "What type of game do you want to play?" << endl
             << "1. Human vs Human" << endl
             << "2. Human vs AI" << endl
             << "3. AI vs AI" << endl
             << "Your choice: ";
        std::getline(cin, input);
        mode = strtol(input.c_str(), nullptr, 10);
        switch (mode) {
            case MODE_HVH:
                player1_type = HUMAN;
                player2_type = HUMAN;
                break;
            case MODE_HVA:
                player1_type = HUMAN;
                break;
            case MODE_AVA:
                player1_type = ask_get_ai_type();
                break;
            default:
                continue;
        }
        break;
    }
    player1 = generate_player(player1_type);
    player1->place_ship();

    switch (mode) {
        case MODE_HVA:
        case MODE_AVA:
            player2_type = ask_get_ai_type();
            break;
    }

    player2 = generate_player(player2_type);
    player2->place_ship();
}

void BattleShip::game::run() {
    //游戏循环
    while (true) {
        this->player1->firing(player2);
        if (player2->check_ships_destroy()) {
            cout << player1->getName() << " won the game!" << endl;
            break;
        }
        player2->firing(player1);
        if (player1->check_ships_destroy()) {
            cout << player2->getName() << " won the game!" << endl;
            break;
        }
    }
    cout << endl;
}