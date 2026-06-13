#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int WIDTH = 1280;
const int HEIGHT = 720;

Camera camera;

bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(0, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(1, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(2, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(3, deltaTime);
}



int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(
            WIDTH,
            HEIGHT,
            "De Lux et Umbra",
            nullptr,
            nullptr);

    if (!window)
    {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD init failed\n";
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);

    float vertices[] =
    {
        // back face
        -0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,

         0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,

        // front face
        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,

         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,

        // left face
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,

        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        // right face
         0.5f, 0.5f, 0.5f,
         0.5f, 0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,

         0.5f,-0.5f,-0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,

         // bottom face
         -0.5f,-0.5f,-0.5f,
          0.5f,-0.5f,-0.5f,
          0.5f,-0.5f, 0.5f,

          0.5f,-0.5f, 0.5f,
         -0.5f,-0.5f, 0.5f,
         -0.5f,-0.5f,-0.5f,

         // top face
         -0.5f, 0.5f,-0.5f,
          0.5f, 0.5f,-0.5f,
          0.5f, 0.5f, 0.5f,

          0.5f, 0.5f, 0.5f,
         -0.5f, 0.5f, 0.5f,
         -0.5f, 0.5f,-0.5f
    };

    Shader shader(
        "geometry.vert",
        "geometry.frag");

    GLuint VAO;
    GLuint VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0);

    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame =
            static_cast<float>(glfwGetTime());

        deltaTime =
            currentFrame - lastFrame;

        lastFrame =
            currentFrame;

        processInput(window, deltaTime);

        glClearColor(
            0.1f,
            0.1f,
            0.15f,
            1.0f);

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 model =
            glm::mat4(1.0f);

        glm::mat4 view =
            camera.GetViewMatrix();

        glm::mat4 projection =
            camera.GetProjectionMatrix(
                static_cast<float>(WIDTH) /
                static_cast<float>(HEIGHT));

        glUniformMatrix4fv(
            glGetUniformLocation(
                shader.ID,
                "model"),
            1,
            GL_FALSE,
            glm::value_ptr(model));

        glUniformMatrix4fv(
            glGetUniformLocation(
                shader.ID,
                "view"),
            1,
            GL_FALSE,
            glm::value_ptr(view));

        glUniformMatrix4fv(
            glGetUniformLocation(
                shader.ID,
                "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(projection));

        glBindVertexArray(VAO);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}