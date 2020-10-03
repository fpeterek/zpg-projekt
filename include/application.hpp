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

class Application {

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);
    // Camera matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);

    std::vector<Renderable> renderables;

    std::vector<Shader> shaders;
    std::optional<Shader> currentShader;

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
    void cursorPosCallback(GLFWwindow * window, double x, double y);
    void buttonCallback(GLFWwindow * window, int button, int action, int mode);

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
