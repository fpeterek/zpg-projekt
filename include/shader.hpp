//
// Created by fpeterek on 29.09.20.
//

#ifndef ZPG_PROJEKT_SHADER_HPP
#define ZPG_PROJEKT_SHADER_HPP

#include <iostream>
#include <string>

#include <GL/glew.h>

class Shader {

    GLuint vertexId;
    GLuint fragmentId;
    GLuint shaderId;

    typedef decltype(GL_FRAGMENT_SHADER) ShaderType;

    static std::string loadShader(const std::string & filename);
    static GLuint compileShader(const std::string & shader, ShaderType type);

    void compile(const std::string & vertex, const std::string & fragment);
    bool checkShader() const;

public:
    Shader(const std::string & vertex, const std::string & fragment);

    void use() const;

    GLint getUniformLocation(const std::string & var) const;
    GLint getUniformLocation(const char * var) const;

};


#endif //ZPG_PROJEKT_SHADER_HPP
