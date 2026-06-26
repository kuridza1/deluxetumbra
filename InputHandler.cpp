#include "InputHandler.h"

#include <iostream>

void InputHandler::init(GLFWwindow* window, Camera* camera, int windowWidth, int windowHeight)
{
    cam    = camera;
    lastX  = (float)windowWidth  / 2.0f;
    lastY  = (float)windowHeight / 2.0f;

    glfwSetWindowUserPointer(window, this);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
}

void InputHandler::processKeyboard(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam->ProcessKeyboard(0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam->ProcessKeyboard(1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam->ProcessKeyboard(2, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam->ProcessKeyboard(3, deltaTime);
}

void InputHandler::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* self = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    if (!self) return;

    if (self->firstMouse)
    {
        self->lastX      = (float)xpos;
        self->lastY      = (float)ypos;
        self->firstMouse = false;
    }

    float xoffset =  (float)xpos - self->lastX;
    float yoffset =  self->lastY - (float)ypos;   // invertovano: Y raste prema dole

    self->lastX = (float)xpos;
    self->lastY = (float)ypos;

    self->cam->ProcessMouseMovement(xoffset, yoffset);
}
