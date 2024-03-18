// ReSharper disable CppParameterNeverUsed
// ReSharper disable CppClangTidyBugproneBranchClone
#include "Events.h"
#include "Window.h"

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


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        Events::_keys[SVE_ZERO_KEY + key] = true;
        Events::_frames[SVE_ZERO_KEY + key] = Events::_current;
    }
    else if (action == GLFW_RELEASE)
    {
        Events::_keys[key] = false;
        Events::_frames[key] = Events::_current;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        Events::_keys[SVE_MOUSE_BUTTONS_BORDER + button] = true;
        Events::_frames[SVE_MOUSE_BUTTONS_BORDER + button] = Events::_current;
    }
    else if (action == GLFW_RELEASE)
    {
        Events::_keys[SVE_MOUSE_BUTTONS_BORDER + button] = false;
        Events::_frames[SVE_MOUSE_BUTTONS_BORDER + button] = Events::_current;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // y scroll
    int btn = SVE_SCROLL_BORDER + 0 + (yoffset > 0.0);
    if (yoffset != 0.0)
    {
        Events::_keys[btn] = true;
        Events::_frames[btn] = Events::_current;
    }
    else
    {
        Events::_keys[btn] = true;
        Events::_frames[btn] = Events::_current;
    }
    // x scroll
    btn = SVE_SCROLL_BORDER + 2 + (xoffset > 0.0);
    if (xoffset != 0.0)
    {
        Events::_keys[btn] = true;
        Events::_frames[btn] = Events::_current;
    }
    else
    {
        Events::_keys[btn] = true;
        Events::_frames[btn] = Events::_current;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Events::_cursor_started)
    {
        Events::deltaX += xpos - Events::x;
        Events::deltaY += ypos - Events::y;
    }
    else
    {
        Events::_cursor_started = true;
    }
    Events::x = xpos;
    Events::y = ypos;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // WinData using
    auto* winData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    winData->width = width;
    winData->height = height;
    glUniform2f(winData->u_resolution_location, static_cast<float>(width), static_cast<float>(height));
    // gl functions
    glDepthRange(winData->zNear, winData->zFar);
    glViewport(0, 0, width, height);
}


int Events::initialize()
{
    GLFWwindow* window = Window::window;
    _keys = new bool[SVE_MAX_KEYS];
    _frames = new uint[SVE_MAX_KEYS];

    memset(_keys, false, SVE_MAX_KEYS * sizeof(bool));
    memset(_frames, 0, SVE_MAX_KEYS * sizeof(uint));

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    return 0;
}


bool Events::pressed(int keycode)
{
    if (keycode < SVE_ZERO_KEY || keycode >= SVE_MOUSE_BUTTONS_BORDER)
        return false;
    return _keys[SVE_ZERO_KEY + keycode];
}

bool Events::jPressed(int keycode)
{
    return pressed(keycode) && _frames[SVE_ZERO_KEY + keycode] == _current;
}

bool Events::clicked(int button)
{
    return _keys[SVE_MOUSE_BUTTONS_BORDER + button];
}

bool Events::jClicked(int button)
{
    return clicked(button) && _frames[SVE_MOUSE_BUTTONS_BORDER + button] == _current;
}

bool Events::jScroll(short int direction)
{
    return _keys[1032 + direction] && _frames[1032 + direction] == _current;
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
