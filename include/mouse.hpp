//
// Created by fpeterek on 14.10.20.
//

#ifndef ZPG_PROJEKT_MOUSE_HPP
#define ZPG_PROJEKT_MOUSE_HPP

#include <functional>
#include <cstdint>
#include <vector>
#include <memory>

#include "observer.hpp"

struct MouseData {
    int x;
    int y;
    int dx;
    int dy;
    uint8_t buttons;

    bool lbPressed() const;
    bool mbPressed() const;
    bool rbPressed() const;

    MouseData();
    MouseData(int x, int y, int dx, int dy, uint8_t buttons);

};

class Mouse : public Observable {

    static Mouse * mouse;

    int x = 0;
    int y = 0;
    uint8_t pressed = 0;

    MouseData current;

    Mouse();

public:

    enum class Button {
        LB = 0b1,
        MB = 0b10,
        RB = 0b100,
        Other = 0b1000
    };

    void buttonPress(Button button);
    void buttonRelease(Button button);

    void mouseMove(int x, int y);

    const MouseData & data() const;

    static Mouse & instance();
};

#endif //ZPG_PROJEKT_MOUSE_HPP
