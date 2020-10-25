//
// Created by fpeterek on 29.09.20.
//

#ifndef ZPG_PROJEKT_SHADER_HPP
#define ZPG_PROJEKT_SHADER_HPP

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "camera_observer.hpp"
#include "light.hpp"

class Shader : public CameraObserver, public LightObserver {

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

    void updateView(const glm::mat4 & view) override;
    void updateProjection(const glm::mat4 & projection) override;

    void colorChanged(glm::vec3 color, LightType lightType) override;
    void positionChanged(glm::vec3 position, LightType lightType) override;

};

class ShaderManager {

    Shader _constant;
    Shader _lambert;
    Shader _phong;
    Shader _blinn;

    static ShaderManager * sm;
    static ShaderManager & instance();

    ShaderManager();

public:
    static Shader & constant();
    static Shader & lambert();
    static Shader & phong();
    static Shader & blinn();
};


#endif //ZPG_PROJEKT_SHADER_HPP
