#ifndef BATTLESHIPAI_COORD_H
#define BATTLESHIPAI_COORD_H

namespace BattleShip {
    class coord {
    public:
        int x;
        int y;

        coord() : x(0), y(0) {}

        coord(int x, int y) : x(x), y(y) {};

        bool operator==(const coord &rhs) const {
            return x == rhs.x &&
                   y == rhs.y;
        }

        bool operator!=(const coord &rhs) const {
            return !(rhs == *this);
        }
    };
}

#endif //BATTLESHIPAI_COORD_H
