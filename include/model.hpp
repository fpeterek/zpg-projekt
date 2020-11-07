//
// Created by fpeterek on 02.11.20.
//

#ifndef ZPG_PROJEKT_MODEL_HPP
#define ZPG_PROJEKT_MODEL_HPP

#include <vector>

#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include "enums.hpp"
#include "shader.hpp"

class Model {

    GLuint vbo = 0;
    GLuint vao = 0;

    const std::vector<float> points;

    void initVbo();
    void initVao();

public:

    Model(std::vector<float> points);
    void bindAndDraw(unsigned int id) const;


};

#endif //ZPG_PROJEKT_MODEL_HPP
