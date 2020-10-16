//
// Created by fpeterek on 14.10.20.
//

#include "mouse.hpp"


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


void Mouse::buttonPress(const Button button) {
    pressed |= (uint8_t)button;

    const auto md = MouseData(x, y, 0, 0, pressed);

    notifyObservers(Action::Press, md);
}

void Mouse::buttonRelease(const Button button) {
    pressed &= ~(uint8_t)button;

    const auto md = MouseData(x, y, 0, 0, pressed);

    notifyObservers(Action::Release, md);
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

    const auto md = MouseData(x, y, dx, dy, pressed);

    notifyObservers(Action::Move, md);
}

void Mouse::registerObserver(MouseObserver & observer) {
    observers.emplace_back(observer);
}

void Mouse::notifyObservers(const Mouse::Action action, const MouseData & md) {
    for (auto & obs : observers) {
        if (action == Action::Move) {
            obs.get().onMouseMove(md);
        } else if (action == Action::Press) {
            obs.get().onButtonPress(md);
        } else if (action == Action::Release) {
            obs.get().onButtonRelease(md);
        }
    }
}

Mouse::Mouse() = default;

MouseObserver::MouseObserver() {
    Mouse::instance().registerObserver(*this);
}

void MouseObserver::onButtonPress(const MouseData & mouseData) {
    // By default do nothing
}

void MouseObserver::onButtonRelease(const MouseData & mouseData) {
    // By default do nothing
}

void MouseObserver::onMouseMove(const MouseData & mouseData) {
    // By default do nothing
}
