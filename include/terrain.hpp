//
// Created by fpeterek on 05.12.20.
//

#ifndef ZPG_PROJEKT_TERRAIN_HPP
#define ZPG_PROJEKT_TERRAIN_HPP

#include <vector>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "shader.hpp"

class Terrain {

public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal { 0, 1, 0 };
    };

private:
    const uint32_t width;
    const uint32_t length;
    GLuint vbo = 0;
    GLuint vao = 0;
    std::vector<Terrain::Vertex> vertices;

    void initVbo();
    void initVao();

    Shader & shader = ShaderManager::terrain();

    Terrain(std::vector<Terrain::Vertex> vertices, uint32_t width, uint32_t length);

public:

    Terrain(Terrain && terrain) noexcept;

    void draw();
    float yAt(float x, float z) const;
    glm::vec3 pointAt(float x, float z) const;

    static Terrain generate(uint32_t width, uint32_t length);
};


#endif //ZPG_PROJEKT_TERRAIN_HPP
