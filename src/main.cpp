#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Include the standard C++ headers  
#include <cstdlib>
#include <cstdio>

#include "shaders.hpp"

#include "application.hpp"


static void error_callback(int error, const char * description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

static void window_focus_callback(GLFWwindow * window, int focused) {
    printf("window_focus_callback \n");
}

static void window_iconify_callback(GLFWwindow * window, int iconified) {
    printf("window_iconify_callback \n");
}

static void window_size_callback(GLFWwindow * window, int width, int height) {
	printf("resize %d, %d \n", width, height);
	glViewport(0, 0, width, height);
}

static void cursor_callback(GLFWwindow * window, double x, double y) {
    printf("cursor_callback \n");
}

static void button_callback(GLFWwindow * window, int button, int action, int mode) {
	if (action == GLFW_PRESS) {
	    printf("button_callback [%d,%d,%d]\n", button, action, mode);
	}
}


int main(int argc, const char * argv[]) {

    glfwSetErrorCallback(error_callback);

    Application::run();

}


