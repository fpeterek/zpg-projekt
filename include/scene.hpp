//
// Created by fpeterek on 02.11.20.
//

#ifndef ZPG_PROJEKT_SCENE_HPP
#define ZPG_PROJEKT_SCENE_HPP

#include <vector>

#include "object.hpp"
#include "light.hpp"
#include "camera.hpp"


class Scene {

    Scene(std::vector<Object> objects, AmbientLight ambientLight, PositionedLight light, Camera camera);

public:

    Scene() = delete;

    std::vector<Object> objects;

    const AmbientLight ambientLight;
    const PositionedLight light;

    Camera camera;

    void init();

    void update(double timedelta);

    void addAll(const std::vector<Object> & objects);
    void addObject(const Object & object);

    class Builder {

        std::vector<Object> objects;
        AmbientLight ambientLight;
        PositionedLight light;

        Camera camera;

        void reset();

    public:

        Builder();

        Builder & emplaceLight(glm::vec3 color, glm::vec3 position);
        Builder & emplaceAmbientLight(glm::vec3 color);
        Builder & emplaceCamera();

        Builder & addAll(const std::vector<Object> & objects);
        Builder & addObject(const Object & object);

        Scene * build();
    };

};

#endif //ZPG_PROJEKT_SCENE_HPP
