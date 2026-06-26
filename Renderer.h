#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GBuffer.h"
#include "Shader.h"

class Renderer
{
public:
    Renderer() = default;

    bool init(int width, int height);

    void geometryPass(const glm::mat4& view, const glm::mat4& projection);

    void lightingPass(const glm::vec3& lightPos, const glm::vec3& viewPos);

    void destroy();
    ~Renderer() { destroy(); }

private:
    void setColor(GLuint shaderID, const glm::vec3& color);
    void setEmission(float e);
    void drawCube();

    GLuint VAO     = 0;
    GLuint VBO     = 0;
    GLuint quadVAO = 0;
    GLuint quadVBO = 0;

    Shader* geometryShader = nullptr;
    Shader* lightingShader = nullptr;

    GBuffer gbuffer;

    int screenWidth  = 0;
    int screenHeight = 0;
};
