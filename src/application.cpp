//
// Created by Filip Peterek on 22/09/2020.
//

#include "application.hpp"

#include <algorithm>

#include "models.hpp"

Application * Application::application = nullptr;

void Application::errorCallback(int error, const char * description) {
    std::cerr << description << std::endl;
}

void Application::keyCallback(GLFWwindow * win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GL_TRUE);
    }

    switch (key) {
        case GLFW_KEY_I:
            renderables.front().applyFy((action == GLFW_RELEASE) ? Direction::none : Direction::up);
            break;
        case GLFW_KEY_K:
            renderables.front().applyFy((action == GLFW_RELEASE) ? Direction::none : Direction::down);
            break;
        case GLFW_KEY_J:
            renderables.front().applyFx((action == GLFW_RELEASE) ? Direction::none : Direction::left);
            break;
        case GLFW_KEY_L:
            renderables.front().applyFx((action == GLFW_RELEASE) ? Direction::none : Direction::right);
            break;
        case GLFW_KEY_W:
            camera.moveForward((action == GLFW_RELEASE) ? Direction::none : Direction::up);
            break;
        case GLFW_KEY_S:
            camera.moveForward((action == GLFW_RELEASE) ? Direction::none : Direction::down);
            break;
        case GLFW_KEY_A:
            camera.moveSideways((action == GLFW_RELEASE) ? Direction::none : Direction::left);
            break;
        case GLFW_KEY_D:
            camera.moveSideways((action == GLFW_RELEASE) ? Direction::none : Direction::right);
            break;
        case GLFW_KEY_UP:
            camera.rotateVer((action == GLFW_RELEASE) ? Direction::none : Direction::up);
            break;
        case GLFW_KEY_DOWN:
            camera.rotateVer((action == GLFW_RELEASE) ? Direction::none : Direction::down);
            break;
        case GLFW_KEY_LEFT:
            camera.rotateHor((action == GLFW_RELEASE) ? Direction::none : Direction::left);
            break;
        case GLFW_KEY_RIGHT:
            camera.rotateHor((action == GLFW_RELEASE) ? Direction::none : Direction::right);
            break;
        case GLFW_KEY_U:
            renderables.front().enableRotation((action == GLFW_RELEASE) ? Rotation::none : Rotation::left);
            break;
        case GLFW_KEY_O:
            renderables.front().enableRotation((action == GLFW_RELEASE) ? Rotation::none : Rotation::right);
            break;
        case GLFW_KEY_KP_ADD:
            renderables.front().enableGrowth((action == GLFW_RELEASE) ? Growth::none : Growth::grow);
            break;
        case GLFW_KEY_KP_SUBTRACT:
            renderables.front().enableGrowth((action == GLFW_RELEASE) ? Growth::none : Growth::shrink);
            break;
        default:
            break;
    }

    // std::cout << "key_callback [" << key << "," << scancode << "," << action << "," << mods << "]" << std::endl;
}

void Application::windowFocusCallback(GLFWwindow * win, int focused) {
    // std::cout << "window_focus_callback" << std::endl;
}

void Application::windowIconifyCallback(GLFWwindow * win, int iconified) {
    // std::cout << "window_iconify_callback" << std::endl;
}

void Application::windowSizeCallback(GLFWwindow * win, int width, int height) {
    // std::cout << "resize " << width << ", " << height << std::endl;
    glViewport(0, 0, width, height);
}

void Application::initCallbacks() {

    auto error = [](int error, const char * description) {
        instance().errorCallback(error, description);
    };
    glfwSetErrorCallback(error);

    auto key = [](GLFWwindow * win, int key, int scancode, int action, int mods) {
        instance().keyCallback(win, key, scancode, action, mods);
    };
    glfwSetKeyCallback(window, key);

    auto focus = [](GLFWwindow * win, int focused) {
        instance().windowFocusCallback(win, focused);
    };
    glfwSetWindowFocusCallback(window, focus);

    auto iconify = [](GLFWwindow * win, int iconified) {
        instance().windowIconifyCallback(win, iconified);
    };
    glfwSetWindowIconifyCallback(window, iconify);

    auto size = [](GLFWwindow * win, int width, int length) {
        instance().windowSizeCallback(win, width, length);
    };
    glfwSetWindowSizeCallback(window, size);

    auto cursor = [](GLFWwindow * win, double x, double y) {
        instance().mouse.mouseMove((int)x, (int)y);
    };
    glfwSetCursorPosCallback(window, cursor);

    auto mouseButton = [](GLFWwindow * win, int button, int action, int mode) {
        Mouse::Button btn = Mouse::Button::Other;
        if (button == GLFW_MOUSE_BUTTON_1) {
            btn = Mouse::Button::LB;
        } else if (button == GLFW_MOUSE_BUTTON_2) {
            btn = Mouse::Button::MB;
        } else if (button == GLFW_MOUSE_BUTTON_3) {
            btn = Mouse::Button::RB;
        }
        if (action == GLFW_PRESS) {
            instance().mouse.buttonPress(btn);
        } else if (action == GLFW_RELEASE) {
            instance().mouse.buttonRelease(btn);
        }
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
    glEnable(GL_DEPTH_TEST);
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.update(dt);
    // camera->apply();
    ambientLight.apply();
    light.apply();

    for (Renderable & rend : renderables) {
        rend.update(dt);
        rend.draw();
    }

    glfwPollEvents();
    glfwSwapBuffers(window);

}

void Application::loop() {

    lastTime = std::chrono::high_resolution_clock::now();

    glClearColor(0.f, 0.f, 0.4f, 0.f);

    update(0.f);
    camera.apply();
    ambientLight.apply();
    light.apply();

    while (not glfwWindowShouldClose(window)) {

        const TimePoint now = std::chrono::high_resolution_clock::now();
        const float delta = std::chrono::duration_cast<Second>(now - lastTime).count();
        update(delta);
        lastTime = now;

    }

}

Application::~Application() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
}

Application::Application() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    initGL();
    initApplication();

    Shader & shader = ShaderManager::phong();

    renderables.emplace_back(models::sphere, shader);
    renderables.back().translate(glm::vec3(5.f, 0.f, 0.f));
    renderables.emplace_back(models::sphere, shader);
    renderables.back().translate(glm::vec3(-5.f, 0.f, 0.f));
    renderables.emplace_back(models::sphere, shader);
    renderables.back().translate(glm::vec3(0.f, 0.f, 5.f));
    renderables.emplace_back(models::sphere, shader);
    renderables.back().translate(glm::vec3(0.f, 0.f, -5.f));

    camera.addObserver(ShaderManager::constant());
    camera.addObserver(ShaderManager::lambert());
    camera.addObserver(ShaderManager::phong());
    camera.addObserver(ShaderManager::blinn());

    ambientLight.addObserver(ShaderManager::constant());
    ambientLight.addObserver(ShaderManager::lambert());
    ambientLight.addObserver(ShaderManager::phong());
    ambientLight.addObserver(ShaderManager::blinn());

    light.addObserver(ShaderManager::constant());
    light.addObserver(ShaderManager::lambert());
    light.addObserver(ShaderManager::phong());
    light.addObserver(ShaderManager::blinn());
}

Application & Application::instance() {
    if (not application) {
        application = new Application();
    }
    return *application;
}
