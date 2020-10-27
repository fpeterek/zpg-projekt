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

    for (auto ref : observers) {
        ref.get().updateView(camera);
        ref.get().updateProjection(projection);
        ref.get().updatePosition(eye);
    }
}

void Camera::moveSideways(Direction dir) {
    sidewaysMovement = (int)dir;
}

void Camera::moveForward(Direction dir) {
    forwardMovement = (int)dir;
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

void Camera::updateForwardMovement(const float dt) {

    const float sideways = sidewaysMovement * dt * moveSpeed;
    const float forward = forwardMovement * dt * moveSpeed;

    const float dx = std::cos(fi) * forward + std::cos(fi + M_PI_2) * sideways;
    const float dy = std::sin(psi) * forward;
    const float dz = std::sin(fi) * forward + std::sin(fi + M_PI_2) * sideways;

    eye.x += dx;
    eye.y += dy;
    eye.z += dz;

    changeMade = changeMade or dx or dy or dz;

}

void Camera::update(const float dt) {

    updateAngle(dt);
    updateForwardMovement(dt);
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

    if (not md.lbPressed()) {
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

void Camera::addObserver(CameraObserver & obs) {
    observers.emplace_back(obs);
}

