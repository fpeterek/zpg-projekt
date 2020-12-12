//
// Created by fpeterek on 30.09.20.
//

#include <cmath>
#include <exception>

#include <glm/ext/matrix_transform.hpp>
#include <utility>

#include "object.hpp"

unsigned int Object::objectCount = 0;

const glm::vec3 Object::defaultColor { 0.92, 0.27, 0.19 };
const glm::vec3 Object::secondaryColor { 0.15, 0.89, 0.68 };

void Object::rotate(const glm::vec3 radians) {
    rotation = radians;
}

void Object::move(glm::vec3 delta) {
    translation += delta;
}

void Object::scale(const glm::vec3 newScales) {
    scales = newScales;
}

void Object::draw() const {
    shader.get().use();
    shader.get().passUniformLocation("objectColor", color);
    shader.get().passUniformLocation("modelMatrix", transformation());
    texture->bind(shader.get());
    model->draw(id, shader.get());
}

glm::mat4 Object::transformation() const {
    glm::mat4 t = glm::rotate(glm::mat4 { 1.f }, rotation.x, { 1.f, 0.f, 0.f });
    t = glm::rotate(t, rotation.y, { 0.f, 1.f, 0.f });
    t = glm::rotate(t, rotation.z, { 0.f, 0.f, 1.f });
    t = glm::translate(t, translation);
    return glm::scale(t, scales);
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
    const float delta = growthV * (int)growthDir * dt;
    scale({ scales.x + delta, scales.y + delta, scales.z + delta });
}

void Object::updateRotation(const float dt) {

    const float dec = rotationDec * dt;
    const float acc = rotationAcc * dt * (int)rotationDir;

    rotationV = capV(calcNewV(rotationV, acc, dec), rotationMax);

    rotation.y += rotationV * dt;
}

void Object::updateForces(const float dt) {

    const float dec = deceleration * dt;
    const float acc = acceleration * dt;

    const float fx = capV(calcNewV(forces.x, acc * movementDir.x, dec), maxV);
    const float fy = capV(calcNewV(forces.y, acc * movementDir.y, dec), maxV);

    forces = { fx, fy };

    move({ fx * dt, fy * dt, 0.f });
}

void Object::updateMovement(double dt) {

    if (not movementCalculator.has_value()) {
        return;
    }

    auto & mc = *(movementCalculator.value());
    mc.update(dt);
    translation = mc.currentPosition();
    rotation = mc.currentHeading();

}

void Object::update(const double dt) {
    updateGrowth(dt);
    updateRotation(dt);
    updateForces(dt);
    updateMovement(dt);
}

Object::Object(std::shared_ptr<Model> model, Shader & shader, std::shared_ptr<Texture> texture) :
    model(std::move(model)), shader(shader), texture(std::move(texture)), id(getNextId()) { }

Object::Object(std::shared_ptr<Model> model, Shader & shader, std::shared_ptr<Texture> texture,
    std::shared_ptr<MovementCalculator> movementCalculator) :
        model(std::move(model)), shader(shader), texture(std::move(texture)), id(getNextId()),
        movementCalculator(movementCalculator) { }

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

Object::Builder & Object::Builder::setModel(std::shared_ptr<Model> newModel) {
    model = std::move(newModel);
    return *this;
}

Object::Builder & Object::Builder::setShader(Shader & newShader) {
    shader = &newShader;
    return *this;
}

Object::Builder & Object::Builder::emplaceObject(std::shared_ptr<Model> model, Shader & shader, std::shared_ptr<Texture> texture) {
    return setModel(std::move(model)).setTexture(std::move(texture)).setShader(shader);
}

Object::Builder & Object::Builder::setRotation(glm::vec3 radians) {
    rotationRadians = radians;
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
    texture = nullptr;
    movementCalculator->reset();

    rotationRadians = glm::vec3 { 0.f };
    position = glm::vec3 { 0.f };
    scales = glm::vec3 { 1.f };
    color = defaultColor;
}

Object Object::Builder::createObject() {
    if (movementCalculator.has_value()) {
        return Object { std::move(model), *shader, std::move(texture), movementCalculator.value() };
    }
    return Object { std::move(model), *shader, std::move(texture) };
}

Object Object::Builder::build() {
    if (not (model and shader)) {
        throw std::runtime_error("ObjectBuilder error: Missing value '" + std::string(model ? "shader" : "model") + "'");
    }

    Object obj = createObject();
    obj.move(position);
    obj.rotate(rotationRadians);
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

Object::Builder & Object::Builder::setTexture(std::shared_ptr<Texture> tex) {
    texture = std::move(tex);
    return *this;
}

Object::Builder & Object::Builder::setMovement(std::shared_ptr<MovementCalculator> mc) {
    movementCalculator.emplace(std::move(mc));
    return *this;
}










