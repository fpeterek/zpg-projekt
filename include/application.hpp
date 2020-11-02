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
#include "object.hpp"
#include "camera.hpp"
#include "mouse.hpp"
#include "light.hpp"

class Application {

    std::vector<Object> renderables;
    Camera camera;

    Mouse & mouse = Mouse::instance();

    AmbientLight ambientLight { glm::vec3 { 0.1f } };
    PositionedLight light { glm::vec3 { 1.f }, glm::vec3 { 0.f } };

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

    void initViewport();

    void initGL();
    void initApplication();

    void update(float dt);
    void loop();

    Application();

    static Application * application;

public:

    static Application & instance();

    void run();

    ~Application();
};

#endif //ZPG_PROJEKT_APPLICATION_HPP
