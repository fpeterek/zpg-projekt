//
// Created by fpeterek on 02.11.20.
//

#include "model.hpp"

#include <cmath>


void Model::initVbo() {
    glGenBuffers(1, &vbo); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    constexpr int typeSize = sizeof(decltype(points)::value_type);
    glBufferData(GL_ARRAY_BUFFER, points.size() * typeSize, points.data(), GL_STATIC_DRAW);
}

void Model::initVao() {
    glGenVertexArrays(1, &vao); //generate the VAO
    glBindVertexArray(vao); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    using type = decltype(points)::value_type;
    constexpr int typeSize = sizeof(type);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * typeSize, nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * typeSize, (GLvoid*)(3*typeSize));
}

void Model::bindAndDraw(const unsigned int id) const {
    glStencilFunc(GL_ALWAYS, id, 0xFF);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, points.size());
}

Model::Model(std::vector<float> points) : points(std::move(points)) {
    initVbo();
    initVao();
}

