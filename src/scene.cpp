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
    applyLights();
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

Scene::Scene(std::vector<Object> objects, AmbientLight ambientLight,
             std::vector<std::shared_ptr<ColoredLight>> lights, glm::vec3 cameraPos) :
    objects(std::move(objects)), ambientLight(std::move(ambientLight)), lights(std::move(lights)) {

    init();
    camera.setPosition(cameraPos);

}

size_t Scene::indexOf(unsigned int objectId) {
    for (size_t i = 0; i < objects.size(); ++i) {
        if (objects[i].objectId() == objectId) {
            return i;
        }
    }
    return std::numeric_limits<size_t>::max();
}

size_t Scene::lightCount() const {
    return lights.size();
}

std::shared_ptr<ColoredLight> Scene::lightPtr(const size_t index) const {
    return lights[index];
}

const ColoredLight & Scene::light(const size_t index) const {
    return *lights[index];
}

void Scene::applyLights() const {
    setShaderCount();
    for (const auto & light : lights) {
        light->apply();
    }
}

void Scene::removeLight(const std::size_t lightIndex) {
    lights.erase(lights.begin() + lightIndex);
    for (std::size_t i = lightIndex; i < lights.size(); ++i) {
        lights[i]->lightIndex = i;
    }
    applyLights();
}

static std::shared_ptr<ColoredLight> createLight(glm::vec3 color, glm::vec3 data, gl::Light type) {
    if (type == gl::Light::Point) {
        return std::make_shared<PositionedLight>(color, data);
    } else if (type == gl::Light::Directional) {
        return std::make_shared<DirectionalLight>(color, data);
    }
    throw std::runtime_error("Unsupported light type");
}

static void emplaceLight(const glm::vec3 & color, const glm::vec3 & pos, const gl::Light type,
                         std::vector<std::shared_ptr<ColoredLight>> & vec) {

    auto light = createLight(color, pos, type);
    light->registerObserver(ShaderManager::constant());
    light->registerObserver(ShaderManager::lambert());
    light->registerObserver(ShaderManager::phong());
    light->registerObserver(ShaderManager::blinn());
    light->lightIndex = vec.size();
    vec.emplace_back(light);

}

void Scene::emplaceLight(glm::vec3 color, glm::vec3 position, gl::Light type) {
    ::emplaceLight(color, position, type, lights);
    applyLights();
}

void Scene::setShaderCount() const {
    ShaderManager::lambert().passUniformLocation("lightCount", lights.size());
    ShaderManager::blinn().passUniformLocation("lightCount", lights.size());
    ShaderManager::phong().passUniformLocation("lightCount", lights.size());
}

void Scene::Builder::reset() {
    objects = { };
    emplaceAmbientLight({ 0.1f, 0.1f, 0.1f });
    lights = { };
    // emplaceLight(glm::vec3 { 1.f }, glm::vec3 { 0.f }, gl::Light::Point);
    cameraPos = { 0.f, 0.f, 0.f };
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

Scene::Builder & Scene::Builder::emplaceLight(glm::vec3 color, glm::vec3 position, gl::Light type) {
    ::emplaceLight(color, position, type, lights);
    return *this;
}

Scene::Builder & Scene::Builder::emplaceAmbientLight(glm::vec3 color) {
    ambientLight = AmbientLight { color };
    ambientLight.registerObserver(ShaderManager::constant());
    ambientLight.registerObserver(ShaderManager::lambert());
    ambientLight.registerObserver(ShaderManager::phong());
    ambientLight.registerObserver(ShaderManager::blinn());
    return *this;
}

Scene * Scene::Builder::build() {
    auto * scene = new Scene { std::move(objects), ambientLight, lights, cameraPos };

    scene->camera.registerObserver(ShaderManager::constant());
    scene->camera.registerObserver(ShaderManager::lambert());
    scene->camera.registerObserver(ShaderManager::phong());
    scene->camera.registerObserver(ShaderManager::blinn());
    Mouse::instance().registerObserver(scene->camera);

    reset();
    return scene;
}

Scene::Builder::Builder() {
    reset();
}

Scene::Builder & Scene::Builder::setCameraPosition(glm::vec3 position) {
    cameraPos = position;
    return *this;
}

Scene::Builder & Scene::Builder::setCameraPosition(float x, float y, float z) {
    cameraPos = { x, y ,z };
    return *this;
}
