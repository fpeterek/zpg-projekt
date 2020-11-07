//
// Created by fpeterek on 30.09.20.
//

#include <cmath>
#include <exception>

#include <glm/ext/matrix_transform.hpp>

#include "object.hpp"

unsigned int Object::objectCount = 0;

const glm::vec3 Object::defaultColor { 0.92, 0.27, 0.19 };
const glm::vec3 Object::secondaryColor { 0.15, 0.89, 0.68 };

void Object::rotate(float degree, const glm::vec3 axis) {
    transform = glm::rotate(transform, degree, axis);
}

void Object::translate(const glm::vec3 delta) {
    transform = glm::translate(transform, delta);
}

void Object::scale(const glm::vec3 scales) {
    transform = glm::scale(transform, scales);
}

void Object::draw() const {
    shader.get().use();
    shader.get().passUniformLocation("objectColor", color);
    shader.get().passUniformLocation("modelMatrix", transform);
    model.get().bindAndDraw(id);
}

const glm::mat4 & Object::transformation() const {
    return transform;
}

float Object::getAcc(Direction dir) {
    return acceleration * (int)dir;
}

void Object::applyFy(Direction dir) {
    movementDir.y = getAcc(dir);
}

void Object::applyFx(Direction dir) {
    movementDir.x = getAcc(dir);
}

void Object::applyForce(Direction dx, Direction dy) {
    applyFx(dx);
    applyFy(dy);
}

void Object::enableGrowth(Growth direction) {
    growthDir = direction;
}

void Object::disableGrowth() {
    growthDir = Growth::none;
}

void Object::enableRotation(Rotation direction) {
    rotationDir = direction;
}

void Object::disableRotation() {
    rotationDir = Rotation::none;
}

float Object::calcNewV(float current, const float acc, const float dec) {

    if (std::abs(current) <= dec) {
        current = 0;
    } else {
        current -= (-1 + (2 * (current > 0))) * dec;
    }

    return current + acc;
}

float Object::capV(float current, float max) {
    return std::min(std::max(current, -max), max);
}

void Object::updateGrowth(const float dt) {
    const auto g = 1 + growthV * (int)growthDir * dt;
    scale(glm::vec3(g));
}

void Object::updateRotation(const float dt) {

    const float dec = rotationDec * dt;
    const float acc = rotationAcc * dt * (int)rotationDir;

    rotationV = capV(calcNewV(rotationV, acc, dec), rotationMax);

    rotate(rotationV * dt, glm::vec3(0.f, 0.f, 1.f));
}

void Object::updateForces(const float dt) {

    const float dec = deceleration * dt;
    const float acc = acceleration * dt;

    const float fx = capV(calcNewV(forces.x, acc * movementDir.x, dec), maxV);
    const float fy = capV(calcNewV(forces.y, acc * movementDir.y, dec), maxV);

    forces = { fx, fy };

    translate({fx * dt, fy * dt, 0.f});
}

void Object::update(const double dt) {
    updateGrowth(dt);
    updateRotation(dt);
    updateForces(dt);
}

Object::Object(Model & model, Shader & shader) : model(model), shader(shader), id(getNextId()) { }

unsigned int Object::getNextId() {
    return ++objectCount;
}

unsigned int Object::objectId() const {
    return id;
}

void Object::setColor(glm::vec3 newColor) {
    color = newColor;
}

void Object::setColor(float r, float g, float b) {
    setColor({r, g, b});
}

Object::Builder & Object::Builder::setModel(Model & newModel) {
    model = &newModel;
    return *this;
}

Object::Builder & Object::Builder::setShader(Shader & newShader) {
    shader = &newShader;
    return *this;
}

Object::Builder & Object::Builder::emplaceObject(Model & model, Shader & shader) {
    setModel(model);
    return setShader(shader);
}

Object::Builder & Object::Builder::setRotation(float newDegree, glm::vec3 axis) {
    degree = newDegree;
    rotationAxis = axis;
    return *this;
}

Object::Builder & Object::Builder::setPosition(glm::vec3 pos) {
    position = pos;
    return *this;
}

Object::Builder & Object::Builder::setPosition(float x, float y, float z) {
    return setPosition({x, y, z});
}

Object::Builder & Object::Builder::setScale(glm::vec3 s) {
    scales = s;
    return *this;
}

Object::Builder & Object::Builder::setScale(float x, float y, float z) {
    return setScale({x, y, z});
}

void Object::Builder::reset() {

    model = nullptr;
    shader = nullptr;
    degree = 0.f;

    rotationAxis = glm::vec3 { 0.f };
    position = glm::vec3 { 0.f };
    scales = glm::vec3 { 1.f };
    color = defaultColor;
}

Object Object::Builder::build() {
    if (not (model and shader)) {
        throw std::runtime_error("ObjectBuilder error: Missing value '" + std::string(model ? "shader" : "model") + "'");
    }

    Object obj { *model, *shader };
    obj.translate(position);
    if (rotationAxis.x or rotationAxis.y or rotationAxis.z) {
        obj.rotate(degree, rotationAxis);
    }
    obj.scale(scales);
    obj.color = color;

    reset();

    return obj;
}

Object::Builder & Object::Builder::setColor(glm::vec3 newColor) {
    color = newColor;
    return *this;
}

Object::Builder & Object::Builder::setColor(float r, float g, float b) {
    return setColor({r, g, b});
}










