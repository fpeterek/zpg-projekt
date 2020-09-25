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
GLuint Application::shaderProgram = glCreateProgram();
int Application::bufferWidth = 0;
int Application::bufferHeight = 0;
float Application::bufferRatio = 0.f;

float Application::points[9] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
};

void Application::compileShaders() {

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &Shaders::vertexShader, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &Shaders::fragmentShader, nullptr);
    glCompileShader(fragmentShader);

    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);

}

bool Application::checkShaders() {

    GLint status;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar * strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(shaderProgram, infoLogLength, nullptr, strInfoLog);
        std::cerr << "Linker failure: " << strInfoLog << std::endl;
        delete[] strInfoLog;
    }

    return status != GL_FALSE;

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
    compileShaders();
    if (not checkShaders()) {
        throw std::runtime_error("Shaders could not be initialized");
    }
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
    initGLFWContext();
    initWindow();
    initGLEW();
    initShaders();
    printInfo();
}

void Application::initViewport() {
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    bufferRatio = bufferWidth / (float)bufferHeight;
    glViewport(0, 0, bufferWidth, bufferHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-bufferRatio, bufferRatio, -1.f, 1.f, 1.f, -1.f);
}

void Application::initApplication() {
    initVBO();
    initVAO();
    initViewport();
}

void Application::init() {
    initGL();
    initApplication();
}

void Application::run() {
    init();
    loop();
    cleanup();
}

void Application::update(const float dt) {

    std::cout << "Time delta: " << dt << std::endl;

    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);
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
