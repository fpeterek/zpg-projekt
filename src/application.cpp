//
// Created by Filip Peterek on 22/09/2020.
//

#include "application.hpp"

// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Application::Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

// Camera matrix
glm::mat4 Application::View = glm::lookAt(
        glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
);

// Model matrix : an identity matrix (model will be at the origin)
glm::mat4 Application::Model = glm::mat4(1.0f);

GLFWwindow * Application::window = nullptr;
Application::TimePoint Application::lastTime = std::chrono::high_resolution_clock::now();
GLuint Application::VBO = 0;
GLuint Application::VAO = 0;
std::vector<Shader> Application::shaders;
std::vector<Shader>::iterator Application::currentShader = shaders.begin();
int Application::bufferWidth = 0;
int Application::bufferHeight = 0;
float Application::bufferRatio = 0.f;
glm::mat4 Application::transformation(1.f);

float Application::points[9] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
};

void Application::errorCallback(int error, const char * description) {
    std::cerr << description << std::endl;
}

void Application::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    std::cout << "key_callback [" << key << "," << scancode << "," << action << "," << mods << "]" << std::endl;
}

void Application::windowFocusCallback(GLFWwindow * window, int focused) {
    std::cout << "window_focus_callback" << std::endl;
}

void Application::windowIconifyCallback(GLFWwindow * window, int iconified) {
    std::cout << "window_iconify_callback" << std::endl;
}

void Application::windowSizeCallback(GLFWwindow * window, int width, int height) {
    std::cout << "resize " << width << ", " << height << std::endl;
    glViewport(0, 0, width, height);
}

void Application::cursorPosCallback(GLFWwindow * window, double x, double y) {
    std::cout << "cursor_pos_callback" << std::endl;
}

void Application::buttonCallback(GLFWwindow * window, int button, int action, int mode) {
    if (action == GLFW_PRESS) {
        std::cout << "button_callback [" << button << "," << action << "," << mode << "]"<< std::endl;
    }
}

void Application::initCallbacks() {
    glfwSetErrorCallback(Application::errorCallback);
    glfwSetKeyCallback(window, Application::keyCallback);
    glfwSetWindowFocusCallback(window, Application::windowFocusCallback);
    glfwSetWindowIconifyCallback(window, Application::windowIconifyCallback);
    glfwSetWindowSizeCallback(window, Application::windowSizeCallback);
    glfwSetCursorPosCallback(window, Application::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Application::buttonCallback);
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
    currentShader = shaders.begin();
}

void Application::initVBO() {

    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

}

void Application::initVAO() {

    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    initVBO();
    initVAO();
    initCallbacks();
    initViewport();
    transformation = glm::rotate(transformation, -(float)M_PI/2 ,glm::vec3(0.0f, 0.0f, 1.0f));
    //transformation = glm::rotate(transformation, (float)M_PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    transformation = glm::translate(transformation, glm::vec3(-0.5f, 0.3f, 0.f));
    transformation = glm::scale(transformation, glm::vec3(0.5f));
}

void Application::init() {
    initGL();
    initApplication();
}

void Application::run() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    init();
    std::cout << "Init complete" << std::endl;
    loop();
    cleanup();
}

void Application::update(const float dt) {

    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLint modelMatrixID = currentShader->getUniformLocation("modelMatrix");
    currentShader->use();
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(transformation));
    glBindVertexArray(VAO);
    // draw triangles
    glDrawArrays(GL_TRIANGLES, 0, 3); //mode,first,count
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

void Application::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
