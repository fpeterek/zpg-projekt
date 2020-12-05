//
// Created by fpeterek on 29.09.20.
//

#include "shader.hpp"

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"

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
    auto location = glGetUniformLocation(shaderId, var);
    if (location < 0) {
        // std::cout << "Uniform variable '" << var << "' not found." << std::endl;
    }
    return location;
}

void Shader::passUniformLocation(const std::string & var, const glm::mat4 & matrix) const {
    passUniformLocation(var.c_str(), matrix);
}

void Shader::passUniformLocation(const char * var, const glm::mat4 & matrix) const {
    const auto model = getUniformLocation(var);
    if (model >= 0) {
        glProgramUniformMatrix4fv(shaderId, model, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::passUniformLocation(const std::string & var, const glm::vec3 & vector) const {
    passUniformLocation(var.c_str(), vector);
}

void Shader::passUniformLocation(const char * var, const glm::vec3 & vector) const {
    const auto location = getUniformLocation(var);
    if (location >= 0) {
        glProgramUniform3f(shaderId, location, vector.x, vector.y, vector.z);
    }
}

void Shader::passUniformLocation(const std::string & var, const glm::mat3 & matrix) const {
    passUniformLocation(var.c_str(), matrix);
}

void Shader::passUniformLocation(const char * var, const glm::mat3 & matrix) const {
    const auto location = getUniformLocation(var);
    if (location >= 0) {
        glProgramUniformMatrix3fv(shaderId, location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::updateView(const glm::mat4 & view) {
    passUniformLocation("viewMatrix", view);
}

void Shader::updateProjection(const glm::mat4 & projection) {
    passUniformLocation("projectionMatrix", projection);
}

void Shader::updatePosition(const glm::vec3 & position) {
    passUniformLocation("cameraPosition", position);
}

void Shader::colorChanged(glm::vec3 color, size_t lightIndex, gl::Light lightType) {
    if (lightType == gl::Light::Ambient) {
        passUniformLocation("ambientColor", color);
    } else if (lightType == gl::Light::Point or lightType == gl::Light::Directional) {
        passUniformLocation("lights[" + std::to_string(lightIndex) + "].lightColor", color);
    }
}

void Shader::positionChanged(glm::vec3 position, size_t lightIndex, gl::Light lightType) {
    if (lightType == gl::Light::Ambient) {
        /* noop -> ambient light has no coordinates */
    } else if (lightType == gl::Light::Point or lightType == gl::Light::Directional) {
        passUniformLocation("lights[" + std::to_string(lightIndex) + "].position", position);
    }
}

void Shader::passUniformLocation(const std::string & var, const int32_t value) const {
    passUniformLocation(var.c_str(), value);
}

void Shader::passUniformLocation(const char * var, const int32_t value) const {
    const auto location = getUniformLocation(var);
    if (location >= 0) {
        glProgramUniform1i(shaderId, location, value);
    }
}

void Shader::typeChanged(gl::Light type, size_t lightIndex) {
    passUniformLocation("lights[" + std::to_string(lightIndex) + "].lightType", int32_t(type));
}

void Shader::notify(EventType eventType, void * object) {
    if (eventType == EventType::LightChanged) {

        ColoredLight & light = *(ColoredLight*)object;

        colorChanged(light.getColor(), light.lightIndex, light.type());
        typeChanged(light.type(), light.lightIndex);

        if (light.type() == gl::Light::Point) {
            PositionedLight & l = *(PositionedLight*)object;
            positionChanged(l.getPosition(), l.lightIndex, l.type());
        } else if (light.type() == gl::Light::Directional) {
            DirectionalLight & l = *(DirectionalLight*)object;
            positionChanged(l.getDirection(), l.lightIndex, l.type());
        }
    } else if (eventType == EventType::CameraMoved) {
        Camera & camera = *((Camera*)object);
        updateView(camera.view());
        updatePosition(camera.position());
        updateProjection(camera.projection());
    }
}

ShaderManager & ShaderManager::instance() {
    if (not sm) {
        sm = new ShaderManager();
    }
    return *sm;
}

ShaderManager * ShaderManager::sm = nullptr;

ShaderManager::ShaderManager() :
    _constant("resources/shaders/shader.vert", "resources/shaders/constant.frag"),
    _lambert("resources/shaders/shader.vert", "resources/shaders/lambert.frag"),
    _phong("resources/shaders/shader.vert", "resources/shaders/phong.frag"),
    _blinn("resources/shaders/shader.vert", "resources/shaders/blinn.frag") { }

Shader & ShaderManager::constant() {
    return instance()._constant;
}

Shader & ShaderManager::lambert() {
    return instance()._lambert;
}

Shader & ShaderManager::phong() {
    return instance()._phong;
}

Shader & ShaderManager::blinn() {
    return instance()._blinn;
}
