#include "../include/InputHandler.h"
#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_glfw.h"  
#include <iostream>

void InputHandler::init(GLFWwindow* window, Camera* camera, Renderer* renderer, int windowWidth, int windowHeight)
{
    cam = camera;
    this->renderer = renderer;

    lastX = (float)windowWidth / 2.0f;
    lastY = (float)windowHeight / 2.0f;

    glfwSetWindowUserPointer(window, this);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void InputHandler::processKeyboard(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->ProcessKeyboard(0, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->ProcessKeyboard(1, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->ProcessKeyboard(2, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->ProcessKeyboard(3, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        renderer->setRenderMode(RenderMode::Basic);

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        renderer->setRenderMode(RenderMode::Shadows);

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        renderer->setRenderMode(RenderMode::Reflections);
}



void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods); // <- prosledi ImGui-ju prvo

    auto* self = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    if (!self)
        return;

    if (ImGui::GetIO().WantCaptureMouse)
        return; // ImGui koristi klik (npr. scrollbar, slider) - ne diraj kameru

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            self->mousePressed = true;
            self->firstMouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (action == GLFW_RELEASE)
        {
            self->mousePressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void InputHandler::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos); // <- prosledi ImGui-ju prvo

    auto* self = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    if (!self)
        return;
    if (!self->mousePressed)
        return;
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (self->firstMouse)
    {
        self->lastX = (float)xpos;
        self->lastY = (float)ypos;
        self->firstMouse = false;
        return;
    }
    float xoffset = (float)xpos - self->lastX;
    float yoffset = self->lastY - (float)ypos;
    self->lastX = (float)xpos;
    self->lastY = (float)ypos;
    self->cam->ProcessMouseMovement(xoffset, yoffset);
}