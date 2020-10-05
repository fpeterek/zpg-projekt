//
// Created by fpeterek on 30.09.20.
//

#include <cmath>

#include <glm/ext/matrix_transform.hpp>

#include "renderable.hpp"

#define growth
#define rotation
#define velocity

void Renderable::rotate(float degree, const glm::vec3 axis) {
    transform = glm::rotate(transform, degree, axis);
}

void Renderable::translate(const glm::vec3 delta) {
    transform = glm::translate(transform, delta);
}

void Renderable::scale(const glm::vec3 scales) {
    transform = glm::scale(transform, scales);
}

void Renderable::initVbo() {

    glGenBuffers(1, &vbo); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(decltype(points)::value_type), points.data(), GL_STATIC_DRAW);

}

void Renderable::initVao() {

    glGenVertexArrays(1, &vao); //generate the VAO
    glBindVertexArray(vao); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

}

void Renderable::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

const glm::mat4 & Renderable::transformation() const {
    return transform;
}

Renderable::Renderable(std::vector<float> points) : points(std::move(points)) {
    initVbo();
    initVao();
}

float Renderable::getAcc(Direction dir) {
    return acceleration * (int)dir;
}

void Renderable::applyFy(Direction dir) {
    movementDir.y = getAcc(dir);
}

void Renderable::applyFx(Direction dir) {
    movementDir.x = getAcc(dir);
}

void Renderable::applyForce(Direction dx, Direction dy) {
    applyFx(dx);
    applyFy(dy);
}

void Renderable::enableGrowth(Growth direction) {
    growthDir = direction;
}

void Renderable::disableGrowth() {
    growthDir = Growth::none;
}

void Renderable::enableRotation(Rotation direction) {
    rotationDir = direction;
}

void Renderable::disableRotation() {
    rotationDir = Rotation::none;
}

float Renderable::calcNewV(float current, const float acc, const float dec) {

    if (std::abs(current) <= dec) {
        current = 0;
    } else {
        current -= (-1 + (2 * (current > 0))) * dec;
    }

    return current + acc;
}

float Renderable::capV(float current, float max) {
    return std::min(std::max(current, -max), max);
}

void Renderable::updateGrowth(const float dt) {
    const auto g = 1 + growthV * (int)growthDir * dt;
    scale(glm::vec3(g));
}

void Renderable::updateRotation(const float dt) {

    const float dec = rotationDec * dt;
    const float acc = rotationAcc * dt * (int)rotationDir;

    rotationV = capV(calcNewV(rotationV, acc, dec), rotationMax);

    rotate(rotationV * dt, glm::vec3(0.f, 0.f, 1.f));
}

void Renderable::updateForces(const float dt) {

    const float dec = deceleration * dt;
    const float acc = acceleration * dt;

    const float fx = capV(calcNewV(forces.x, acc * movementDir.x, dec), maxV);
    const float fy = capV(calcNewV(forces.y, acc * movementDir.y, dec), maxV);

    forces = { fx, fy };

    translate({fx * dt, fy * dt, 0.f});
}

void Renderable::update(const double dt) {
    updateGrowth(dt);
    updateRotation(dt);
    updateForces(dt);
}

