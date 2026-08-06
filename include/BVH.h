#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
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
    int type;
    float emission;
    float reflectivity;
    float _pad[5];
};

enum ObjectType
{
    Cube = 0,
    Sphere = 1
};

struct SceneObject
{
    glm::mat4 model;
    glm::vec3 color;
    float emission;
    float reflectivity;
    ObjectType type;
};

struct ShadowObject
{
    glm::mat4 model;
    AABB bounds;
    int id;

};
struct EmissiveLight
{
    glm::vec3 position;
    glm::vec4 colorEmission;
};


class BVH {
public:
    AABB computeBounds(const glm::mat4& model);
    int buildBVH(int begin, int end);
    void uploadBVH();
    void uploadObjects();
    void updateEmissiveLights();
    void uploadLights();
    void destroy();

    std::vector<BVHNode> bvhNodes;
    GLuint bvhSSBO = 0;
    GLuint objectSSBO = 0;
    GLuint lightSSBO = 0;

    std::vector<ShadowObject> shadowObjects;
    std::vector<SceneObject> sceneObjects;
    std::vector<EmissiveLight> emissiveLights;
};