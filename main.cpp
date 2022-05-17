#include "game.h"

int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        printf("[format] ./BattleShip <path_of_config_file> <seed>\n");
        return 1;
    }
    if (argc == 3) {
        BattleShip::ai_player::set_random_seed(strtol(argv[2], nullptr, 10));
    }

    auto this_game = BattleShip::game(argv[1]);

    this_game.setup();
    this_game.run();

    return 0;
}
