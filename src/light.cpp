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

void ColoredLight::addColor(glm::vec3 delta) {
    setColor(color + delta);
}

void ColoredLight::setColor(const glm::vec3 newColor) {
    color = boundColor(newColor);
    onColorChanged(LightType::Ambient);
}

void ColoredLight::addObserver(LightObserver & observer) {
    observers.emplace_back(std::reference_wrapper(observer));
}

void ColoredLight::onColorChanged(const LightType lightType) {
    for (auto obs : observers) {
        obs.get().colorChanged(color, lightType);
    }
}

void ColoredLight::apply() {
    for (auto obs : observers) {
        obs.get().colorChanged(color, LightType::Ambient);
    }
}

PositionedLight::PositionedLight() : ColoredLight() { }

PositionedLight::PositionedLight(const glm::vec3 color, const glm::vec3 position) : ColoredLight(color), position(position) { }

void PositionedLight::move(const glm::vec3 delta) {
    setPosition(position + delta);
}

void PositionedLight::setPosition(const glm::vec3 newPos) {
    position = newPos;
    onPositionChanged(LightType::Default);
}

void PositionedLight::setColor(const glm::vec3 newColor) {
    color = boundColor(newColor);
    onColorChanged(LightType::Default);
}

void PositionedLight::onPositionChanged(const LightType lightType) {
    for (auto obs : observers) {
        obs.get().positionChanged(position, lightType);
    }
}

void PositionedLight::apply() {
    for (auto obs : observers) {
        obs.get().colorChanged(color, LightType::Default);
        obs.get().positionChanged(position, LightType::Default);
    }
}
