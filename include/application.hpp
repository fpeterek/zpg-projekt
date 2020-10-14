//
// Created by Filip Peterek on 22/09/2020.
//

#ifndef ZPG_PROJEKT_APPLICATION_HPP
#define ZPG_PROJEKT_APPLICATION_HPP

#include <iostream>
#include <chrono>
#include <vector>
#include <functional>
#include <optional>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "renderable.hpp"
#include "camera.hpp"
#include "mouse.hpp"

class Application {

    std::vector<Renderable> renderables;
    std::vector<Shader> shaders;
    std::optional<Camera> camera;

    Mouse & mouse = Mouse::instance();

    glm::vec3 lightPos { 0.0, 1.0, 0.0 };

    int bufferWidth = 0;
    int bufferHeight = 0;
    float bufferRatio = 0.f;

    GLFWwindow * window = nullptr;

    typedef decltype(std::chrono::high_resolution_clock::now()) TimePoint;
    typedef std::chrono::duration<double, std::ratio<1>> Second;

    TimePoint lastTime = std::chrono::high_resolution_clock::now();

    void errorCallback(int error, const char * description);
    void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
    void windowFocusCallback(GLFWwindow * window, int focused);
    void windowIconifyCallback(GLFWwindow * window, int iconified);
    void windowSizeCallback(GLFWwindow * window, int width, int height);

    void initCallbacks();

    void printInfo();

    void initGLFW();
    void initWindow();
    void initGLFWContext();
    void initGLEW();
    void initShaders();

    void initViewport();

    void initGL();
    void initApplication();

    void update(float dt);
    void loop();

    Application();

public:

    static Application application;

    void run();

    ~Application();
};

#endif //ZPG_PROJEKT_APPLICATION_HPP
