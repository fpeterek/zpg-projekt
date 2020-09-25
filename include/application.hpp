//
// Created by Filip Peterek on 22/09/2020.
//

#ifndef ZPG_PROJEKT_APPLICATION_HPP
#define ZPG_PROJEKT_APPLICATION_HPP

#include <iostream>
#include <chrono>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shaders.hpp"

class Application {

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    static glm::mat4 Projection;
    // Camera matrix
    static glm::mat4 View;
    // Model matrix : an identity matrix (model will be at the origin)
    static glm::mat4 Model;

    static float points[9];

    static GLuint shaderProgram;

    static GLuint VBO;
    static GLuint VAO;

    static int bufferWidth;
    static int bufferHeight;
    static float bufferRatio;

    static GLFWwindow * window;

    typedef decltype(std::chrono::high_resolution_clock::now()) TimePoint;
    typedef std::chrono::duration<double, std::ratio<1>> Second;

    static TimePoint lastTime;

    static void printInfo();

    static void initGLFW();
    static void initWindow();
    static void initGLFWContext();
    static void initGLEW();
    static void initShaders();

    static void compileShaders();
    static bool checkShaders();

    static void initVBO();
    static void initVAO();
    static void initViewport();

    static void initGL();
    static void initApplication();
    static void init();

    static void update(float dt);
    static void loop();

    static void cleanup();

public:

    static void run();

};

#endif //ZPG_PROJEKT_APPLICATION_HPP
