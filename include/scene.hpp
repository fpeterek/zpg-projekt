//
// Created by fpeterek on 02.11.20.
//

#ifndef ZPG_PROJEKT_SCENE_HPP
#define ZPG_PROJEKT_SCENE_HPP

#include <vector>

#include "object.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "skybox.hpp"
#include "terrain.hpp"


class Scene {

    Scene(std::vector<Object> objects, AmbientLight ambientLight,
          std::vector<std::shared_ptr<ColoredLight>> light, glm::vec3 cameraPos,
          std::shared_ptr<Skybox> skybox, Terrain terrain);

    std::vector<std::shared_ptr<ColoredLight>> lights;
    std::shared_ptr<Skybox> skybox;
    Terrain terrain;

    void applyLights() const;
    void setShaderCount() const;

public:

    Scene() = delete;

    std::vector<Object> objects;

    const AmbientLight ambientLight;

    Camera camera;

    void init();

    void update(double timedelta);

    void addAll(const std::vector<Object> & objects);
    void addObject(const Object & object);

    size_t lightCount() const;
    const ColoredLight & light(size_t index) const;
    std::shared_ptr<ColoredLight> lightPtr(size_t index) const;
    void removeLight(std::size_t lightIndex);
    void emplaceLight(glm::vec3 color, glm::vec3 position, gl::Light type);
    void emplaceLight(glm::vec3 color, glm::vec3 pos, glm::vec3 dir, float cutoff);

    void removeObject(size_t id);

    size_t indexOf(unsigned int objectId);

    class Builder {

        std::vector<Object> objects;
        AmbientLight ambientLight;
        std::vector<std::shared_ptr<ColoredLight>> lights;
        glm::vec3 cameraPos { 0.f };
        std::optional<Terrain> terrain;

        void reset();

    public:

        Builder();

        Builder & emplaceLight(glm::vec3 color, glm::vec3 data, gl::Light type);
        Builder & emplaceLight(glm::vec3 color, glm::vec3 pos, glm::vec3 dir, float cutoff);
        Builder & emplaceAmbientLight(glm::vec3 color);
        Builder & setCameraPosition(glm::vec3 position);
        Builder & setCameraPosition(float x, float y, float z);

        Builder & addAll(const std::vector<Object> & objects);
        Builder & addObject(const Object & object);

        Builder & createTerrain(uint32_t width, uint32_t length);

        Scene * build();
    };

};

#endif //ZPG_PROJEKT_SCENE_HPP
