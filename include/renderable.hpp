//
// Created by fpeterek on 30.09.20.
//

#ifndef ZPG_PROJEKT_RENDERABLE_HPP
#define ZPG_PROJEKT_RENDERABLE_HPP

#include <vector>

#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include "enums.hpp"

class Renderable {

    glm::mat4 transform{1.f};
    GLuint vbo = 0;
    GLuint vao = 0;

    Growth growthDir = Growth::none;
    Rotation rotationDir = Rotation::none;
    glm::vec2 movementDir { 0.f, 0.f };

    float rotationV = 0;
    glm::vec2 forces { 0.f, 0.f };

    static constexpr float growthV = 1.0;

    static constexpr float rotationAcc = 1.0;
    static constexpr float rotationDec = 0.5;
    static constexpr float rotationMax = 10.0;

    static constexpr float acceleration = 1.0;
    static constexpr float deceleration = 0.5;
    static constexpr float maxV = 10.0;

    const std::vector<float> points;

    void initVbo();
    void initVao();

    static float getAcc(Direction dir);

    static float calcNewV(float current, float acc, float dec);
    static float capV(float current, float max);

    void updateGrowth(float dt);
    void updateRotation(float dt);
    void updateForces(float dt);

public:

    explicit Renderable(std::vector<float> points);

    void draw() const;

    const glm::mat4 & transformation() const;

    void rotate(float degree, glm::vec3 axis);
    void translate(glm::vec3 delta);
    void scale(glm::vec3 scales);

    void applyForce(Direction dx, Direction dy);
    void applyFx(Direction dir);
    void applyFy(Direction dir);
    void enableGrowth(Growth growth);
    void disableGrowth();
    void enableRotation(Rotation rotation);
    void disableRotation();

    void update(double dt);

};

#endif //ZPG_PROJEKT_RENDERABLE_HPP
