//
// Created by fpeterek on 05.12.20.
//

#include "skybox.hpp"

void Skybox::initVbo() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    constexpr int typeSize = sizeof(decltype(points)::value_type);
    glBufferData(GL_ARRAY_BUFFER, points.size() * typeSize, points.data(), GL_STATIC_DRAW);
}

void Skybox::initVao() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    using type = decltype(points)::value_type;
    constexpr int typeSize = sizeof(type);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*typeSize, nullptr);
}

Skybox::Skybox(std::shared_ptr<Texture> cubeMap) : cubeMap(std::move(cubeMap)), shader(ShaderManager::skybox()) {
    initVbo();
    initVao();
}

void Skybox::draw() const {
    glDepthMask(GL_FALSE);
    shader.use();
    cubeMap->bind(shader);
    glStencilFunc(GL_ALWAYS, -1, 0xFF);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, points.size());
    glDepthMask(GL_TRUE);
}

const std::vector<float> Skybox::points = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};


