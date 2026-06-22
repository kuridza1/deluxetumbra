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
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(0, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(1, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(2, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(3, dt);
}

void setColor(GLuint shaderID, const glm::vec3& color)
{
    glUniform3f(
        glGetUniformLocation(shaderID, "objectColor"),
        color.x, color.y, color.z);
}

void drawCube()
{
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 6);
    GLFWwindow* window =
        glfwCreateWindow(WIDTH, HEIGHT, "Cornell Box", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "GLAD init failed\n";
        glfwTerminate();
        return -1;
    }
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, WIDTH, HEIGHT);

    float vertices[] =
    {
        // pos                // normal
        -0.5f,-0.5f,-0.5f,    0.0f,0.0f,-1.0f,
         0.5f,-0.5f,-0.5f,    0.0f,0.0f,-1.0f,
         0.5f, 0.5f,-0.5f,    0.0f,0.0f,-1.0f,
         0.5f, 0.5f,-0.5f,    0.0f,0.0f,-1.0f,
        -0.5f, 0.5f,-0.5f,    0.0f,0.0f,-1.0f,
        -0.5f,-0.5f,-0.5f,    0.0f,0.0f,-1.0f,

        -0.5f,-0.5f, 0.5f,    0.0f,0.0f,1.0f,
         0.5f,-0.5f, 0.5f,    0.0f,0.0f,1.0f,
         0.5f, 0.5f, 0.5f,    0.0f,0.0f,1.0f,
         0.5f, 0.5f, 0.5f,    0.0f,0.0f,1.0f,
        -0.5f, 0.5f, 0.5f,    0.0f,0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,    0.0f,0.0f,1.0f,

        -0.5f, 0.5f, 0.5f,   -1.0f,0.0f,0.0f,
        -0.5f, 0.5f,-0.5f,   -1.0f,0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,   -1.0f,0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,   -1.0f,0.0f,0.0f,
        -0.5f,-0.5f, 0.5f,   -1.0f,0.0f,0.0f,
        -0.5f, 0.5f, 0.5f,   -1.0f,0.0f,0.0f,

         0.5f, 0.5f, 0.5f,    1.0f,0.0f,0.0f,
         0.5f, 0.5f,-0.5f,    1.0f,0.0f,0.0f,
         0.5f,-0.5f,-0.5f,    1.0f,0.0f,0.0f,
         0.5f,-0.5f,-0.5f,    1.0f,0.0f,0.0f,
         0.5f,-0.5f, 0.5f,    1.0f,0.0f,0.0f,
         0.5f, 0.5f, 0.5f,    1.0f,0.0f,0.0f,

        -0.5f,-0.5f,-0.5f,    0.0f,-1.0f,0.0f,
         0.5f,-0.5f,-0.5f,    0.0f,-1.0f,0.0f,
         0.5f,-0.5f, 0.5f,    0.0f,-1.0f,0.0f,
         0.5f,-0.5f, 0.5f,    0.0f,-1.0f,0.0f,
        -0.5f,-0.5f, 0.5f,    0.0f,-1.0f,0.0f,
        -0.5f,-0.5f,-0.5f,    0.0f,-1.0f,0.0f,

        -0.5f, 0.5f,-0.5f,    0.0f,1.0f,0.0f,
         0.5f, 0.5f,-0.5f,    0.0f,1.0f,0.0f,
         0.5f, 0.5f, 0.5f,    0.0f,1.0f,0.0f,
         0.5f, 0.5f, 0.5f,    0.0f,1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,    0.0f,1.0f,0.0f,
        -0.5f, 0.5f,-0.5f,    0.0f,1.0f,0.0f
    };

    // Quad for lighting pass
    GLuint quadVAO = 0;
    GLuint quadVBO;

    float quadVertices[] = {
        // pos      // uv
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Shader geometryShader("geometry.vert", "geometry.frag");
    Shader lightingShader("lighting.vert", "lighting.frag");

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // G-Buffer setup
    GLuint gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    GLuint gPosition, gNormal, gAlbedo;

    // Position texture
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // Normal texture
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // Albedo texture
    glGenTextures(1, &gAlbedo);
    glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

    // Depth renderbuffer
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    GLuint attachments[3] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };
    glDrawBuffers(3, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "GBuffer not complete!\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Light positions (same as original)
    glm::vec3 lightSamples[4] = {
        {-0.3f, 4.2f, -0.3f},
        { 0.3f, 4.2f, -0.3f},
        {-0.3f, 4.2f,  0.3f},
        { 0.3f, 4.2f,  0.3f}
    };

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        // GEOMETRY PASS
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        geometryShader.use();

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix((float)WIDTH / HEIGHT);

        glUniformMatrix4fv(glGetUniformLocation(geometryShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(geometryShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        // FLOOR
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(geometryShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        setColor(geometryShader.ID, glm::vec3(0.8f));
        drawCube();

        // CEILING
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.5f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(geometryShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        setColor(geometryShader.ID, glm::vec3(0.8f));
        drawCube();

        // LEFT WALL (RED)
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 5.0f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(geometryShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        setColor(geometryShader.ID, glm::vec3(0.75f, 0.1f, 0.1f));
        drawCube();

        // RIGHT WALL (GREEN)
        model = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 5.0f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(geometryShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        setColor(geometryShader.ID, glm::vec3(0.1f, 0.75f, 0.1f));
        drawCube();

        // BACK WALL
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -2.5f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(geometryShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        setColor(geometryShader.ID, glm::vec3(0.8f));
        drawCube();

        // SMALL BOX
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.9f, 0.5f, 0.8f));
        model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(geometryShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        setColor(geometryShader.ID, glm::vec3(0.85f));
        drawCube();

        // LARGE BOX
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 1.0f, -0.6f));
        model = glm::rotate(model, glm::radians(18.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.3f, 3.0f, 1.3f));
        glUniformMatrix4fv(glGetUniformLocation(geometryShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        setColor(geometryShader.ID, glm::vec3(0.85f));
        drawCube();

        // LIGHTING PASS
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        lightingShader.use();

        glUniform1i(glGetUniformLocation(lightingShader.ID, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.ID, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(lightingShader.ID, "gAlbedo"), 2);

        // Set light positions
        for (int i = 0; i < 4; i++)
        {
            std::string name = "lightPos[" + std::to_string(i) + "]";
            glUniform3f(glGetUniformLocation(lightingShader.ID, name.c_str()),
                lightSamples[i].x,
                lightSamples[i].y,
                lightSamples[i].z);
        }

        // Set view position
        glUniform3f(glGetUniformLocation(lightingShader.ID, "viewPos"),
            camera.Position.x, camera.Position.y, camera.Position.z);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}