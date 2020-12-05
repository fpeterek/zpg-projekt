//
// Created by fpeterek on 25.10.20.
//

#include "light.hpp"

#include <cmath>

static float boundColor(const float color) {
    return std::max(std::min(1.f, color), 0.f);
}

static glm::vec3 boundColor(const glm::vec3 color) {
    return { boundColor(color.x), boundColor(color.y), boundColor(color.z) };
}

ColoredLight::ColoredLight() = default;

ColoredLight::ColoredLight(const glm::vec3 color) : color(boundColor(color)) { }

void ColoredLight::addColor(glm::vec3 delta) const {
    setColor(color + delta);
}

void ColoredLight::setColor(const glm::vec3 newColor) const {
    color = boundColor(newColor);
    apply();
}

void ColoredLight::apply() const {
    notifyObservers(EventType::LightChanged, (void *) this);
}

gl::Light ColoredLight::type() const {
    return gl::Light::Ambient;
}

glm::vec3 ColoredLight::getColor() const {
    return color;
}

PositionedLight::PositionedLight() : ColoredLight() { }

PositionedLight::PositionedLight(const glm::vec3 color, const glm::vec3 position) : ColoredLight(color), position(position) { }

void PositionedLight::move(const glm::vec3 delta) const {
    setPosition(position + delta);
}

void PositionedLight::setPosition(const glm::vec3 newPos) const {
    position = newPos;
    apply();
}

void PositionedLight::setColor(const glm::vec3 newColor) const {
    color = boundColor(newColor);
    apply();
}

void PositionedLight::apply() const {
    notifyObservers(EventType::LightChanged, (void *) this);
}

gl::Light PositionedLight::type() const {
    return gl::Light::Point;
}

glm::vec3 PositionedLight::getPosition() const {
    return position;
}

DirectionalLight::DirectionalLight() : ColoredLight() { }

DirectionalLight::DirectionalLight(const glm::vec3 color, const glm::vec3 direction) :
    ColoredLight(color), direction(direction) { }

void DirectionalLight::move(const glm::vec3 delta) const {
    setDirection(direction + delta);
}

void DirectionalLight::setDirection(const glm::vec3 newDirection) const {
    direction = newDirection;
    apply();
}

void DirectionalLight::setColor(const glm::vec3 newColor) const {
    color = boundColor(newColor);
    apply();
}

void DirectionalLight::apply() const {
    notifyObservers(EventType::LightChanged, (void *) this);
}

gl::Light DirectionalLight::type() const {
    return gl::Light::Directional;
}

glm::vec3 DirectionalLight::getDirection() const {
    return direction;
}
