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
    GLuint vbo = 0;
    GLuint vao = 0;
    std::vector<Terrain::Vertex> vertices;

    void initVbo();
    void initVao();

    Shader & shader = ShaderManager::terrain();

    explicit Terrain(std::vector<Terrain::Vertex> vertices);

public:

    Terrain(Terrain && terrain) noexcept;

    void draw();

    static Terrain generate(uint32_t width, uint32_t length);
};


#endif //ZPG_PROJEKT_TERRAIN_HPP
