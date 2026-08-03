#include "BVH.h"
#include <iostream>
#include <algorithm>



AABB mergeAABB(const AABB& a, const AABB& b)
{
    return {
        glm::min(a.min, b.min),
        glm::max(a.max, b.max)
    };
}

int BVH::buildBVH(int begin, int end)
{
    if (begin >= end || begin < 0 || end > shadowObjects.size())
    {
        std::cerr << "Invalid BVH range: " << begin << " " << end << " size=" << shadowObjects.size() << std::endl;
        return -1;
    }

    BVHNode node;

    AABB bounds = shadowObjects[begin].bounds;

    for (int i = begin + 1; i < end; i++)
    {
        bounds = mergeAABB(bounds, shadowObjects[i].bounds);
    }

    node.bounds = bounds;

    int nodeIndex = bvhNodes.size();
    bvhNodes.push_back(node);


    int count = end - begin;


    // Leaf node
    if (count == 1)
    {
        bvhNodes[nodeIndex].leaf = true;
        bvhNodes[nodeIndex].object = shadowObjects[begin].id;
        return nodeIndex;
    }


    glm::vec3 size = bounds.max - bounds.min;

    int axis = 0;

    if (size.y > size.x && size.y > size.z)
        axis = 1;
    else if (size.z > size.x)
        axis = 2;


    std::sort(
        shadowObjects.begin() + begin,
        shadowObjects.begin() + end,
        [axis](const ShadowObject& a, const ShadowObject& b)
        {
            float ca = a.bounds.min[axis] + a.bounds.max[axis];
            float cb = b.bounds.min[axis] + b.bounds.max[axis];

            return ca < cb;
        }
    );


    int middle = (begin + end) / 2;


    int left = buildBVH(begin, middle);
    int right = buildBVH(middle, end);


    bvhNodes[nodeIndex].left = left;
    bvhNodes[nodeIndex].right = right;


    return nodeIndex;
}

void BVH::uploadBVH()
{
    std::vector<GPUBVHNode> gpuNodes;

    gpuNodes.reserve(bvhNodes.size());

    for (const BVHNode& node : bvhNodes)
    {
        GPUBVHNode gpu;

        gpu.min = glm::vec4(node.bounds.min, 0.0f);
        gpu.max = glm::vec4(node.bounds.max, 0.0f);

        gpu.left = node.left;
        gpu.right = node.right;
        gpu.object = node.object;
        gpu.leaf = node.leaf ? 1 : 0;

        gpuNodes.push_back(gpu);
    }

    if (bvhSSBO == 0)
        glGenBuffers(1, &bvhSSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, gpuNodes.size() * sizeof(GPUBVHNode), gpuNodes.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bvhSSBO);

    std::vector<GPUObject> gpuObjects;
    gpuObjects.reserve(sceneObjects.size());

    for (const SceneObject& obj : sceneObjects)
    {
        GPUObject gpu;
        gpu.inverseModel = glm::inverse(obj.model);
        gpu.color = glm::vec4(obj.color, 1.0f);
        gpu.type = obj.type;
        gpu.emission = obj.emission;
        gpu.reflectivity = obj.reflectivity;
        gpuObjects.push_back(gpu);
    }

    if (objectSSBO == 0)
        glGenBuffers(1, &objectSSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, objectSSBO);

    glBufferData(GL_SHADER_STORAGE_BUFFER, gpuObjects.size() * sizeof(GPUObject), gpuObjects.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, objectSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

AABB BVH::computeBounds(const glm::mat4& model)
{
    glm::vec3 corners[8] =
    {
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f}
    };

    glm::vec3 bmin(std::numeric_limits<float>::max());
    glm::vec3 bmax(std::numeric_limits<float>::lowest());

    for (int i = 0; i < 8; i++)
    {
        glm::vec3 p = glm::vec3(model * glm::vec4(corners[i], 1.0f));

        bmin = glm::min(bmin, p);
        bmax = glm::max(bmax, p);
    }

    return { bmin, bmax };
}


void BVH::destroy()
{
    if (bvhSSBO)
    {
        glDeleteBuffers(1, &bvhSSBO);
        bvhSSBO = 0;
    }

    if (objectSSBO)
    {
        glDeleteBuffers(1, &objectSSBO);
        objectSSBO = 0;
    }

    bvhNodes.clear();
    shadowObjects.clear();
    sceneObjects.clear();
}