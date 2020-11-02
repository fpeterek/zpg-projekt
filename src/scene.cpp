//
// Created by fpeterek on 02.11.20.
//

#include "scene.hpp"


void Scene::update(const double dt) {
    camera.update(dt);
    for (Object & obj : objects) {
        obj.update(dt);
        obj.draw();
    }
}

void Scene::init() {
    ambientLight.apply();
    light.apply();
    camera.apply();
}

void Scene::addObject(const Object & object) {
    objects.emplace_back(object);
}

void Scene::addAll(const std::vector<Object> & vector) {
    for (const Object & obj : vector) {
        objects.emplace_back(obj);
    }
}

Scene::Scene(std::vector<Object> objects, AmbientLight ambientLight, PositionedLight posLight, Camera camera) :
    objects(std::move(objects)), ambientLight(std::move(ambientLight)),
    light(std::move(posLight)), camera(std::move(camera)) {
    init();
}

void Scene::Builder::reset() {
    objects = { };
    emplaceAmbientLight({ 0.1f, 0.1f, 0.1f });
    emplaceLight(glm::vec3 { 1.f }, glm::vec3 { 0.f } );
    emplaceCamera();
}

Scene::Builder & Scene::Builder::emplaceCamera() {
    camera = Camera();
    camera.addObserver(ShaderManager::constant());
    camera.addObserver(ShaderManager::lambert());
    camera.addObserver(ShaderManager::phong());
    camera.addObserver(ShaderManager::blinn());
    return *this;
}

Scene::Builder & Scene::Builder::addAll(const std::vector<Object> & vector) {
    for (const Object & obj : vector) {
        objects.emplace_back(obj);
    }
    return *this;
}

Scene::Builder & Scene::Builder::addObject(const Object & object) {
    objects.emplace_back(object);
    return *this;
}

Scene::Builder & Scene::Builder::emplaceLight(glm::vec3 color, glm::vec3 position) {
    light = PositionedLight { color, position };
    light.addObserver(ShaderManager::constant());
    light.addObserver(ShaderManager::lambert());
    light.addObserver(ShaderManager::phong());
    light.addObserver(ShaderManager::blinn());
    return *this;
}

Scene::Builder & Scene::Builder::emplaceAmbientLight(glm::vec3 color) {
    ambientLight = AmbientLight { color };
    ambientLight.addObserver(ShaderManager::constant());
    ambientLight.addObserver(ShaderManager::lambert());
    ambientLight.addObserver(ShaderManager::phong());
    ambientLight.addObserver(ShaderManager::blinn());
    return *this;
}

Scene * Scene::Builder::build() {
    Scene * scene = new Scene { std::move(objects), ambientLight, light, camera };
    reset();
    return scene;
}

Scene::Builder::Builder() {
    reset();
}
