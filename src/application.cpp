//
// Created by Filip Peterek on 22/09/2020.
//

#include "application.hpp"

#include <algorithm>
#include <iostream>


Application * Application::application = nullptr;

void Application::errorCallback(int error, const char * description) {
    (void)error;
    std::cerr << description << std::endl;
}

void Application::keyCallback(GLFWwindow * win, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GL_TRUE);
    }

    if (not scenePtr) {
        return;
    }

    switch (key) {
        case GLFW_KEY_I:
            if (hasSelected()) {
                getSelected().applyFy((action == GLFW_RELEASE) ? Direction::none : Direction::up);
            }
            break;
        case GLFW_KEY_K:
            if (hasSelected()) {
                getSelected().applyFy((action == GLFW_RELEASE) ? Direction::none : Direction::down);
            }
            break;
        case GLFW_KEY_J:
            if (hasSelected()) {
                getSelected().applyFx((action == GLFW_RELEASE) ? Direction::none : Direction::left);
            }
            break;
        case GLFW_KEY_L:
            if (hasSelected()) {
                getSelected().applyFx((action == GLFW_RELEASE) ? Direction::none : Direction::right);
            }
            break;
        case GLFW_KEY_W:
            scene().camera.moveForward((action == GLFW_RELEASE) ? Direction::none : Direction::up);
            break;
        case GLFW_KEY_S:
            scene().camera.moveForward((action == GLFW_RELEASE) ? Direction::none : Direction::down);
            break;
        case GLFW_KEY_A:
            scene().camera.moveSideways((action == GLFW_RELEASE) ? Direction::none : Direction::left);
            break;
        case GLFW_KEY_D:
            scene().camera.moveSideways((action == GLFW_RELEASE) ? Direction::none : Direction::right);
            break;
        case GLFW_KEY_UP:
            scene().camera.rotateVer((action == GLFW_RELEASE) ? Direction::none : Direction::up);
            break;
        case GLFW_KEY_DOWN:
            scene().camera.rotateVer((action == GLFW_RELEASE) ? Direction::none : Direction::down);
            break;
        case GLFW_KEY_LEFT:
            scene().camera.rotateHor((action == GLFW_RELEASE) ? Direction::none : Direction::left);
            break;
        case GLFW_KEY_RIGHT:
            scene().camera.rotateHor((action == GLFW_RELEASE) ? Direction::none : Direction::right);
            break;
        case GLFW_KEY_U:
            if (hasSelected()) {
                getSelected().enableRotation((action == GLFW_RELEASE) ? Rotation::none : Rotation::left);
            }
            break;
        case GLFW_KEY_O:
            if (hasSelected()) {
                getSelected().enableRotation((action == GLFW_RELEASE) ? Rotation::none : Rotation::right);
            }
            break;
        case GLFW_KEY_KP_ADD:
            if (hasSelected()) {
                getSelected().enableGrowth((action == GLFW_RELEASE) ? Growth::none : Growth::grow);
            }
            break;
        case GLFW_KEY_KP_SUBTRACT:
            if (hasSelected()) {
                getSelected().enableGrowth((action == GLFW_RELEASE) ? Growth::none : Growth::shrink);
            }
            break;
        default:
            break;
    }

    // std::cout << "key_callback [" << key << "," << scancode << "," << action << "," << mods << "]" << std::endl;
}

void Application::windowFocusCallback(GLFWwindow * win, int focused) {
    (void)win;
    (void)focused;
    // std::cout << "window_focus_callback" << std::endl;
}

void Application::windowIconifyCallback(GLFWwindow * win, int iconified) {
    (void)win;
    (void)iconified;
    // std::cout << "window_iconify_callback" << std::endl;
}

void Application::windowSizeCallback(GLFWwindow * win, int width, int height) {
    (void)win;
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
            btn = Mouse::Button::RB;
        } else if (button == GLFW_MOUSE_BUTTON_3) {
            btn = Mouse::Button::MB;
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
        const char * dsc;
        glfwGetError(&dsc);
        std::cout << dsc << std::endl;
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
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    scene().update(dt);

    glfwPollEvents();
    glfwSwapBuffers(window);

}

void Application::loop() {

    lastTime = std::chrono::high_resolution_clock::now();

    glClearColor(0.f, 0.f, 0.4f, 0.f);

    update(0.0);
    scene().init();

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

    Object::Builder objBuilder;
    Scene::Builder sceneBuilder;

    scenePtr = sceneBuilder
        .emplaceLight(glm::vec3 { 1.f }, glm::vec3 { 0.f, 1.f, 0.f }, gl::Light::Point)
        .emplaceLight(glm::vec3 { 1.f }, glm::vec3 { 0.f, 0.f, 8.0f }, gl::Light::Point)
        .emplaceLight(glm::vec3 { 1.f }, glm::vec3 { -3.f, -8.f, 0.f }, gl::Light::Directional)
        // .emplaceLight(glm::vec3 { 1.f }, glm::vec3 { 0.f, 6.f, 6.0f })
        // .emplaceLight(glm::vec3 { 1.f }, glm::vec3 { 0.f, -6.f, -6.0f })
        // .emplaceLight(glm::vec3 { 1.f }, glm::vec3 { 0.f, 0.f, -8.0f })
        // .emplaceLight(glm::vec3 { 1.f }, glm::vec3 { 0.f, 0.f, 8.0f })
        .emplaceAmbientLight(glm::vec3 { .1f })
        .addObject(
            objBuilder
            .emplaceObject(ModelLoader::get("a380_alt/11803_Airplane_v1_l1"), ShaderManager::lambert(), TextureManager::getOrEmplace("grass", "resources/textures/grass.jpg"))
            .setPosition(0.f, 5.f, -5.f).setScale(0.005f, 0.005f, 0.005f)
            .setRotation(-M_PI/2, glm::vec3 { 1.f, 0.f, 0.f })
            .build()

        )
        .setCameraPosition(0.f, 1.f, 3.f)
        .build();

}

Application & Application::instance() {
    if (not application) {
        application = new Application();
        Mouse::instance().registerObserver(*application);
    }
    return *application;
}

Scene & Application::scene() {
    if (not scenePtr) {
        throw std::runtime_error("Application: Scene is not initialized");
    }
    return *scenePtr;
}

void Application::deselect() {
    if (hasSelected()) {
        getSelected().setColor(Object::defaultColor);
    }
    selected = noneSelected;
}

void Application::onButtonPress(const MouseData & mouseData) {
    if (mouseData.lbPressed()) {
        selectObject(mouseData.x, mouseData.y);
    } else if (mouseData.rbPressed()) {
        emplaceObject(mouseData.x, mouseData.y);
    }
}

bool Application::hasSelected() {
    return selected != noneSelected and selected < scene().objects.size();
}

Object & Application::getSelected() {
    if (not hasSelected()) {
        throw std::runtime_error("No object selected");
    }
    return scene().objects[selected];
}

void Application::selectObject(int mouseX, int mouseY) {

    // GLbyte color[4];
    // GLfloat depth;
    GLuint objectId;

    const GLint x = mouseX;
    const GLint y = bufferHeight - mouseY;

    glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &objectId);

    std::cout << "Selected object: " << objectId << std::endl;

    deselect();
    if (objectId) {
        scene().removeObject(objectId);
        // selected = scene().indexOf(objectId);
        // getSelected().setColor(Object::secondaryColor);
    }

}

void Application::emplaceObject(const int mouseX, const int mouseY) {

    GLfloat depth;

    const GLint x = mouseX;
    const GLint y = bufferHeight - mouseY;

    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    glm::vec3 screenX = glm::vec3 { x, y, depth };
    glm::vec4 viewport { 0, 0, bufferWidth, bufferHeight };
    auto pos = glm::unProject(screenX, scene().camera.view(), scene().camera.projection(), viewport);

    scene().objects.emplace_back(
        Object::Builder()
            .emplaceObject(ModelLoader::get("alt_tree/lowpolytree"), ShaderManager::blinn(),
                           TextureManager::get("grass"))
            .setPosition(pos.x, pos.y + 1.6, pos.z)
            .build()
    );
}

void Application::notify(EventType eventType, void * object) {
    if (eventType == EventType::MouseButtonPressed) {
        onButtonPress(((Mouse*)object)->data());
    }
}
