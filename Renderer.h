#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "GBuffer.h"

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
};

struct ShadowObject
{
    glm::mat4 model;
    AABB bounds;
    int id;

};

struct SceneObject
{
    glm::mat4 model;
    glm::vec3 color;
    float emission;
    float reflectivity;
};

struct BVHNode
{
    AABB bounds;

    int left = -1;
    int right = -1;

    int object = -1;

    bool leaf = false;
};

struct GPUBVHNode
{
    glm::vec4 min;
    glm::vec4 max;

    int left;
    int right;
    int object;

    int leaf;
};

struct GPUObject
{
    glm::mat4 inverseModel;
    glm::vec4 color;
};

class Renderer
{
public:
    bool init(int width, int height);
    void destroy();

    void geometryPass(const glm::mat4& view, const glm::mat4& projection);
    void shadowPass(const glm::vec3& lightPos);
    void lightingPass( const glm::vec3& lightPos, const glm::vec3& viewPos);
    void reflectionPass(const glm::vec3& cameraPos);
    void denoisePass();
    void buildScene();
private:
    void drawCube();
    void setColor(GLuint shaderID, const glm::vec3& color);
    void setEmission(float e);
    void setReflectivity(float r);
    GLuint VAO = 0;
    GLuint VBO = 0;

    GLuint quadVAO = 0;
    GLuint quadVBO = 0;

    int screenWidth = 0;
    int screenHeight = 0;

    std::vector<ShadowObject> shadowObjects;
    std::vector<BVHNode> bvhNodes;

    Shader* geometryShader = nullptr;
    Shader* lightingShader = nullptr;
    Shader* shadowShader = nullptr;

    GLuint shadowTexture = 0;
    GLuint bvhSSBO = 0;
    GLuint objectSSBO = 0;

    GBuffer gbuffer;

    GLuint reflectionTexture = 0;
    Shader* reflectionShader = nullptr;
    Shader* denoiseShader = nullptr;
    GLuint denoisedReflectionTexture = 0;

    std::vector<SceneObject> sceneObjects;

    AABB computeBounds(const glm::mat4& model);

    glm::vec3 redWallColor = glm::vec3(0.75f, 0.1f, 0.1f);
    glm::vec3 greenWallColor = glm::vec3(0.1f, 0.75f, 0.1f);
    float redWallX = -2.5f;
    float greenWallX = 2.5f;
    float bleedStrength = 0.3f;

    int buildBVH(int begin, int end);

    void uploadBVH();

};