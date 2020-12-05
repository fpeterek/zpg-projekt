//
// Created by fpeterek on 05.12.20.
//

#ifndef ZPG_PROJEKT_SKYBOX_HPP
#define ZPG_PROJEKT_SKYBOX_HPP

#include <memory>

#include "model.hpp"
#include "shader.hpp"
#include "texture.hpp"


class Skybox {

    static const std::vector<float> points;

    std::shared_ptr<Texture> cubeMap;
    Shader & shader;

    GLuint vbo = 0;
    GLuint vao = 0;

    void initVbo();
    void initVao();

public:
    Skybox(std::shared_ptr<Texture> cubeMap);

    void draw() const;
};

#endif //ZPG_PROJEKT_SKYBOX_HPP
