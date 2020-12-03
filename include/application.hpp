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
#include "scene.hpp"

class Application : public Observer {

    Mouse & mouse = Mouse::instance();

    Scene * scenePtr = nullptr;
    Scene & scene();

    size_t selected = noneSelected;

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

    void deselect();
    void selectObject(int mouseX, int mouseY);

    void emplaceObject(int mouseX, int mouseY);

    bool hasSelected();
    Object & getSelected();

    void onButtonPress(const MouseData & mouseData);
    void notify(EventType eventType, void * object) override;

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

    static constexpr size_t noneSelected = std::numeric_limits<std::size_t>::max();

public:

    static Application & instance();

    void run();

    ~Application();
};

#endif //ZPG_PROJEKT_APPLICATION_HPP
