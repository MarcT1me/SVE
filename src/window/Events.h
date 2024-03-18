/*
* Voxel_cpp
*     Events.h
* Create on: 10 March 2024
*    Author: Marc_Time
* according to the lessons: Minecraft on C++ - MihailRis
*/
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#ifndef WINDOW_EVENTS_H_
#define WINDOW_EVENTS_H_
// others
#include <glad/glad.h>
#include <GLFW/glfw3.h>


typedef unsigned int uint;

// BORDERS
enum
{
    SVE_ZERO_KEY = 0,
    SVE_MOUSE_BUTTONS_BORDER = 1024,
    SVE_SCROLL_BORDER = 1032,
    SVE_MAX_KEYS = 1036,
};
// MOUSE
enum
{
    SVE_MOUSE_LEFT_BUTTON = GLFW_MOUSE_BUTTON_1,
    SVE_MOUSE_RIGHT_BUTTON = GLFW_MOUSE_BUTTON_2,
    SVE_MOUSE_MIDDLE_BUTTON = GLFW_MOUSE_BUTTON_3,
    SVE_MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4,
    SVE_MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5,
    SVE_MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6,
    SVE_MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7,
    SVE_MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8,
    // scroll
    SVE_MOUSE_SCROLL_DOWN = 0,
    SVE_MOUSE_SCROLL_UP = 1,
    SVE_MOUSE_SCROLL_LEFT = 2,
    SVE_MOUSE_SCROLL_RIGHT = 3,
};


class Events
{
public:
    // event field
    static bool* _keys;
    static uint* _frames;
    static uint _current;
    static float deltaX, deltaY;
    static float x, y;
    static bool _cursor_locked, _cursor_started;
    // main loop functions
    static int initialize();
    static void pullEvents();
    // checking the status
    static bool pressed(int keycode);
    static bool jPressed(int keycode);

    static bool clicked(int button);
    static bool jClicked(int button);
    static bool jScroll(short int direction);

    static void toggleCursor();
};

#endif
