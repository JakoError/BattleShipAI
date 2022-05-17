#include "ai_player.h"

BattleShip::ai_player::ai_player(int row_len, int col_len, int ship_num, const vector<BattleShip::ship> &ships,
                                 const BattleShip::board *ai_board) : player(row_len, col_len, ship_num, ships,
                                                                             get_ai_name()) {

}

string BattleShip::ai_player::get_ai_name() {
    return *new string("AI ") + std::to_string(ai_index++);
}
