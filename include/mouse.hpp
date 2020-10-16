//
// Created by fpeterek on 14.10.20.
//

#ifndef ZPG_PROJEKT_MOUSE_HPP
#define ZPG_PROJEKT_MOUSE_HPP

#include <functional>
#include <cstdint>
#include <vector>
#include <memory>

struct MouseData {
    const int x;
    const int y;
    const int dx;
    const int dy;
    const uint8_t buttons;

    bool lbPressed() const;
    bool mbPressed() const;
    bool rbPressed() const;

    MouseData(int x, int y, int dx, int dy, uint8_t buttons);

};


class MouseObserver {

public:

    MouseObserver();

    virtual void onButtonPress(const MouseData & mouseData);
    virtual void onButtonRelease(const MouseData & mouseData);
    virtual void onMouseMove(const MouseData & mouseData);
};


class Mouse {

    static Mouse * mouse;

    int x = 0;
    int y = 0;
    uint8_t pressed = 0;

    std::vector<std::reference_wrapper<MouseObserver>> observers;

    Mouse();

    enum class Action {
        Press,
        Release,
        Move,
        None
    };

    void notifyObservers(Action action, const MouseData & md);

public:

    enum class Button {
        LB = 0b1,
        MB = 0b10,
        RB = 0b100,
        Other = 0b1000
    };

    void registerObserver(MouseObserver & observer);

    void buttonPress(Button button);
    void buttonRelease(Button button);

    void mouseMove(int x, int y);

    static Mouse & instance();
};

#endif //ZPG_PROJEKT_MOUSE_HPP
