/*
* Voxel_engine
 *    window.h
 * Create on: 10 March 2024
 *    Author: Marc_Time
 * according to the lessons: Minecraft on C++ - MihailRis
 */
#pragma once
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_


struct GLFWwindow;
struct GLFWmonitor;


struct WindowData
{
    // window size
    int width = 1280;
    int height = 720;
    // window viewport cords
    int view_x = 0;
    int view_y = 0;
    // additional GLFW window attr
    GLFWmonitor* monitor = nullptr;
    GLFWwindow* share = nullptr;
    // V-Sync
    int swapInterval = 1;
    float zNear = 0.01;
    float zFar = 300.0;
    // uniform cords (in last shader)
    unsigned int u_resolution_location;
};


class Window
{
public:
    static GLFWwindow* window;
    static int initialise(WindowData& winData, const char* title);
    static void terminate();

    static void setCursorMode(int mode);
    static bool isShouldClose();
    static void setShouldClose(bool flag);
    static void swapBuffers();
};

#endif
