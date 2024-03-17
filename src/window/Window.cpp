#include "Window.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


GLFWwindow* Window::window;


int Window::initialise(WindowData& winData, const char* title)
{
    /* Init GLFWindow */
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    /* Create app window */
    window = glfwCreateWindow(winData.width, winData.height, title, winData.monitor, winData.share);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -2;
    }
    // create context
    glfwMakeContextCurrent(window);

    /* Init glad loader, for OpenGL */
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        // glad
        std::cerr << "Failed to initialize GLAD" << std::endl;
        terminate();
        return -3;
    }

    glViewport(winData.view_x, winData.view_y, winData.width, winData.height);
    glDepthRange(winData.zNear, winData.zFar);
    glfwSwapInterval(winData.swapInterval);
    glfwSetWindowUserPointer(window, &winData);
    return 0;
}


void Window::setCursorMode(int mode)
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void Window::terminate()
{
    /* Terminate GLFWwindow */
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::isShouldClose()
{
    /* Checking for closure */
    return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool flag)
{
    glfwSetWindowShouldClose(window, flag);
}

void Window::swapBuffers()
{
    /* Latest operation with window */
    glfwSwapBuffers(window);
}
