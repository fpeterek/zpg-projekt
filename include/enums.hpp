//
// Created by fpeterek on 05.10.20.
//

#ifndef ZPG_PROJEKT_ENUMS_HPP
#define ZPG_PROJEKT_ENUMS_HPP

enum class Growth {
    none = 0,
    grow = 1,
    shrink = -1
};

enum class Rotation {
    none = 0,
    left = 1,
    right = -1
};

enum class Direction {
    none = 0,
    up = 1,
    down = -1,
    left = -1,
    right = 1
};

#endif //ZPG_PROJEKT_ENUMS_HPP
