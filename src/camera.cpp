//
// Created by fpeterek on 10.10.20.
//

#include "camera.hpp"

#include <cmath>

Camera::Camera() {
    updateCameraMatrix();
}

void Camera::updateCameraMatrix() {
    camera = glm::lookAt(eye, eye+target, up);
}

void Camera::apply() {
    notifyObservers(EventType::CameraMoved, this);
}

void Camera::moveSideways(Direction dir) {
    movement.y = (int)dir;
}

void Camera::moveForward(Direction dir) {
    movement.x = (int)dir;
}

void Camera::rotateHor(Direction dir) {
    hRotate = (int)dir;
}

void Camera::rotateVer(Direction dir) {
    vRotate = (int)dir;
}

void Camera::updateAngle(const float dt) {

    const float dFi = hRotate * dt * rotationSpeed;
    const float dPsi = vRotate * dt * rotationSpeed;

    fi += dFi;
    psi += dPsi;

    capAngles();

    changeMade = changeMade or dFi or dPsi;

}

void Camera::update(const float dt) {

    updateAngle(dt);
    calcTarget();
    updateCameraMatrix();

    if (changeMade) {
        apply();
        changeMade = false;
    }
}

void Camera::capAngles() {

    if (fi < 0) {
        fi += 360.f;
    }
    fi = std::fmod(fi, 360.f);

    psi = std::max(psi, -85.f);
    psi = std::min(psi, 85.f);
}

void Camera::calcTarget() {
    target = glm::vec3{ std::cos(fi), std::sin(psi), std::sin(fi) };
}

void Camera::onMouseMove(const MouseData & md) {

    if (not md.mbPressed()) {
        return;
    }

    const float rad_x = md.dx / 180.f * M_PI;
    const float rad_y = md.dy / 180.f * M_PI;

    const float dFi = rad_x * dragSpeed;
    const float dPsi = -1.f * rad_y * dragSpeed;

    fi += dFi;
    psi += dPsi;

    capAngles();

    changeMade = dFi or dPsi;

}

void Camera::setPosition(glm::vec3 pos) {
    eye = pos;
    calcTarget();
    updateCameraMatrix();
    apply();
}

glm::mat4 Camera::view() const {
    return camera;
}

glm::mat4 Camera::projection() const {
    return projectMat;
}

void Camera::notify(EventType eventType, void * object) {
    if (eventType == EventType::MouseMoved) {
        onMouseMove(((Mouse*)object)->data());
    }
}

glm::vec3 Camera::position() const {
    return eye;
}

static glm::vec2 adjustMovementSpeed(const glm::vec2 vector, const float max) {
    const float fx = (vector.y != 0) ? vector.x/2 : vector.x;
    const float fy = (vector.x != 0) ? vector.y/2 : vector.y;

    return { fx, fy };
}

glm::vec2 Camera::movementVector(float dt) const {
    const glm::vec2 adjusted = adjustMovementSpeed(movement, moveSpeed);
    const float sideways = adjusted.y * dt * moveSpeed;
    const float forward = adjusted.x * dt * moveSpeed;

    const float dx = std::cos(fi) * forward + std::cos(fi + M_PI_2) * sideways;
    const float dz = std::sin(fi) * forward + std::sin(fi + M_PI_2) * sideways;

    return { dx, dz };
}

void Camera::move(glm::vec3 delta) {
    eye += delta;
    apply();
}

