//
// Created by fpeterek on 29.09.20.
//

#ifndef ZPG_PROJEKT_SHADER_HPP
#define ZPG_PROJEKT_SHADER_HPP

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "light.hpp"

class Shader : public Observer {

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
    void passUniformLocation(const std::string & var, int32_t value) const;
    void passUniformLocation(const char * var, int32_t value) const;

    void updateView(const glm::mat4 & view);
    void updateProjection(const glm::mat4 & projection);
    void updatePosition(const glm::vec3 & position);

    void colorChanged(glm::vec3 color, size_t lightIndex, gl::Light lightType);
    void positionChanged(glm::vec3 position, size_t lightIndex, gl::Light lightType);
    void typeChanged(gl::Light type, size_t lightIndex);

    void notify(EventType eventType, void * object) override;
};

class ShaderManager {

    Shader _constant;
    Shader _lambert;
    Shader _phong;
    Shader _blinn;
    Shader _skybox;

    static ShaderManager * sm;
    static ShaderManager & instance();

    ShaderManager();

public:
    static Shader & constant();
    static Shader & lambert();
    static Shader & phong();
    static Shader & blinn();
    static Shader & skybox();
};


#endif //ZPG_PROJEKT_SHADER_HPP
