#ifndef BATTLESHIP_GAME_H
#define BATTLESHIP_GAME_H

#include "ship.h"
#include "ai_player.h"

namespace BattleShip {
    class game {
    private:
        int row_len, col_len;
        int ship_num;
        std::vector<BattleShip::ship> ships;

        BattleShip::player *player1;
        BattleShip::player *player2;
        int player1_type;
        int player2_type;

        static const int MODE_HVH = 1;
        static const int MODE_HVA = 2;
        static const int MODE_AVA = 3;

        static const int HUMAN = 0;
        static const int CHEATING_AI = 1;
        static const int RANDOM_AI = 2;
        static const int HUNT_DESTROY_AI = 3;

        int mode;

        int ask_get_ai_type();

        BattleShip::player *generate_player(int player_type);


    public:
        explicit game(char *path);

        void setup();

        void run();
    };
}

#endif //BATTLESHIP_GAME_H
