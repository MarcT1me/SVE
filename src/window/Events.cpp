#include "Events.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstring>


bool* Events::_keys;
uint* Events::_frames;
uint Events::_current = 0;
float Events::deltaX = 0.0f;
float Events::deltaY = 0.0f;
float Events::x = 0.0f;
float Events::y = 0.0f;
bool Events::_cursor_locked = false;
bool Events::_cursor_started = false;


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Events::_cursor_started)
    {
        Events::deltaX += xpos-Events::x;
        Events::deltaY += ypos-Events::y;
    }
    else
    {
        Events::_cursor_started = true;
    }
    Events::x = xpos;
    Events::y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        Events::_keys[MOUSE_BUTTONS + button] = true;
        Events::_frames[MOUSE_BUTTONS + button] = Events::_current;
    }
    else if (action == GLFW_RELEASE)
    {
        Events::_keys[MOUSE_BUTTONS + button] = false;
        Events::_frames[MOUSE_BUTTONS + button] = Events::_current;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        Events::_keys[key] = true;
        Events::_frames[key] = Events::_current;
    }
    else if (action == GLFW_RELEASE)
    {
        Events::_keys[key] = false;
        Events::_frames[key] = Events::_current;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    auto* winData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    winData->width = width;
    winData->height = height;
    glUniform2f(winData->u_resolution_location, static_cast<float>(width), static_cast<float>(height));
}

int Events::initialize()
{
    GLFWwindow* window = Window::window;
    _keys = new bool[MAX_KEYS];
    _frames = new uint[MAX_KEYS];

    memset(_keys, false, MAX_KEYS * sizeof(bool));
    memset(_frames, 0, MAX_KEYS * sizeof(uint));

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    return 0;
}

bool Events::pressed(int keycode)
{
    if (keycode < 0 || keycode >= MOUSE_BUTTONS)
        return false;
    return _keys[keycode];
}

bool Events::jPressed(int keycode)
{
    if (keycode < 0 || keycode > MOUSE_BUTTONS)
        return false;
    return _keys[keycode] && _frames[keycode] == _current;
}

bool Events::clicked(int button)
{
    return _keys[MOUSE_BUTTONS + button];
}

bool Events::jClicked(int button)
{
    return _keys[MOUSE_BUTTONS + button] && _frames[MOUSE_BUTTONS + button] == _current;
}

void Events::toggleCursor()
{
    _cursor_locked = !_cursor_locked;
    Window::setCursorMode(_cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Events::pullEvents()
{
    _current++;
    deltaX = 0.0f;
    deltaY = 0.0f;
    glfwPollEvents();
}
