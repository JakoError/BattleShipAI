#include "game.h"

int main(int argv, char **args) {
    if (argv != 2) {
        printf("Usge: BattleShip path_of_config_file\n");
        return 1;
    }

    auto this_game = BattleShip::game(args[1]);

    this_game.setup();
    this_game.run();

    return 0;
}
