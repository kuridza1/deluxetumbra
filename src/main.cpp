#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "../include/Camera.h"
#include "../include/InputHandler.h"
#include "../include/Renderer.h"

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_opengl3.h"

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, mode->width, mode->height);

    Camera       camera(glm::vec3(0.0f, 2.0f, 8.79583f));
    InputHandler input;
    Renderer     renderer;

    input.init(window, &camera, &renderer, mode->width, mode->height);

    if (!renderer.init(mode->width, mode->height)) {
        glfwTerminate();
        return -1;
    }

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

        if (renderer.getRenderMode() == RenderMode::Shadows)
        {
            renderer.shadowPass();
        }

        if (renderer.getRenderMode() == RenderMode::Reflections)
        {
            renderer.shadowPass();
            renderer.reflectionPass(camera.Position);
            renderer.denoisePass();
        }

        renderer.lightingPass(camera.Position);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiCond_FirstUseEver);

        ImGui::Begin("Material Controls");

        renderer.drawMaterialUI();

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
