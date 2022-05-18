#include <iostream>
#include "ai_player.h"

using std::cout;
using std::endl;

std::mt19937 BattleShip::ai_player::rand_generator;

int BattleShip::ai_player::ai_index = 1;

void BattleShip::ai_player::place_ship() {
    for (int i = 0; i < this->ship_num; i++) {
        string input;
        while (true) {
            std::uniform_int_distribution<int> place_method_distribution{0, 1};
            int place_method = place_method_distribution(ai_player::rand_generator);
            int row_max, col_max;
            if (place_method == BattleShip::HORIZONTAL) {
                row_max = this->board->getRowLen() - 1;
                col_max = this->board->getColLen() - this->ships[i].getSize();
            } else {
                row_max = this->board->getRowLen() - this->ships[i].getSize();
                col_max = this->board->getColLen() - 1;
            }
            std::uniform_int_distribution<int> row_distribution{0, row_max};
            std::uniform_int_distribution<int> col_distribution{0, col_max};
            int row = row_distribution(ai_player::rand_generator);
            int col = col_distribution(ai_player::rand_generator);
            if (this->board->place_ship(row, col, place_method, this->ships[i].getSize(),
                                        this->ships[i].getType(), i)) {
                this->ships[i].setRow(row);
                this->ships[i].setCol(col);
                this->ships[i].setPlaceMethod(place_method);
                cout << name << "'s Board" << endl;
                show_board(true);
                cout << endl;
                break;
            }
        }
    }
}

void BattleShip::ai_player::set_random_seed(int seed) {
    rand_generator = std::mt19937(seed);
}

vector<BattleShip::coord> BattleShip::ai_player::get_coords(int row_len, int col_len) {
    if (full_coords.size() != row_len * col_len) {
        if (full_coords.empty()) {
            for (int i = 0; i < row_len; ++i) {
                for (int j = 0; j < col_len; ++j) {
                    full_coords.emplace_back(i, j);
                }
            }
        }
    }
    return full_coords;
}

string BattleShip::ai_player::get_ai_name() {
    return *new string("AI ") + std::to_string(ai_index++);
}

void BattleShip::cheat_ai_player::firing(BattleShip::player *opponent) {
    auto target_crd = find_next_target(opponent->getBoard());
    opponent->getBoard()->hit(target_crd.x, target_crd.y, nullptr);
    show_fire_result(opponent, target_crd.x, target_crd.y, true);
}

BattleShip::coord BattleShip::cheat_ai_player::find_next_target(BattleShip::board *opponent_board) {
    auto coords = get_coords(opponent_board->getRowLen(), opponent_board->getColLen());
    if (target_crd_idx > coords.size())
        target_crd_idx = 0;
    while (target_crd_idx < coords.size()) {
        auto crd = coords.at(target_crd_idx);
        if (opponent_board->get_status(crd.x, crd.y) == BattleShip::SHIP) {
            return crd;
        }
        target_crd_idx++;
    }
    exit(-1);
}


BattleShip::random_ai_player::random_ai_player(int row_len, int col_len, int ship_num,
                                               const vector<BattleShip::ship> &ships) :
        ai_player(row_len, col_len, ship_num, ships), coords(get_coords(row_len, col_len)) {}

void BattleShip::random_ai_player::firing(BattleShip::player *opponent) {
    bool is_hit = false;
    auto target_crd = pop_random_coord(&coords);
    opponent->getBoard()->hit(target_crd.x, target_crd.y, &is_hit);
    show_fire_result(opponent, target_crd.x, target_crd.y, is_hit);
}

BattleShip::coord BattleShip::random_ai_player::pop_random_coord(vector<coord> *v) {
    auto itr = v->begin();
    std::uniform_int_distribution<int> distribution{0, static_cast<int>(v->size() - 1)};
    auto elementPos = distribution(ai_player::rand_generator);
    std::advance(itr, elementPos);
    BattleShip::coord pick(*itr);
    v->erase(itr);
    return pick;
}

void BattleShip::hunt_destroy_ai_player::firing(BattleShip::player *opponent) {
    if (destroy_targets.empty())
        mode = HUNT_MODE;
    else
        mode = DESTROY_MODE;

    bool is_hit = false;
    coord target_crd;
    if (mode == HUNT_MODE) {
        target_crd = pop_random_coord(&hunt_targets);
        opponent->getBoard()->hit(target_crd.x, target_crd.y, &is_hit);
        show_fire_result(opponent, target_crd.x, target_crd.y, is_hit);
    } else if (mode == DESTROY_MODE) {
        target_crd = *destroy_targets.begin();
        destroy_targets.erase(destroy_targets.begin());
        opponent->getBoard()->hit(target_crd.x, target_crd.y, &is_hit);
        show_fire_result(opponent, target_crd.x, target_crd.y, is_hit);
    } else {
        exit(-1);
    }
    if (is_hit) {
        auto iter = std::find(hunt_targets.begin(), hunt_targets.end(), coord(target_crd.x, target_crd.y - 1));
        if (iter != hunt_targets.end()) {
            destroy_targets.push_back(*iter);
            hunt_targets.erase(iter);
        }
        iter = std::find(hunt_targets.begin(), hunt_targets.end(), coord(target_crd.x - 1, target_crd.y));
        if (iter != hunt_targets.end()) {
            destroy_targets.push_back(*iter);
            hunt_targets.erase(iter);
        }
        iter = std::find(hunt_targets.begin(), hunt_targets.end(), coord(target_crd.x, target_crd.y + 1));
        if (iter != hunt_targets.end()) {
            destroy_targets.push_back(*iter);
            hunt_targets.erase(iter);
        }
        iter = std::find(hunt_targets.begin(), hunt_targets.end(), coord(target_crd.x + 1, target_crd.y));
        if (iter != hunt_targets.end()) {
            destroy_targets.push_back(*iter);
            hunt_targets.erase(iter);
        }
    }
}
