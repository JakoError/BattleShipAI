#ifndef BATTLESHIPAI_AI_PLAYER_H
#define BATTLESHIPAI_AI_PLAYER_H

#include "player.h"

namespace BattleShip {
    class ai_player : public player {
    private:
        static int ai_index;
    protected:
        BattleShip::board *ai_board;

        using player::player;
    public:
        ai_player(int row_len, int col_len, int ship_num, const vector<BattleShip::ship> &ships,
                  const BattleShip::board *ai_board);
        string get_ai_name();
    };

    int ai_player::ai_index = 1;

    class cheat_ai_player : public ai_player {
    public:
        cheat_ai_player();
    };
}

#endif //BATTLESHIPAI_AI_PLAYER_H
