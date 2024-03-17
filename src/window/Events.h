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


typedef unsigned int uint;

enum
{
    MOUSE_BUTTONS = 1024,
    MAX_KEYS = 1032,
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

    static void toggleCursor();
};

#endif
