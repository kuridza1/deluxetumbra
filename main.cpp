#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Camera.h"
#include "InputHandler.h"
#include "Renderer.h"

const int WIDTH  = 1280;
const int HEIGHT = 720;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 6);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Cornell Box", monitor, nullptr);
    if (!window)
    {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD init failed\n";
        glfwTerminate();
        return -1;
    }

    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, mode->width, mode->height);

    Camera       camera;
    InputHandler input;
    Renderer     renderer;

    input.init(window, &camera, WIDTH, HEIGHT);

    if (!renderer.init(mode->width, mode->height)) {
        glfwTerminate();
        return -1;
    }

    const glm::vec3 lightPos(0.0f, 4.0f, 0.0f);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime  = currentFrame - lastFrame;
        lastFrame  = currentFrame;

        input.processKeyboard(window, deltaTime);

        glm::mat4 view       = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix((float)mode->width / mode->height);

        renderer.geometryPass(view, projection);
        renderer.shadowPass(lightPos);
		renderer.reflectionPass(camera.Position, lightPos);
        renderer.denoisePass();
        renderer.lightingPass(lightPos, camera.Position);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
