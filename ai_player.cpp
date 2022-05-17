#include "ai_player.h"

int BattleShip::ai_player::ai_index = 1;

void BattleShip::ai_player::place_ship() {
    std::uniform_int_distribution<int> place_method_distribution{0, 1};
    std::uniform_int_distribution<int> row_distribution{0, this->board->getRowLen()};
    std::uniform_int_distribution<int> col_distribution{0, this->board->getColLen()};
    for (int i = 0; i < this->ship_num; i++) {
        show_board(true);
        string input;
        while (true) {
            int place_method = place_method_distribution(rand_generator);
            int row = row_distribution(rand_generator);
            int col = col_distribution(rand_generator);
            if (this->board->place_ship(row, col, place_method, this->ships[i].getSize(),
                                        this->ships[i].getType(), i)) {
                this->ships[i].setRow(row);
                this->ships[i].setCol(col);
                this->ships[i].setPlaceMethod(place_method);
                this->show_board(true);
                break;
            }
        }
    }
}

void BattleShip::ai_player::set_random_seed(int seed) {
    rand_generator = std::minstd_rand(seed);
}

vector<BattleShip::coord> BattleShip::ai_player::get_coords(int row_len, int col_len) {
    if (full_coords.size() != row_len * col_len) {
        int row_mi = row_len - 1;
        int col_mi = col_len - 1;
        if (full_coords.empty()) {
            for (int i = 0; i < row_mi + col_mi; ++i) {
                for (int j = std::max(0, i - row_mi); j < col_mi && j <= i; ++j) {
                    full_coords.emplace_back(i - j, j);
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
    while (target_crd_idx++ <= coords.size()) {
        auto crd = coords.at(target_crd_idx);
        if (opponent_board->get_status(crd.x, crd.y) == BattleShip::SHIP) {
            return crd;
        }
    }
    exit(-1);
}


BattleShip::random_ai_player::random_ai_player(int row_len, int col_len, int ship_num,
                                               const vector<BattleShip::ship> &ships) :
        ai_player(row_len, col_len, ship_num, ships), coords(get_coords(row_len, col_len)) {};

void BattleShip::random_ai_player::firing(BattleShip::player *opponent) {
    bool is_hit = false;
    auto target_crd = pop_random_coord(coords);
    opponent->getBoard()->hit(target_crd.x, target_crd.y, &is_hit);
    show_fire_result(opponent, target_crd.x, target_crd.y, is_hit);
}

BattleShip::coord BattleShip::random_ai_player::pop_random_coord(vector<coord> v) {
    auto itr = v.begin();
    std::uniform_int_distribution<int> distribution{0, static_cast<int>(v.size())};
    auto elementPos = distribution(rand_generator);
    std::advance(itr, elementPos);
    BattleShip::coord pick(*itr);
    v.erase(itr);
    return pick;
}

void BattleShip::hunt_destroy_ai_player::firing(BattleShip::player *opponent) {
    if (destroy_targets.empty())
        mode = HUNT_MODE;
    else
        mode = DESTROY_MODE;

    bool is_hit = false;
    coord target_crd = coord(0, 0);
    if (mode == HUNT_MODE) {
        target_crd = pop_random_coord(hunt_targets);
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
