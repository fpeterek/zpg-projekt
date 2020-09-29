//
// Created by fpeterek on 29.09.20.
//

#include "shader.hpp"

#include <fstream>
#include <sstream>

std::string Shader::loadShader(const std::string & filename) {

    std::ifstream in(filename);
    std::stringstream ss;

    while (in) {
        std::string line;
        std::getline(in, line);
        ss << line << "\n";
    }

    return ss.str();
}

GLuint Shader::compileShader(const std::string & shader, const ShaderType type) {

    GLuint id = glCreateShader(type);
    const char * shaderPtr = shader.c_str();
    glShaderSource(id, 1, &shaderPtr, nullptr);
    glCompileShader(id);

    return id;
}

void Shader::compile(const std::string & vertex, const std::string & fragment) {

    shaderId = glCreateProgram();

    vertexId = compileShader(vertex, GL_VERTEX_SHADER);
    fragmentId = compileShader(fragment, GL_FRAGMENT_SHADER);

    glAttachShader(shaderId, vertexId);
    glAttachShader(shaderId, fragmentId);
    glLinkProgram(shaderId);

}

Shader::Shader(const std::string & vertex, const std::string & fragment): shaderId(-1), vertexId(-1), fragmentId(-1) {

    compile(loadShader(vertex), loadShader(fragment));

    if (not checkShader()) {
        throw std::runtime_error("Shader could not be compiled or linked");
    }

}

void Shader::use() const {
    glUseProgram(shaderId);
}

bool Shader::checkShader() const {

    GLint status;
    glGetProgramiv(shaderId, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto * strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(shaderId, infoLogLength, nullptr, strInfoLog);
        std::cerr << "Linker failure: " << strInfoLog << std::endl;
        delete[] strInfoLog;
    }

    return status != GL_FALSE;
}

GLint Shader::getUniformLocation(const std::string & var) const {
    return getUniformLocation(var.c_str());
}

GLint Shader::getUniformLocation(const char * var) const {
    return glGetUniformLocation(shaderId, var);
}
