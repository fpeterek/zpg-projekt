//
// Created by fpeterek on 11.12.20.
//

#ifndef ZPG_PROJEKT_MOVEMENT_HPP
#define ZPG_PROJEKT_MOVEMENT_HPP

#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct MovementCurve {
    virtual glm::vec3 position(float t) const = 0;
    virtual glm::vec3 direction(float t) const = 0;
};

class Line : public MovementCurve {

    const glm::vec3 begin;
    const glm::vec3 end;
    const glm::vec3 dir;

public:
    Line(glm::vec3 begin, glm::vec3 end);

    glm::vec3 position(float t) const override;
    glm::vec3 direction(float t) const override;
};

class Circle : public MovementCurve {

    const glm::vec3 center;
    const float radius;
    const float r2;

    static float calcAngle(float t);

public:
    Circle(glm::vec3 center, float radius);

    glm::vec3 position(float t) const override;
    glm::vec3 direction(float t) const override;
};

class MovementCalculator {

    std::shared_ptr<MovementCurve> curve;
    const glm::vec3 offset;
    const float duration;
    float t = 0.0;

    float currentT() const;

public:

    MovementCalculator(std::shared_ptr<MovementCurve> curve, glm::vec3 rotationOffset, float duration);

    void update(float dt);
    glm::vec3 currentPosition() const;
    glm::vec3 currentHeading() const;
};

#endif //ZPG_PROJEKT_MOVEMENT_HPP
