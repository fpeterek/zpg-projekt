//
// Created by fpeterek on 30.09.20.
//

#include <iostream>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderable.hpp"


void Renderable::rotate(float degree, const glm::vec3 axis) {
    transformation = glm::rotate(transformation, degree, axis);
}

void Renderable::translate(const glm::vec3 delta) {
    transformation = glm::translate(transformation, delta);
}

void Renderable::scale(const glm::vec3 scales) {
    transformation = glm::scale(transformation, scales);
}

void Renderable::initVbo() {

    glGenBuffers(1, &vbo); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

}

void Renderable::initVao() {

    glGenVertexArrays(1, &vao); //generate the VAO
    glBindVertexArray(vao); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

}

void Renderable::draw(const GLuint modelId) {
    glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(transformation));
    glBindVertexArray(vao);
    // draw triangles
    glDrawArrays(GL_TRIANGLES, 0, 3); //mode,first,count
}

Renderable::Renderable(std::vector<float> points) : points(std::move(points)) {
    initVbo();
    initVao();
}

