#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "GBuffer.h"
#include "Scene.h"
#include "BVH.h"

enum class RenderMode
{
    Basic = 1,
    Shadows = 2,
    Reflections = 3
};

class Renderer
{
public:
    bool init(int width, int height);
    void destroy();

    void geometryPass(const glm::mat4& view, const glm::mat4& projection);
    void shadowPass(const glm::vec3& lightPos);
    void aoPass(const glm::vec3& lightPos);
    void lightingPass( const glm::vec3& lightPos, const glm::vec3& viewPos);
    void reflectionPass(const glm::vec3& viewPos, const glm::vec3& lightPos);
    void denoisePass();

    void setRenderMode(RenderMode mode);
    RenderMode getRenderMode() const;

    void drawMaterialUI();
private:
    int screenWidth = 0;
    int screenHeight = 0;

    GBuffer gbuffer;
    Scene scene;
    BVH bvh;

    Shader* geometryShader = nullptr;
    Shader* lightingShader = nullptr;
    Shader* shadowShader = nullptr;
    Shader* aoShader = nullptr;
    Shader* reflectionShader = nullptr;
    Shader* denoiseShader = nullptr;

    GLuint shadowTexture = 0;
	GLuint aoTexture = 0;
    GLuint reflectionTexture = 0;
    GLuint denoisedReflectionTexture = 0;

    glm::vec3 redWallColor = glm::vec3(0.75f, 0.1f, 0.1f);
    glm::vec3 greenWallColor = glm::vec3(0.1f, 0.75f, 0.1f);
    float redWallX = -2.5f;
    float greenWallX = 2.5f;
    float bleedStrength = 0.3f;

    
    RenderMode renderMode = RenderMode::Reflections;
};