//
// Created by fpeterek on 30.09.20.
//

#ifndef ZPG_PROJEKT_OBJECT_HPP
#define ZPG_PROJEKT_OBJECT_HPP

#include <vector>
#include <functional>

#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include "model.hpp"
#include "enums.hpp"
#include "shader.hpp"
#include "texture.hpp"


class Object {

    glm::mat4 transform { 1.f };

    Growth growthDir = Growth::none;
    Rotation rotationDir = Rotation::none;
    glm::vec2 movementDir { 0.f, 0.f };

    glm::vec3 color = defaultColor;

    std::reference_wrapper<Shader> shader;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Model> model;

    unsigned int id;

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

    static unsigned int objectCount;

    static unsigned int getNextId();

    void updateGrowth(float dt);
    void updateRotation(float dt);
    void updateForces(float dt);

public:

    Object(std::shared_ptr<Model> model, Shader & shader, std::shared_ptr<Texture> texture);

    void draw() const;

    const glm::mat4 & transformation() const;
    unsigned int objectId() const;

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

    void setColor(glm::vec3 color);
    void setColor(float r, float g, float b);

    void update(double dt);

    static const glm::vec3 defaultColor;
    static const glm::vec3 secondaryColor;

    class Builder {

        std::shared_ptr<Model> model;
        Shader * shader = nullptr;
        std::shared_ptr<Texture> texture;

        float degree = 0.f;
        glm::vec3 rotationAxis { 0.f };
        glm::vec3 position { 0.f };
        glm::vec3 scales { 1.f };
        glm::vec3 color = defaultColor;

        void reset();

    public:
        Builder & setModel(std::shared_ptr<Model> model);
        Builder & setShader(Shader & shader);
        Builder & setTexture(std::shared_ptr<Texture> texture);
        Builder & emplaceObject(std::shared_ptr<Model> model, Shader & shader, std::shared_ptr<Texture> texture);
        Builder & setRotation(float degree, glm::vec3 axis);
        Builder & setPosition(glm::vec3 position);
        Builder & setPosition(float x, float y, float z);
        Builder & setScale(glm::vec3 scales);
        Builder & setScale(float x, float y, float z);
        Builder & setColor(glm::vec3 color);
        Builder & setColor(float r, float g, float b);

        Object build();

    };

};

#endif //ZPG_PROJEKT_OBJECT_HPP
