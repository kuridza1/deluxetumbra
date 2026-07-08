#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "GBuffer.h"

struct ShadowObject {
    glm::mat4 model;
};

class Renderer
{
public:
    bool init(int width, int height);
    void destroy();

    void geometryPass(const glm::mat4& view, const glm::mat4& projection);
    void shadowPass(const glm::vec3& lightPos);
    void lightingPass( const glm::vec3& lightPos, const glm::vec3& viewPos);

private:
    void drawCube();
    void setColor(GLuint shaderID, const glm::vec3& color);
    void setEmission(float e);

    GLuint VAO = 0;
    GLuint VBO = 0;

    GLuint quadVAO = 0;
    GLuint quadVBO = 0;

    int screenWidth = 0;
    int screenHeight = 0;

    std::vector<ShadowObject> shadowObjects;

    Shader* geometryShader = nullptr;
    Shader* lightingShader = nullptr;
    Shader* shadowShader = nullptr;

    GLuint shadowTexture = 0;

    GBuffer gbuffer;
};