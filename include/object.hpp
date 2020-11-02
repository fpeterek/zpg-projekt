//
// Created by fpeterek on 30.09.20.
//

#ifndef ZPG_PROJEKT_OBJECT_HPP
#define ZPG_PROJEKT_OBJECT_HPP

#include <vector>

#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include "model.hpp"
#include "enums.hpp"
#include "shader.hpp"

class Object {

    glm::mat4 transform{1.f};
    const Model & model;

    Growth growthDir = Growth::none;
    Rotation rotationDir = Rotation::none;
    glm::vec2 movementDir { 0.f, 0.f };

    Shader & shader;

    float rotationV = 0;
    glm::vec2 forces { 0.f, 0.f };

    static constexpr float growthV = 1.0;

    static constexpr float rotationAcc = 1.0;
    static constexpr float rotationDec = 0.5;
    static constexpr float rotationMax = 10.0;

    static constexpr float acceleration = 1.0;
    static constexpr float deceleration = 0.5;
    static constexpr float maxV = 10.0;

    static float getAcc(Direction dir);

    static float calcNewV(float current, float acc, float dec);
    static float capV(float current, float max);

    void updateGrowth(float dt);
    void updateRotation(float dt);
    void updateForces(float dt);

public:

    Object(const Model & model, Shader & shader);

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

class ObjectBuilder {

    const Model * model = nullptr;
    Shader * shader = nullptr;

    float degree = 0.f;
    glm::vec3 rotationAxis { 0.f };
    glm::vec3 position { 0.f };
    glm::vec3 scales { 1.f };

    void reset();

public:
    ObjectBuilder & setModel(const Model & model);
    ObjectBuilder & setShader(Shader & shader);
    ObjectBuilder & emplaceObject(const Model & model, Shader & shader);
    ObjectBuilder & setRotation(float degree, glm::vec3 axis);
    ObjectBuilder & setPosition(glm::vec3 position);
    ObjectBuilder & setPosition(float x, float y, float z);
    ObjectBuilder & setScale(glm::vec3 scales);
    ObjectBuilder & setScale(float x, float y, float z);

    Object build();

};

#endif //ZPG_PROJEKT_OBJECT_HPP
