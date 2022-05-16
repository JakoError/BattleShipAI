#ifndef BATTLESHIP_GAME_H
#define BATTLESHIP_GAME_H

#include "player.h"
#include "ship.h"

namespace BattleShip {
    class game {
    private:
        int row_len, col_len;
        int ship_num;
        std::vector<BattleShip::ship> ships;

        BattleShip::player *player1;
        BattleShip::player *player2;

    public:
        explicit game(char *path);

        void setup();

        void run();
    };
}

#endif //BATTLESHIP_GAME_H
