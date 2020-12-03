//
// Created by fpeterek on 03.12.20.
//

#ifndef ZPG_PROJEKT_OBSERVER_HPP
#define ZPG_PROJEKT_OBSERVER_HPP

#include <vector>

enum class EventType {
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    LightChanged,
    CameraMoved
};

struct Observer {

    virtual void notify(EventType eventType, void * object) = 0;

};

class Observable {

    mutable std::vector<Observer *> observers;

public:

    void registerObserver(Observer & obs);
    void removeObserver(Observer & obs);
    void notify(EventType type, void * obj) const;

};

#endif //ZPG_PROJEKT_OBSERVER_HPP
