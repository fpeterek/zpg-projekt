//
// Created by fpeterek on 12.12.20.
//

#include "movement.hpp"

/* -------------------- Line -------------------- */

Line::Line(glm::vec3 begin, glm::vec3 end) : begin(begin), end(end), dir(glm::normalize(end - begin)) { }

static glm::vec3 operator*(const glm::vec3 vec, const float f) {
    return { vec.x * f, vec.y * f, vec.z * f };
}

glm::vec3 Line::position(const float t) const {
    return begin * (1.f - t) + end * t;
}

glm::vec3 Line::direction(const float t) const {
    (void)t;
    return dir;
}

/* -------------------- Circle -------------------- */

float Circle::calcAngle(const float t) {
    constexpr float twopi = 2.f * M_PI;
    return twopi * t;
}

Circle::Circle(const glm::vec3 center, const float radius) : center(center), radius(radius), r2(radius*radius) { }

glm::vec3 Circle::position(const float t) const {
    const float angle = calcAngle(t);
    const float x = center.x + std::cos(glm::radians(angle)) * radius;
    const float y = center.y;
    const float z = center.z + std::sin(glm::radians(angle)) * radius;
    return { x, y, z };
}

glm::vec3 Circle::direction(const float t) const {
    const float angle = calcAngle(t);
    const float x = -1 * std::sin(angle);
    const float z = std::cos(angle);
    return { x, 0.f, z };
}

/* -------------------- Calculator -------------------- */

MovementCalculator::MovementCalculator(std::shared_ptr<MovementCurve> curve, glm::vec3 rotationOffset, float duration) :
    curve(std::move(curve)), offset(rotationOffset), duration(duration) { }

float MovementCalculator::currentT() const {
    return 1.f / t;
}

void MovementCalculator::update(float dt) {
    t = std::fmod(t+dt, duration);
}

glm::vec3 MovementCalculator::currentPosition() const {
    return curve->position(currentT());
}

glm::vec3 MovementCalculator::currentHeading() const {
    return curve->direction(currentT()) + offset;
}
