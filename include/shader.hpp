//
// Created by fpeterek on 29.09.20.
//

#ifndef ZPG_PROJEKT_SHADER_HPP
#define ZPG_PROJEKT_SHADER_HPP

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>


class Shader {

    GLuint vertexId;
    GLuint fragmentId;
    GLuint shaderId;

    typedef decltype(GL_FRAGMENT_SHADER) ShaderType;

    static std::string loadShader(const std::string & filename);
    static GLuint compileShader(const std::string & shader, ShaderType type);

    void compile(const std::string & vertex, const std::string & fragment);
    bool checkShader() const;

    GLint getUniformLocation(const std::string & var) const;
    GLint getUniformLocation(const char * var) const;

public:
    Shader(const std::string & vertex, const std::string & fragment);

    void use() const;

    void passUniformLocation(const std::string & var, const glm::mat4 & matrix) const;
    void passUniformLocation(const char * var, const glm::mat4 & matrix) const;
    void passUniformLocation(const std::string & var, const glm::mat3 & matrix) const;
    void passUniformLocation(const char * var, const glm::mat3 & matrix) const;
    void passUniformLocation(const std::string & var, const glm::vec3 & vector) const;
    void passUniformLocation(const char * var, const glm::vec3 & vector) const;

};


#endif //ZPG_PROJEKT_SHADER_HPP
