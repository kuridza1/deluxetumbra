#pragma once

#include "Renderer.h"
#include <GLFW/glfw3.h>
#include "Camera.h"


class InputHandler
{
public:
    void init(GLFWwindow* window, Camera* camera, Renderer* renderer, int windowWidth, int windowHeight);

    void processKeyboard(GLFWwindow* window, float deltaTime);

private:
    Camera* cam = nullptr;

    bool  firstMouse = true;
    float lastX      = 0.0f;
    float lastY      = 0.0f;

    bool mousePressed = false;

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    Renderer* renderer = nullptr;
};
