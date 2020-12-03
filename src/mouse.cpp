//
// Created by fpeterek on 14.10.20.
//

#include "mouse.hpp"

#include <algorithm>

Mouse * Mouse::mouse = nullptr;

bool MouseData::lbPressed() const {
    return buttons & (uint8_t)Mouse::Button::LB;
}

bool MouseData::mbPressed() const {
    return buttons & (uint8_t)Mouse::Button::MB;
}

bool MouseData::rbPressed() const {
    return buttons & (uint8_t)Mouse::Button::RB;
}

MouseData::MouseData(const int x, const int y, const int dx, const int dy, const uint8_t buttons) :
    x(x), y(y), dx(dx), dy(dy), buttons(buttons) { }

MouseData::MouseData() :
x(0), y(0), dx(0), dy(0), buttons(0) { }


void Mouse::buttonPress(const Button button) {
    pressed |= (uint8_t)button;

    current = MouseData(x, y, 0, 0, pressed);

    notify(EventType::MouseButtonPressed, this);
}

void Mouse::buttonRelease(const Button button) {
    pressed &= ~(uint8_t)button;

    current = MouseData(x, y, 0, 0, pressed);

    notify(EventType::MouseButtonReleased, this);
}

Mouse & Mouse::instance() {
    if (not mouse) {
        mouse = new Mouse();
    }
    return *mouse;
}

void Mouse::mouseMove(const int nx, const int ny) {

    const int dx = nx - x;
    const int dy = ny - y;

    x = nx;
    y = ny;

    current = MouseData(x, y, dx, dy, pressed);

    notify(EventType::MouseMoved, this);
}

const MouseData & Mouse::data() const {
    return current;
}

Mouse::Mouse() = default;
