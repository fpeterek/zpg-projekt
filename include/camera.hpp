//
// Created by fpeterek on 06.10.20.
//

#ifndef ZPG_PROJEKT_CAMERA_HPP
#define ZPG_PROJEKT_CAMERA_HPP

#include <functional>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "enums.hpp"
#include "mouse.hpp"


class Camera : public Observer, public Observable {
    glm::mat4 projectMat = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 200.0f);

    glm::vec3 eye { 0.f, 1.f, 3.f };
    glm::vec3 target { 0.f };
    glm::vec3 up { 0.f, 1.f, 0.f };

    float fi = 1.5f * M_PI;
    float psi = 0.f;

    glm::vec2 movement { 0.0, 0.0 };
    int hRotate = 0;
    int vRotate = 0;

    bool changeMade = false;

    glm::mat4 camera;

    static constexpr float moveSpeed = 3.f;
    static constexpr float rotationSpeed = 1.f;
    static constexpr float dragSpeed = 0.3f;

    void updateCameraMatrix();
    void calcTarget();

    void capAngles();

    void updateAngle(float dt);

public:
    Camera();

    void setPosition(glm::vec3 pos);
    void move(glm::vec3 delta);

    void moveSideways(Direction dir);
    void moveForward(Direction dir);
    void rotateHor(Direction dir);
    void rotateVer(Direction dir);

    glm::vec2 movementVector(float dt) const;

    void update(float dt);

    void apply();

    void onMouseMove(const MouseData & md);
    void notify(EventType eventType, void * object) override;

    glm::mat4 view() const;
    glm::mat4 projection() const;
    glm::vec3 position() const;
};

#endif //ZPG_PROJEKT_CAMERA_HPP
