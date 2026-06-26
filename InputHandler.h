#pragma once

#include <GLFW/glfw3.h>
#include "Camera.h"


class InputHandler
{
public:
    void init(GLFWwindow* window, Camera* camera, int windowWidth, int windowHeight);

    void processKeyboard(GLFWwindow* window, float deltaTime);

private:
    Camera* cam = nullptr;

    bool  firstMouse = true;
    float lastX      = 0.0f;
    float lastY      = 0.0f;

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
};
