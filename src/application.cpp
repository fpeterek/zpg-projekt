//
// Created by Filip Peterek on 22/09/2020.
//

#include "application.hpp"

#include <algorithm>

Application Application::application;

void Application::errorCallback(int error, const char * description) {
    std::cerr << description << std::endl;
}

void Application::keyCallback(GLFWwindow * win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GL_TRUE);
    }
    std::cout << "key_callback [" << key << "," << scancode << "," << action << "," << mods << "]" << std::endl;
}

void Application::windowFocusCallback(GLFWwindow * win, int focused) {
    std::cout << "window_focus_callback" << std::endl;
}

void Application::windowIconifyCallback(GLFWwindow * win, int iconified) {
    std::cout << "window_iconify_callback" << std::endl;
}

void Application::windowSizeCallback(GLFWwindow * win, int width, int height) {
    std::cout << "resize " << width << ", " << height << std::endl;
    glViewport(0, 0, width, height);
}

void Application::cursorPosCallback(GLFWwindow * win, double x, double y) {
    std::cout << "cursor_pos_callback" << std::endl;
}

void Application::buttonCallback(GLFWwindow * win, int button, int action, int mode) {
    if (action == GLFW_PRESS) {
        std::cout << "button_callback [" << button << "," << action << "," << mode << "]"<< std::endl;
    }
}

void Application::initCallbacks() {

    auto error = [](int error, const char * description) {
        application.errorCallback(error, description);
    };
    glfwSetErrorCallback(error);

    auto key = [](GLFWwindow * win, int key, int scancode, int action, int mods) {
        application.keyCallback(win, key, scancode, action, mods);
    };
    glfwSetKeyCallback(window, key);

    auto focus = [](GLFWwindow * win, int focused) {
        application.windowFocusCallback(win, focused);
    };
    glfwSetWindowFocusCallback(window, focus);

    auto iconify = [](GLFWwindow * win, int iconified) {
        application.windowIconifyCallback(win, iconified);
    };
    glfwSetWindowIconifyCallback(window, iconify);

    auto size = [](GLFWwindow * win, int width, int length) {
        application.windowSizeCallback(win, width, length);
    };
    glfwSetWindowSizeCallback(window, size);

    auto cursor = [](GLFWwindow * win, double x, double y) {
        application.cursorPosCallback(win, x, y);
    };
    glfwSetCursorPosCallback(window, cursor);

    auto mouseButton = [](GLFWwindow * win, int button, int action, int mode) {
        application.buttonCallback(win, button, action, mode);
    };
    glfwSetMouseButtonCallback(window, mouseButton);

}


void Application::initGLFWContext() {
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

void Application::initWindow() {
    window = glfwCreateWindow(800, 600, "ZPG", nullptr, nullptr);
    if (not window) {
        glfwTerminate();
        throw std::runtime_error("Window could not be initialized");
    }
}

void Application::initGLEW() {
    glewExperimental = GL_TRUE;
    glewInit();
}

void Application::initShaders() {
    shaders.emplace_back("resources/shaders/shader.vert", "resources/shaders/shader.frag");
    currentShader = *shaders.begin();
}

void Application::printInfo() {

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Vendor " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "Using GLFW " << major << '.' << minor << '.' << revision << std::endl;

}

void Application::initGLFW() {

    if (not glfwInit()) {
        throw std::runtime_error("ERROR: could not start GLFW3");
    }

    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

}
void Application::initGL() {
    initGLFW();
    initWindow();
    initGLFWContext();
    initGLEW();
    initShaders();
    printInfo();
}

void Application::initViewport() {
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    bufferRatio = bufferWidth / (float)bufferHeight;
    glViewport(0, 0, bufferWidth, bufferHeight);
}

void Application::initApplication() {
    initCallbacks();
    initViewport();
}

void Application::run() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    loop();
}

void Application::update(const float dt) {

    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint modelMatrixID = currentShader->getUniformLocation("modelMatrix");
    currentShader->use();

    std::for_each(renderables.rbegin(), renderables.rend(), [modelMatrixID](Renderable & rend) {
        rend.draw(modelMatrixID);
    });

    // update other events like input handling
    glfwPollEvents();
    // put the stuff we’ve been drawing onto the display
    glfwSwapBuffers(window);

}

void Application::loop() {

    lastTime = std::chrono::high_resolution_clock::now();

    while (not glfwWindowShouldClose(window)) {

        const TimePoint now = std::chrono::high_resolution_clock::now();
        const float delta = std::chrono::duration_cast<Second>(now - lastTime).count();
        update(delta);
        lastTime = now;

    }

}

Application::~Application() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

Application::Application() {
    initGL();
    initApplication();
    renderables.emplace_back(std::vector<float>{0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f});
}
