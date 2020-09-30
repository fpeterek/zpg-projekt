//
// Created by fpeterek on 30.09.20.
//

#ifndef ZPG_PROJEKT_RENDERABLE_HPP
#define ZPG_PROJEKT_RENDERABLE_HPP

#include <vector>

#include <glm/mat4x4.hpp>
#include <GL/glew.h>

class Renderable {

    glm::mat4 transformation;
    GLuint vbo;
    GLuint vao;

    std::vector<float> points;

    void initVbo();
    void initVao();

public:

    explicit Renderable(std::vector<float> && points);

    void draw(GLuint modelId);

    void rotate(float degree, glm::vec3 axis);
    void translate(glm::vec3 delta);
    void scale(glm::vec3 scales);

};

#endif //ZPG_PROJEKT_RENDERABLE_HPP
