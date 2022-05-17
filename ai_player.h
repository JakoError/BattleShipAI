#ifndef BATTLESHIPAI_AI_PLAYER_H
#define BATTLESHIPAI_AI_PLAYER_H

#include <random>

#include "coord.h"
#include "player.h"


namespace BattleShip {
    static std::minstd_rand rand_generator;

    /**
     * AI player
     */
    class ai_player : public player {
    private:
        static int ai_index;
    protected:

        /**
         * static coord vector to store upper leftmost conner to bottom right most corner backup
         */
        vector<BattleShip::coord> full_coords;
        using player::player;

        vector<BattleShip::coord> get_coords(int row_len, int col_len);

    public:
        ai_player(int row_len, int col_len, int ship_num, const vector<BattleShip::ship> &ships) :
                player(row_len, col_len, ship_num, ships, get_ai_name()) {};

        static void set_random_seed(int seed);

        static string get_ai_name();

        void place_ship() override;
    };

    /**
     * Cheating AI
     */
    class cheat_ai_player : public ai_player {
    private:
        unsigned int target_crd_idx;

    protected:
        using ai_player::ai_player;

        coord find_next_target(BattleShip::board *opponent_board);

    public:
        cheat_ai_player(int row_len, int col_len, int ship_num, const vector<BattleShip::ship> &ships) : ai_player(
                row_len, col_len, ship_num, ships), target_crd_idx(0) {};

        void firing(BattleShip::player *opponent) override;
    };

    /**
     * Random AI
     */
    class random_ai_player : public ai_player {
    protected:
        using ai_player::ai_player;
        vector<BattleShip::coord> coords;

        static coord pop_random_coord(vector<coord> v);

    private:

    public:
        random_ai_player(int row_len, int col_len, int ship_num, const vector<BattleShip::ship> &ships);

        void firing(BattleShip::player *opponent) override;

    };

    /**
     * Hunt Destroy AI
     */
    class hunt_destroy_ai_player : public random_ai_player {
    protected:
        using random_ai_player::random_ai_player;
        static const int HUNT_MODE = 0;
        static const int DESTROY_MODE = 1;
        vector<coord> hunt_targets;
        vector<coord> destroy_targets;
        int mode;
    public:
        hunt_destroy_ai_player(int row_len, int col_len, int ship_num, const vector<BattleShip::ship> &ships)
                : random_ai_player(row_len, col_len, ship_num, ships), mode(HUNT_MODE),
                  hunt_targets(get_coords(row_len, col_len)) {};

        void firing(BattleShip::player *opponent) override;
    };
}

#endif //BATTLESHIPAI_AI_PLAYER_H
