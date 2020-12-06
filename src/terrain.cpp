//
// Created by fpeterek on 05.12.20.
//

#include "terrain.hpp"

#include <glm/gtc/noise.hpp>

constexpr float distance = 5;

static std::vector<Terrain::Vertex> triangulateAndFlatten(const std::vector<std::vector<Terrain::Vertex>> & vertices) {

    std::vector<Terrain::Vertex> result;
    result.reserve(vertices.size() * vertices.front().size() * 6);

    const auto width = vertices.size();
    const auto length = vertices.front().size();

    for (int x = 0; x < width-1; ++x) {
        for (int z = 0; z < length-1; ++z) {
            result.emplace_back(vertices[x][z]);
            result.emplace_back(vertices[x+1][z]);
            result.emplace_back(vertices[x+1][z+1]);

            result.emplace_back(vertices[x][z]);
            result.emplace_back(vertices[x][z+1]);
            result.emplace_back(vertices[x+1][z+1]);
        }
    }

    return result;

}

static std::vector<std::vector<Terrain::Vertex>> generateVertices(uint32_t width, uint32_t length) {

    std::vector<std::vector<Terrain::Vertex>> vertices;
    vertices.reserve(width);

    const float w_offset = (width*0.5f) * distance;
    const float l_offset = (length*0.5f) * distance;

    for (uint32_t v = 0; v < width; ++v) {
        vertices.emplace_back();
        auto & vec = vertices.back();
        vec.reserve(length);

        const float x = v*distance - w_offset;

        for (uint32_t i = 0; i < length; ++i) {
            vec.emplace_back();
            auto & vert = vec.back();


            const float z = i*distance - l_offset;
            const float y = 10 * glm::perlin(glm::vec2 {v/(float)width, i/(float)length});

            vert.position = { x, y, z };
        }
    }

    for (int x = 1; x < width - 1; ++x) {
        for (int z = 1; z < length - 1; ++z) {

            glm::vec3 mid = vertices[x][z].position;

            glm::vec3 v1 = glm::normalize(vertices[x-1][z].position - mid); // left
            glm::vec3 v2 = glm::normalize(vertices[x][z+1].position - mid); // down
            glm::vec3 v3 = glm::normalize(vertices[x+1][z].position - mid); // right
            glm::vec3 v4 = glm::normalize(vertices[x][z-1].position - mid); // up

            glm::vec3 v12 = glm::normalize(cross(v1, v2));
            glm::vec3 v23 = glm::normalize(cross(v2, v3));
            glm::vec3 v34 = glm::normalize(cross(v3, v4));
            glm::vec3 v41 = glm::normalize(cross(v4, v1));

            vertices[x][z].normal = normalize(v12 + v23 + v34 + v41);
        }
    }

    return vertices;
}

Terrain Terrain::generate(uint32_t width, uint32_t length) {

    auto matrix = generateVertices(width, length);
    auto flat = triangulateAndFlatten(matrix);

    return Terrain(std::move(flat));
}

Terrain::Terrain(std::vector<Terrain::Vertex> vertices) : vertices(std::move(vertices)) {
    initVbo();
    initVao();
}

void Terrain::initVbo() {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    constexpr int typeSize = sizeof(decltype(vertices)::value_type);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * typeSize, vertices.data(), GL_STATIC_DRAW);
}

void Terrain::initVao() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    using type = decltype(vertices)::value_type;
    constexpr int typeSize = sizeof(type);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, typeSize, nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, typeSize, (GLvoid*)(sizeof(type::position)));
}

void Terrain::draw() {
    glDepthMask(GL_FALSE);
    shader.use();
    glStencilFunc(GL_ALWAYS, -1, 0xFF);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glDepthMask(GL_TRUE);
}

Terrain::Terrain(Terrain && terrain) noexcept :
    vbo(terrain.vbo), vao(terrain.vao), vertices(std::move(terrain.vertices)) { }
