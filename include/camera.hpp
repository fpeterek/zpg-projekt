//
// Created by fpeterek on 06.10.20.
//

#ifndef ZPG_PROJEKT_CAMERA_HPP
#define ZPG_PROJEKT_CAMERA_HPP

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "enums.hpp"

class Camera {
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    glm::vec3 eye { 0.f, 1.f, 3.f };
    glm::vec3 target { 0.f };
    glm::vec3 up { 0.f, 1.f, 0.f };

    float fi = 1.5f * M_PI;
    float psi = 0.f;

    int sidewaysMovement = 0;
    int forwardMovement = 0;
    int hRotate = 0;
    int vRotate = 0;

    glm::mat4 camera;

    Shader & shader;

    static constexpr float moveSpeed = 3.f;
    static constexpr float rotationSpeed = 1.f;

    void updateCameraMatrix();
    void calcTarget();

    void capAngles();

    void updateAngle(float dt);
    void updateForwardMovement(float dt);

public:
    explicit Camera(Shader & shader);

    void moveSideways(Direction dir);
    void moveForward(Direction dir);
    void rotateHor(Direction dir);
    void rotateVer(Direction dir);

    void update(float dt);

    void apply();
};

#endif //ZPG_PROJEKT_CAMERA_HPP
